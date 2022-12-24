#ifndef SENSOR_SIMULATOR_H
#define SENSOR_SIMULATOR_H

typedef struct Simulator_{
        double mean;
        double std_deviation;
        double max_step;
        double max_value;
        double min_value;

        double prev_value;
        int direction;
} *Simulator;

void init_simulator(Simulator sim, double mean, double std_deviation, double max_step, double max_value, double min_value);

double std_dev_value(int random_number, Simulator sim);

int decide_direction(double step_lenght, double prev_value, double mean, double std_deviation, int seed);

int force_to_range(int value, int min_value, int max_value);

double re_randomize_lcg(int seed, int max_value);

#endif /* SENSOR_SIMULATOR_H */