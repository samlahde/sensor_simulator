#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sensor_simulator.h"

#define RANDOM_NUMBER_RANGE 100

void simulator_loop(Simulator sim, size_t iterations, FILE *output_file){
    srand((unsigned int)time(0));
    int random_number = 0;
    for (size_t i = 0; i < iterations; i++)
    {
        double value;
        random_number = rand() / (RAND_MAX / (RANDOM_NUMBER_RANGE));
        value = simulate_next_value(sim, random_number);
        fprintf(output_file, "%f;\n", value);
    }
}

void simulator_trend_loop(Simulator sim, double target_mean, double trend_speed, FILE *output_file){
    srand((unsigned int)time(0));
    int random_number = 0;
    set_trend(sim, target_mean, trend_speed);
    while (sim->mean != target_mean)
    {
        double value;
        random_number = rand() / (RAND_MAX / (RANDOM_NUMBER_RANGE));
        value = simulate_next_value(sim, random_number);
        fprintf(output_file, "%f;\n", value);
    }
}

void simulator_oscillating_trend_loop(Simulator sim, size_t iterations, double target_mean, double trend_speed, FILE *output_file){
    srand((unsigned int)time(0));
    double start_mean;
    int random_number = 0;
    for (size_t i = 0; i <= iterations * 2; i++){
        start_mean = sim->mean;
        set_trend(sim, target_mean, trend_speed);
        while (sim->mean != target_mean)
        {
            double value;
            random_number = rand() / (RAND_MAX / (RANDOM_NUMBER_RANGE));
            value = simulate_next_value(sim, random_number);
            fprintf(output_file, "%f;\n", value);
        }
        target_mean = start_mean;
    }
}