#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sensor_simulator.h"

#define RANDOM_NUMBER_RANGE 100

// Path of the CSV (or other) file. 
#define OUTPUT_FILEPATH "output/data.csv"

// The number of simulated values wanted
#define ITERATIONS      1000000

// Simulator minimum parameters
#define MEAN            0
#define STD_DEV         10
#define MAX_STEP        1
#define MIN_VALUE       -100
#define MAX_VALUE       100

// Error parameters
#define ERROR_RATE      0
#define ERROR_MIN       -15
#define ERROR_MAX       15

// Trend configuration
#define TREND_TARGET    0
#define TREND_SPEED     0.01

int main(int argc, char const *argv[])
{
    srand((unsigned int)time(0));
    int random_number = 0;

    FILE *output_file = fopen(OUTPUT_FILEPATH, "w+");
    if( output_file == NULL ) {
        printf("Could not open file.");
        exit(-1);
    }
    fprintf(output_file, "Data;\n");
    
    Simulator sim = (Simulator)malloc(sizeof(Simulator_));
    init_simulator(sim, MEAN, STD_DEV, MAX_STEP, MAX_VALUE, MIN_VALUE);
    init_simulator_errors(sim, ERROR_RATE, ERROR_MIN, ERROR_MAX);
    set_trend(sim, TREND_TARGET, TREND_SPEED);
    for (size_t i = 0; i < ITERATIONS; i++)
    {
        double value;
        random_number = rand() / (RAND_MAX / (RANDOM_NUMBER_RANGE));
        value = simulate_next_value(sim, random_number);
        fprintf(output_file, "%f;\n", value);
    }
    fclose(output_file);
    free(sim);
    return 0;
}
