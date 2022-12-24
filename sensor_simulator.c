#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include "sensor_simulator.h"

#define RANDOM_SCALE        8
#define RANDOM_MAXIMUM      ((1 << RANDOM_SCALE) - 1)
#define RANDOM_MINIMUM      0

void init_simulator(Simulator sim, double mean, double std_deviation, double max_step, double max_value, double min_value){
    sim->prev_value = mean;
    sim->direction = 1;
    sim->mean = mean;
    sim->std_deviation = std_deviation;
    if(max_step > std_deviation / 2){
        max_step = std_deviation / 2;
    } 
    sim->max_step = max_step;
    sim->max_value = max_value;
    sim->min_value = min_value;
}

double std_dev_value(int random_number, Simulator sim){
    double step_lenght, new_value;
    int direction;

    random_number = force_to_range(random_number, RANDOM_MINIMUM, RANDOM_MAXIMUM);
    step_lenght = (sim->max_step * random_number) / RANDOM_MAXIMUM; //check precision!!
    //printf("Rand: %d B4 shift: %f StepL: %.2f\n", random_number, sim->max_step*random_number, step_lenght);

    direction = decide_direction(step_lenght, sim->prev_value, sim->mean, sim->std_deviation, random_number);
    //printf("Direction: %d\n", direction);
    new_value = sim->prev_value + (direction * step_lenght);
    sim->prev_value = new_value;
    return new_value;

}

int decide_direction(double step_lenght, double prev_value, double mean, double std_deviation, int seed){
    double z, probability, random_2;

    z = (prev_value + step_lenght - mean) / std_deviation;

    probability = 0.5 * (1 + erf(z / sqrt(2)));

    random_2 = re_randomize_lcg(seed, 1);

    int positive_direction = -(random_2 >= probability);
    //printf("Prob: %f Rand: %f Pos: %d\n", probability, random_2, positive_direction);
    //printf("Pos: %d\n", positive_direction);
    return (positive_direction & 1) | ((~positive_direction) & -1);
}

double re_randomize_lcg(int seed, int max_value)
{
    seed = (seed * 1103515245 + 12345) & INT_MAX;

    return (double)seed / INT_MAX;
}

int force_to_range(int value, int min_value, int max_value){
    int is_smaller = -(value <= max_value);
    int is_larger = -(value >= min_value);
    value = (is_smaller & value) | ((~is_smaller) & max_value);
    value = (is_larger & value) | ((~is_larger) & min_value);
    return value;
}