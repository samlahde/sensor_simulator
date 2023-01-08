#ifndef SENSOR_SIMULATOR_H
#define SENSOR_SIMULATOR_H

/*
The simulator struct, used to store simulation configurations and changing variables such as prev_value between iterations.
*/
typedef struct Simulator_{
        double mean;
        double std_deviation;
        double max_step;
        int steps_per_dir;
        double max_value;
        double min_value;

        double error_rate;
        double error_min;
        double error_max;

        double target_mean;
        double trend_speed;

        double prev_value;
        int steps_in_dir;
        int direction;
} *Simulator;

/* USER FUNCTIONS*/

/*
Simulator initialization with minimum parameters.
mean: Mean of the values.
std_deviation: The standard deviation wanted for the values.
max_step: The maximum lenght the value will move in one iteration. The step lenght will be randomly chosen between zero and max_step.
max_value & min_value: The limits of the values returned. 
*/
void init_simulator(Simulator sim, double mean, double std_deviation, double max_step, double max_value, double min_value);
/*
Introduce error to the simulation.
error_rate: Probability of errors in decimal (0.1 = 10% chance of error).
error_min & error_max: Range of values where the error values will be distributed on.
*/
void init_simulator_errors(Simulator sim, double error_rate, double error_min, double error_max);
/*
Sets the starting point of the simulation. Default is the mean.
starting_value: The value where you want the next iteration of the simulation to start from.
*/
void set_starting_value(Simulator sim, double starting_value);
/*
If set to other than 1, this will force the simulation go in a certain direction X steps before re-evaluating.
steps_per_direction: The number of iterations the simulation will go in a direction before deciding again if the direction will be changed.
*/
void set_steps_per_direction(Simulator sim, int steps_per_direction);
/*
Add a trend to the simulation. This means that with each iteration, the mean of the simulator will change by trend_speed until the target_mean is reached.
target_mean: The target mean for the trend.
trend_speed: The value by which the mean will change each iteration.
*/
void set_trend(Simulator sim, double target_mean, double trend_speed);
/*
The simulation function. It is called every time a simulated value is wanted.
sim: A Simulator struct pointer initialized with the previous functions.
random_number: A random integer between RANDOM_MAXIMUM and RANDOM_MINIMUM generated by the user program.
*/
double simulate_next_value(Simulator sim, int random_number);

/* INTERNAL FUNCTIONS */

// Decide if the next value is an error value 0/1.
int next_is_error(Simulator sim, int random_number);
// Returns the error value
double get_error_value(Simulator sim, int seed);
// Returns the next deviated value
double std_dev_value(Simulator sim, int random_number);
// Returns the direction of the next value -1/1.
int decide_direction(double step_lenght, double prev_value, double max_step, double mean, double std_deviation, int seed);
// Returns the given value forced to the given range
double double_to_range(double value, double min, double max);
// Given a probability between 0 and 1, decides if the event occurs.
int event_occurs(double probability, int seed);
// A simple pseudorandom generator used to get a new random value from a seed.
double re_randomize_lcg(int seed, double max_value);
// Returns a standard distribution Z value.
double std_cdf(double x, double mean, double std_deviation);
// Calculates the probability of a value being between points a and b in a standard distributed set.
double std_probability_between(double a, double b, double mean, double std_deviation);

#endif /* SENSOR_SIMULATOR_H */