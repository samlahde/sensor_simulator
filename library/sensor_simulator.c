#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "sensor_simulator.h"

#define RANDOM_MAXIMUM      (100 - 1)   // The maximum and minimum random value input from the user program to simulate_next_value().
#define RANDOM_MINIMUM      0

#define MAGIC_STD_SCALE 1.14            // A magic number for scaling the given standard deviation that somehow makes it more accurate.

union {
    double d;
    int i;
} bitwise_double;

void init_simulator(Simulator sim, double mean, double std_deviation, double max_step, double max_value, double min_value){
    sim->prev_value = mean;
    sim->direction = 1;
    sim->error_rate = 0;
    sim->error_min = 0;
    sim->error_max = 0;
    sim->target_mean = mean;
    sim->trend_speed = 0;

    sim->mean = mean;
    sim->std_deviation = std_deviation * MAGIC_STD_SCALE;
    sim->max_step = max_step;
    sim->steps_per_dir = 1;
    sim->steps_in_dir = sim->steps_per_dir;
    sim->max_value = max_value;
    sim->min_value = min_value;
}

void init_simulator_errors(Simulator sim, double error_rate, double error_min, double error_max){
    sim->error_rate = error_rate;
    sim->error_min = error_min;
    sim->error_max = error_max;
}

void set_starting_value(Simulator sim, double starting_value){
    if (starting_value > sim->max_value){
        starting_value = sim->max_value;
    }
    else if (starting_value < sim->min_value){
        starting_value = sim->min_value;
    }  
    sim->prev_value = starting_value;
}

void set_steps_per_direction(Simulator sim, int steps_per_direction){
    sim->steps_per_dir = steps_per_direction;
    sim->steps_in_dir = steps_per_direction;
}

void set_trend(Simulator sim, double target_mean, double trend_speed){
    sim->target_mean = target_mean;
    sim->trend_speed = trend_speed;
}


double simulate_next_value(Simulator sim, int random_number){
    double next_value;
    if (sim->mean != sim->target_mean){
        if (abs(sim->mean - sim->target_mean) < sim->trend_speed){
            sim->mean = sim->target_mean;
        }
        else if (sim->mean > sim->target_mean)
        {
            sim->mean -= sim->trend_speed;
        }
        else if (sim->mean < sim->target_mean)
        {
            sim->mean += sim->trend_speed;
        }        
    }

    if(next_is_error(sim, random_number)){
        next_value = get_error_value(sim, random_number);
    }
    else{
        next_value = std_dev_value(sim, random_number);
        next_value = double_to_range(next_value, sim->min_value, sim->max_value);
        sim->prev_value = next_value;
    }
    return next_value;
}

int next_is_error(Simulator sim, int random_number){
    bitwise_double.d = sim->prev_value;
    int seed = (bitwise_double.i >> 1) + random_number;
    return event_occurs(sim->error_rate, seed);
}

double get_error_value(Simulator sim, int seed){
    double error_value, error_range;
    error_range = sim->error_max - sim->error_min;
    error_value = re_randomize_lcg(seed, error_range) + sim->error_min;
    return error_value;
}

double std_dev_value(Simulator sim, int random_number){
    double step_lenght, new_value;

    step_lenght = (sim->max_step * random_number) / RANDOM_MAXIMUM;
    new_value = sim->prev_value + (sim->direction * step_lenght);
    sim->steps_in_dir++;
    if (sim->steps_in_dir >= sim->steps_per_dir){
        sim->direction = decide_direction(step_lenght, sim->prev_value, sim->max_step * sim->steps_per_dir, sim->mean, sim->std_deviation, random_number);
        sim->steps_in_dir = 0;
    }
    return new_value;
}

int decide_direction(double step_lenght, double prev_value, double max_step, double mean, double std_deviation, int seed){
    double z, probability, prob_high, prob_low;

    prob_low = std_probability_between(prev_value - max_step, prev_value, mean, std_deviation);
    prob_high = std_probability_between(prev_value, prev_value + max_step, mean, std_deviation);
    probability = prob_high / (prob_low + prob_high);
    int positive_direction = -(event_occurs(probability, seed));
    return (positive_direction & 1) | ((~positive_direction) & -1);
}

int event_occurs(double probability, int seed){
    int output;
    double random_value;
    random_value = re_randomize_lcg(seed, 1);
    output = (random_value <= probability);
    return output;
}

double re_randomize_lcg(int seed, double max_value){
    double value;
    value = (seed * 1103515245 + 12345) & INT_MAX;
    return ((value / INT_MAX) * max_value);
}

double double_to_range(double value, double min, double max){
    if(value > max) value = max;
    else if(value < min) value = min;
    return value;
}

double std_cdf(double x, double mean, double std_deviation) {
    return 0.5 * (1 + erf((x - mean) / (std_deviation * sqrt(2))));
}

double std_probability_between(double a, double b, double mean, double std_deviation) {
    return std_cdf(b, mean, std_deviation) - std_cdf(a, mean, std_deviation);
}
