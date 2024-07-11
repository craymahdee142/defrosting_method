#include "main.h"


/* Function to generate a random float between min and max */
float random_float(float min, float max) {
    return min + ((float)rand() / RAND_MAX) * (max - min);
}



int main() {
    srand(time(NULL)); // Seed the random number generator

    TemperatureSensor temp_sensor = { read_temperature() };
    HumiditySensor humidity_sensor;
    DoorSensor door_sensor = { 0 }; // Door initially closed
    Compressor compressor = { COMPRESSOR_POWER };

    float compressor_energy_consumed = 0.0;
    float defrost_energy_consumed = 0.0;
    int defrost_needed = 0;

    // Variables for COP calculation
    float total_energy_consumed = 0.0;
    float total_heat_removed = 0.0;
    float mass_of_air = AIR_DENSITY * COLD_ROOM_VOLUME;

    // Initialize humidity sensor separately
    humidity_sensor.humidity = read_humidity(&humidity_sensor);

    for (int i = 0; i < SIMULATION_DURATION / CHECK_INTERVAL; i++) {
        float old_temperature = temp_sensor.temperature;
        float temperature_adjustment = 0.0;

        // Read ambient and surrounding temperatures
        float ambient_temperature = read_ambient_temperature();
        float surrounding_temperature = read_surrounding_temperature();

        adjust_for_door_state(&door_sensor, &temperature_adjustment);
        humidity_sensor.humidity = read_humidity(&humidity_sensor);

        simulate_compressor_effect(&temp_sensor.temperature, ambient_temperature, &compressor_energy_consumed);
        simulate_condenser_effect(&temp_sensor.temperature, surrounding_temperature);
        simulate_evaporator_effect(&temp_sensor.temperature, surrounding_temperature);
        temp_sensor.temperature += temperature_adjustment;
        simulate_fan_effect(&humidity_sensor);

        if ((temp_sensor.temperature <= 0 || humidity_sensor.humidity > 70) && !defrost_needed) {
            printf("Conditions met for defrosting. Initiating defrost...\n");
            defrost_needed = 1;
            defrost_energy_consumed = 0; // Reset at the start of a defrost cycle
        }

        if (defrost_needed) {
            simulate_heater_effect(&temp_sensor, &defrost_energy_consumed);
            total_energy_consumed += defrost_energy_consumed;

            printf("Defrost Energy Consumed: %.2f Joules\n", defrost_energy_consumed);

            defrost_needed = 0; // Reset defrost need
        }

        float delta_t = old_temperature - temp_sensor.temperature;
        float heat_removed = mass_of_air * SPECIFIC_HEAT_CAPACITY_AIR * delta_t;
        total_heat_removed += heat_removed;

        printf("Current temperature: %.2f°C, Humidity: %d%%, Door: %s, Ambient Temperature: %.2f°C, Surrounding Temperature: %.2f°C\n",
               temp_sensor.temperature, humidity_sensor.humidity, door_sensor.isOpen ? "Open" : "Closed", ambient_temperature, surrounding_temperature);

        sleep(CHECK_INTERVAL); // Wait for CHECK_INTERVAL seconds before the next cycle
    }

    total_energy_consumed = compressor_energy_consumed + defrost_energy_consumed;

    // Example temperature values for COP calculation (replace these with actual readings)
    float T_evap_in = -5.0;
    float T_evap_out = 5.0;
    float T_comp_in = 5.0;
    float T_comp_out = 30.0;

    // Example specific heats (Cp) for R-134a (replace with actual values from tables)
    float Cp_evap = 0.85; // Example value
    float Cp_comp = 0.90; // Example value

    // Example mass flow rate (replace with actual value)
    float mass_flow_rate = 0.05; // Example value

    float cop = calculate_cop(T_evap_in, T_evap_out, T_comp_in, T_comp_out, mass_flow_rate, Cp_evap, Cp_comp);

    printf("Compressor Energy Consumed: %.2f Joules\n", compressor_energy_consumed);
    printf("Defrost Energy Consumed: %.2f Joules\n", defrost_energy_consumed);
    printf("Total Energy Consumed: %.2f Joules\n", total_energy_consumed);
    printf("Total Heat Removed: %.2f Joules\n", total_heat_removed);
    printf("Overall COP: %.2f\n", cop);

    return 0;
}
