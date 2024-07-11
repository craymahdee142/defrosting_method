#include "main.h"


/* Simulate compressor effect */
void simulate_compressor_effect(float* temperature, float ambient_temperature, float* energy_consumed) {
    float temp_decrease = 2.0; // Base temperature decrease due to compressor

    // Modify the temperature decrease based on ambient temperature
    if (ambient_temperature > 25.0) {
        temp_decrease -= (ambient_temperature - 25.0) * 0.05; // Example adjustment
    } else if (ambient_temperature < 15.0) {
        temp_decrease += (15.0 - ambient_temperature) * 0.05; // Example adjustment
    }

    *temperature -= temp_decrease;
    *energy_consumed += COMPRESSOR_POWER * CHECK_INTERVAL / 3600.0; // Accumulate energy consumed
}

/* Simulate condenser effect */
void simulate_condenser_effect(float* temperature, float surrounding_temperature) {
    float temp_decrease = 2.0; // Base temperature decrease due to condenser

    // Modify the temperature decrease based on surrounding temperature
    if (surrounding_temperature > 25.0) {
        temp_decrease -= (surrounding_temperature - 25.0) * 0.05; // Example adjustment
    } else if (surrounding_temperature < 15.0) {
        temp_decrease += (15.0 - surrounding_temperature) * 0.05; // Example adjustment
    }

    *temperature -= temp_decrease;
}

/* Simulate evaporator effect */
void simulate_evaporator_effect(float* temperature, float surrounding_temperature) {
    float temp_increase = 2.0; // Base temperature increase due to evaporator

    // Modify the temperature increase based on surrounding temperature
    if (surrounding_temperature > 25.0) {
        temp_increase += (surrounding_temperature - 25.0) * 0.05; // Example adjustment
    } else if (surrounding_temperature < 15.0) {
        temp_increase -= (15.0 - surrounding_temperature) * 0.05; // Example adjustment
    }

    *temperature += temp_increase;
}

/* Simulate heater effect */
void simulate_heater_effect(TemperatureSensor* temp_sensor, float* energy_consumed) {
    float temp_increase = 5.0; // Example temperature increase due to heater
    temp_sensor->temperature += temp_increase;
    *energy_consumed += HEATER_POWER * CHECK_INTERVAL / 3600.0; // Accumulate energy consumed
}

/* Simulate fan effect */
void simulate_fan_effect(HumiditySensor* humidity_sensor) {
    float humidity_decrease = random_float(1, 5); // Example decrease in humidity due to fan
    humidity_sensor->humidity -= humidity_decrease;
    if (humidity_sensor->humidity < 0) {
        humidity_sensor->humidity = 0;
    }
}


/* Simulate expansion valve effect */
void simulate_expansion_valve_effect(ExpansionValve* expansion_valve) {
    if (expansion_valve->isOpen) {
        // Implement logic to simulate expansion valve effect
        // Example: Controlling refrigerant flow
    }
}

/* Simulate solenoid effect */
void simulate_solenoid_valve_effect(SolenoidValve* solenoid_valve) {
    if (solenoid_valve->isOn) {
        // Implement logic to simulate solenoid valve effect
        // Example: Controlling flow of another fluid or gas
    }
}
