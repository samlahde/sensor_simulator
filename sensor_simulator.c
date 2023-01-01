#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <limits.h>
#include "sensor_simulator.h"

#define RANDOM_MAXIMUM      (100 - 1)
#define RANDOM_MINIMUM      0

union {
    double d;
    int i;
} bitwise_double;

double std_cdf(double x, double mean, double std_deviation) {
    return 0.5 * (1 + erf((x - mean) / (std_deviation * sqrt(2))));
}

double std_probability_between(double a, double b, double mean, double std_deviation) {
    return std_cdf(b, mean, std_deviation) - std_cdf(a, mean, std_deviation);
}

void init_simulator(Simulator sim, double mean, double std_deviation, double max_step, double max_value, double min_value){
    sim->prev_value = mean;
    sim->direction = 1;
    sim->error_rate = 0;
    sim->error_min = 0;
    sim->error_max = 0;

    sim->mean = mean;
    sim->std_deviation = std_deviation;
    if(max_step > std_deviation / 2){
        max_step = std_deviation / 2;
    } 
    sim->max_step = max_step;
    sim->max_value = max_value;
    sim->min_value = min_value;
}

void init_simulator_errors(Simulator sim, double error_rate, double error_min, double error_max){
    sim->error_rate = error_rate;
    sim->error_min = error_min;
    sim->error_max = error_max;
}

double simulate_next_value(Simulator sim, int random_number){
    double next_value;
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

/*
Decide if the next value is an error value. Here the previous value is used in the seed to prevent having to use the same seed again later in the iteration.
*/
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

    random_number = force_to_range(random_number, RANDOM_MINIMUM, RANDOM_MAXIMUM);
    step_lenght = (sim->max_step * random_number) / RANDOM_MAXIMUM; //check precision!!
    //printf("Rand: %d B4 shift: %f StepL: %.2f\n", random_number, sim->max_step*random_number, step_lenght);
    new_value = sim->prev_value + (sim->direction * step_lenght);

    sim->direction = decide_direction(step_lenght, sim->prev_value, sim->mean, sim->std_deviation, random_number);
    //printf("Direction: %d\n", direction);  
    return new_value;
}

int decide_direction(double step_lenght, double prev_value, double mean, double std_deviation, int seed){
    double z, probability, prob_high, prob_low, prob_high_s, prob_low_s;

    //z = (prev_value + step_lenght - mean) / std_deviation;

    //probability = 0.5 * (1 + erf(z / sqrt(2)));
    //probability = (std_deviation / 2) - ((prev_value + step_lenght - mean) / 50);
    prob_low = std_probability_between(prev_value - step_lenght, prev_value, mean, std_deviation);
    prob_high = std_probability_between(prev_value, prev_value + step_lenght, mean, std_deviation);
    prob_low_s = prob_low / (prob_low + prob_high);
    prob_high_s = prob_high / (prob_low + prob_high);
    probability = prob_high_s;

    printf("h:%f l:%f\n", prob_high_s, prob_low_s);

    int positive_direction = -(event_occurs(probability, seed));
    //printf("Prob: %f Pos: %d\n", probability, positive_direction);
    //printf("Pos: %d\n", positive_direction);
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

int force_to_range(int value, int min_value, int max_value){
    int is_smaller = -(value <= max_value);
    int is_larger = -(value >= min_value);
    value = (is_smaller & value) | ((~is_smaller) & max_value);
    value = (is_larger & value) | ((~is_larger) & min_value);
    return value;
}

