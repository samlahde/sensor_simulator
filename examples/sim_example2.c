#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sensor_simulator.h"
#include "sim_utils.h"

#define RANDOM_NUMBER_RANGE 100

// Path of the CSV (or other) file. 
#define OUTPUT_FILEPATH "output/data.csv"

// Simulator starting parameters
#define MEAN            0
#define STD_DEV         10
#define MAX_STEP        1
#define MIN_VALUE       -1000
#define MAX_VALUE       1000

int main(int argc, char const *argv[])
{
    // Create the output CSV file
    FILE *output_file = fopen(OUTPUT_FILEPATH, "w+");
    if( output_file == NULL ) {
        printf("Could not open file.");
        exit(-1);
    }
    fprintf(output_file, "Data;\n");
    
    // Initialize the simulator
    Simulator sim = (Simulator)malloc(sizeof(Simulator_));
    init_simulator(sim, MEAN, STD_DEV, MAX_STEP, MAX_VALUE, MIN_VALUE);

    // Loop 100000 times around 0, starting from -10
    set_starting_value(sim, -10);
    simulator_loop(sim, 100000, output_file);

    // Move to 50 with a speed of 0.1/
    simulator_trend_loop(sim, 50, 0.1, output_file);

    // Add 1% error to the simulation with values between 30 and 70
    init_simulator_errors(sim, 0.01, 30, 70);

    // Loop 100000 times
    simulator_loop(sim, 100000, output_file);

    // Remove error
    sim->error_rate = 0;

    // Move to 150 with a speed of 0.01
    simulator_trend_loop(sim, 150, 0.01, output_file);

    // Oscillate between 150 and 300 10 times with a speed of 0.01
    simulator_oscillating_trend_loop(sim, 10, 300, 0.01, output_file);

    fclose(output_file);
    free(sim);
    return 0;
}
