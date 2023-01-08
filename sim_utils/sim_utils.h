#include <stdlib.h>
#include "sensor_simulator.h"

/*
A simple loop that simulates values into output file using rand() as pseudorandomizer
sim: The simulator struct
iterations: The number of iterations
*output_file: The output file for the values, assumed to be a semicolon separated CSV
*/
void simulator_loop(Simulator sim, size_t iterations, FILE *output_file);
/*
A loop that moves the mean of the simulation. Number of iterations can be estimated with abs(target_mean - mean)/trend_speed.
sim: The simulator struct
target_mean: The target mean for the trend.
trend_speed: The value by which the mean will change each iteration.
*output_file: The output file for the values, assumed to be a semicolon separated CSV
*/
void simulator_trend_loop(Simulator sim, double target_mean, double trend_speed, FILE *output_file);
/*
Same as simulator_trend_loop, but moves between the target_mean and the starting mean X times.
sim: The simulator struct
iterations: The number of oscillations wanted
target_mean: The target mean for the trend.
trend_speed: The value by which the mean will change each iteration.
*output_file: The output file for the values, assumed to be a semicolon separated CSV
*/
void simulator_oscillating_trend_loop(Simulator sim, size_t iterations, double target_mean, double trend_speed, FILE *output_file);