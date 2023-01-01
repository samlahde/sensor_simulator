#ifndef SENSOR_SIMULATOR_H
#define SENSOR_SIMULATOR_H

typedef struct Simulator_{
        double mean;
        double std_deviation;
        double max_step;
        double max_value;
        double min_value;

        double error_rate;
        double error_min;
        double error_max;

        double prev_value;
        int direction;
} *Simulator;

void init_simulator(Simulator sim, double mean, double std_deviation, double max_step, double max_value, double min_value);

void init_simulator_errors(Simulator sim, double error_rate, double error_min, double error_max);

double simulate_next_value(Simulator sim, int random_number);

int next_is_error(Simulator sim, int random_number);

double get_error_value(Simulator sim, int seed);

double std_dev_value(Simulator sim, int random_number);

int decide_direction(double step_lenght, double prev_value, double mean, double std_deviation, int seed);

int force_to_range(int value, int min_value, int max_value);

double double_to_range(double value, double min, double max);

int event_occurs(double probability, int seed);

double re_randomize_lcg(int seed, double max_value);

#endif /* SENSOR_SIMULATOR_H */