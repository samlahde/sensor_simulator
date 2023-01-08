# Sensor simulator

Sensor simulator is a fast and simple C-library for simulating sensor values.


## How it works

Sensor simulator at it's core 
In practice you configure the simulator with parameters suitable to you particular use case, and then call the functions repeatedly. The simulator itself does not contain any looping, it always returns just a single value. Also, to make the simulator as platform-independent as possible, the user program must also provide the pseudorandomized variables for each iteration. You can use the looping functions provided in *sim_utils* for testing.

### The *Simulator* struct

The **Simulator** struct is the object that holds the configurations for the simulation, and the persistency of values over iterations. You can directly change the parameters within the struct during simulation, or preferably use the provided initialization functions. Example of minimal initialization:
```
Simulator sim = (Simulator)malloc(sizeof(Simulator_));

init_simulator(sim, mean, standard_deviation, max_step, max_value, min_value);
```

### init_simulator()
Used for initializing the simulator with the minimum parameters. This should be called first before using any other functions.
```
void  init_simulator(Simulator  sim, double  mean, double  std_deviation, double  max_step, double  max_value, double  min_value);
```
**sim**: The simulator struct. 
**mean**: The mean of the value distribution.
**std_deviation**: The value of the standard deviation.
**max_step**: The maximum value by which the value changes each iteration. The actual value will be between 0 and max_step.
**max_value & min_value**: Hard limits for the values returned.