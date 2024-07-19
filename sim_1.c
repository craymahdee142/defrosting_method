#include "main.h"


void simulate_heater_effect(TemperatureSensor* temp_sensor, float* defrost_energy_consumed) {
    double initial_temp = temp_sensor->temperature;


    // Example duration of defrost cycle in seconds
    double defrost_duration_seconds = 5.0; // Example duration, replace with actual value if applicable


    // Calculate energy consumption based on system performance
    double energy_consumed = (HEATER_POWER / heater_efficiency) * defrost_duration_seconds / 3600.0; // Convert to Wh


    float temp_increase = 5.0;
    temp_sensor->temperature += temp_increase;
    *defrost_energy_consumed += energy_consumed;
}

void simulate_fan_effect(HumiditySensor* humidity_sensor) {
    float humidity_decrease = random_float(1, 5);
    humidity_sensor->humidity -= humidity_decrease;
    if (humidity_sensor->humidity < 0) {
        humidity_sensor->humidity = 0;
    }
}





int read_humidity(HumiditySensor* humidity_sensor) {
    humidity_sensor->humidity = random_float(30, 50);
    return humidity_sensor->humidity;
}

float read_temperature() {
    return random_float(-5.0, 25.0);
}

void adjust_for_door_state(DoorSensor* door_sensor, float* temperature_adjustment) {
    if (door_sensor->isOpen) {
        *temperature_adjustment += 1.0;
    }
}

void simulate_hot_gas_bypass_effect(HotGasBypass* hot_gas_bypass, TemperatureSensor* temp_sensor) {
    if (hot_gas_bypass->isActive) {
        float temp_increase = 1.0;
        temp_sensor->temperature += temp_increase;
    }
}

float read_ambient_temperature() {
    return random_float(15.0, 35.0);
}

float read_surrounding_temperature() {
    return random_float(10.0, 30.0);
}

