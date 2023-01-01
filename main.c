#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sensor_simulator.h"

#define MEAN 60
#define STD_DEV 10
#define MAX_STEP 0.1
#define MIN_VALUE -100
#define MAX_VALUE 100

#define ERROR_RATE 0
#define ERROR_MIN 15
#define ERROR_MAX 25

#define RANDOM_NUMBER_RANGE 100

#define ITERATIONS 100000

int main(int argc, char const *argv[])
{
    srand((unsigned int)time(0));
    double max_val = MEAN;
    double min_val = MEAN;
    int rand_count = 0;
    int random_number = 0;
    int val_mean_count = 0;

    FILE *fpt = fopen("data.csv", "w+");
    if( fpt == NULL ) {
        printf("Could not open file.");
        exit(-1);
    }
    fprintf(fpt, "Data; Input\n");
    
    Simulator sim = (Simulator)malloc(sizeof(Simulator_));
    init_simulator(sim, MEAN, STD_DEV, MAX_STEP, MAX_VALUE, MIN_VALUE);
    init_simulator_errors(sim, ERROR_RATE, ERROR_MIN, ERROR_MAX);
    for (size_t i = 0; i < ITERATIONS; i++)
    {
        double value;
        random_number = rand() / (RAND_MAX / (RANDOM_NUMBER_RANGE));
        // value = re_randomize_lcg(random_number, 1);
        // if(value > 0.5){
        //     rand_count++;
        // }
        // if(value < 0.5){
        //     rand_count--;
        // }
        value = simulate_next_value(sim, random_number);
        fprintf(fpt, "%f; %d\n", value, random_number);
        // char buffer[100];
        // sprintf(buffer, "%4.4f", value); 
        // printf(buffer);
        // fwrite(buffer, sizeof(buffer), 1, fpt);
        // if(value > max_val){
        //     max_val = value;
        // }
        // if(value < min_val){
        //     min_val = value;
        // }
        // if(value > MEAN){
        //     val_mean_count++;
        // }
        // if(value < MEAN){
        //     val_mean_count--;
        // }
        // if(random_number >= 128){
        //     rand_count++;
        // }
        // if(random_number <= 127){
        //     rand_count--;
        // }
        // //printf("%f\n", value);
    }
    // printf("Max: %f Min: %f\n", max_val, min_val);
    //printf("Rand count: %d", rand_count);
    // printf("Val mean count: %d", val_mean_count);
    fclose(fpt);
    free(sim);
    return 0;
}
