#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sensor_simulator.h"

#define MEAN 20
#define STD_DEV 10
#define STEP_SIZE 3
#define MAX_STEP 3
#define MIN_VALUE -100
#define MAX_VALUE 100
#define RANDOM_NUMBER_RANGE (1 << 8)

int main(int argc, char const *argv[])
{
    srand((unsigned int)time(0));
    double max_val = MEAN;
    double min_val = MEAN;
    int rand_count = 0;
    int val_mean_count = 0;

    FILE *fpt = fopen("D://data.csv", "w+");
    if( fpt == NULL ) {
        printf("Could not open file.");
        exit(-1);
    }
    
    Simulator sim = malloc(sizeof(Simulator));
    init_simulator(sim, MEAN, STD_DEV, MAX_STEP, MAX_VALUE, MIN_VALUE);
    for (size_t i = 0; i < 100; i++)
    {
        int random_number;
        double value;
        random_number = rand() / (RAND_MAX / (RANDOM_NUMBER_RANGE + 1));
        // value = re_randomize_lcg(random_number, 1);
        // if(value > 0.5){
        //     rand_count++;
        // }
        // if(value < 0.5){
        //     rand_count--;
        // }
        value = std_dev_value(random_number, sim);
        char buffer[100];
        sprintf(buffer, "%4.4f", value); 
        printf(buffer);
        fwrite(buffer, sizeof(buffer), 1, fpt);
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
    printf("Rand count: %d", rand_count);
    // printf("Val mean count: %d", val_mean_count);
    fclose(fpt);
    free(sim);
    return 0;
}
