#include "main.h"


/* Read humidity value */
int read_humidity(HumiditySensor* humidity_sensor) {
    humidity_sensor->humidity = (int)random_float(30, 50); // Generate a random humidity value between 30% and 50%
    return humidity_sensor->humidity;
}


/* Function to simulate reading a temperature value */
float read_temperature() {
    return random_float(-5.0, 25.0);
}


void adjust_for_door_state(DoorSensor* door_sensor, float* temperature_adjustment) {
    if (door_sensor->isOpen) {
        // Adjust temperature based on door state
        *temperature_adjustment += 1.0; // Example adjustment value when door is open
    }
}


/* Hot gas */
void simulate_hot_gas_bypass_effect(HotGasBypass* hot_gas_bypass, TemperatureSensor* temp_sensor) {
    if (hot_gas_bypass->isActive) {
        // Simulate effect of hot gas bypass
        float temp_increase = 1.0; // Example temperature increase due to hot gas bypass
        temp_sensor->temperature += temp_increase;
    }
}

