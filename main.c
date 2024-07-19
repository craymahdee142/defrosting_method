#include "main.h"

int main() {
    srand(time(NULL));

    TemperatureSensor temp_sensor;
    DoorSensor door_sensor;
    HumiditySensor humidity_sensor;
    Compressor compressor;
    Condenser condenser;
    Evaporator evaporator;
    ExpansionValve expansion_valve;
    SolenoidValve solenoid_valve;
    HotGasBypass hot_gas_bypass;

    float mass_flow_rate = 3.0;
    float compressor_energy_consumed = 0.0;
    float defrost_energy_consumed = 0.0;
    float total_energy_consumed = 0.0;
    float total_heat_removed = 0.0;
    float Q_evap = 0.0;
    int defrost_needed = 0;
    float mass_of_air = AIR_DENSITY * COLD_ROOM_VOLUME;
    int last_defrost_time = 0;

    initialize_refrigerant_data();

    /*
    RefrigerantProperties* refrigerant_data_liq = readLiquidDataset();
    RefrigerantProperties* refrigerant_data_vap = readVaporDataset();
    RefrigerantProperties* refrigerant_data_sup = readSuperheatedDataset();

    
    if (!refrigerant_data_liq) {
        fprintf(stderr, "Error: Failed to read liquid refrigerant data from file\n");
        return EXIT_FAILURE;
    }

    if (!refrigerant_data_vap) {
        fprintf(stderr, "Error: Failed to read vapor refrigerant data from file\n");
        return EXIT_FAILURE;
    }

    if (!refrigerant_data_sup) {
        fprintf(stderr, "Error: Failed to read superheated refrigerant data from file\n");
        return EXIT_FAILURE;
    }
*/
    initialize_sensors(&temp_sensor, &door_sensor, &humidity_sensor);
    initialize_components(&compressor, &condenser, &evaporator, &expansion_valve, &solenoid_valve, &hot_gas_bypass);

    for (int i = 0; i < SIMULATION_DURATION / CHECK_INTERVAL; i++) {
        temp_sensor.temperature = read_temperature();
        humidity_sensor.humidity = read_humidity(&humidity_sensor);
        float ambient_temperature = read_ambient_temperature();
        float surrounding_temperature = read_surrounding_temperature();

        float temperature_adjustment = 0.0;
        adjust_for_door_state(&door_sensor, &temperature_adjustment);
        temp_sensor.temperature += temperature_adjustment;

        if (compressor.isOn) {
            simulate_compressor_effect(&temp_sensor.temperature, ambient_temperature, &compressor_energy_consumed);
        }

        simulate_condenser_effect(&temp_sensor.temperature, surrounding_temperature);
        simulate_evaporator_effect(&temp_sensor.temperature, surrounding_temperature);
        simulate_solenoid_valve_effect(&solenoid_valve);
        simulate_expansion_valve_effect(&expansion_valve);
        simulate_hot_gas_bypass_effect(&hot_gas_bypass, &temp_sensor);

        control_system(&temp_sensor, &door_sensor, &humidity_sensor, &compressor, &condenser, &evaporator, &expansion_valve, &solenoid_valve, &hot_gas_bypass, &compressor_energy_consumed);

        // Calculate enthalpy based on current conditions
        float h_1 = calculate_enthalpy(temp_sensor.temperature, ambient_temperature, surrounding_temperature, PRESSURE_LOW);
        float h_2 = calculate_enthalpy(temp_sensor.temperature, ambient_temperature, surrounding_temperature, PRESSURE_HIGH);
        float h_3 = calculate_enthalpy(temp_sensor.temperature, ambient_temperature, surrounding_temperature, PRESSURE_HIGH);
        float h_4 = calculate_enthalpy(temp_sensor.temperature, ambient_temperature, surrounding_temperature, PRESSURE_LOW);

        // Debugging: Print enthalpy values
        printf("h_1: %.2f, h_2: %.2f, h_3: %.2f, h_4: %.2f\n", h_1, h_2, h_3, h_4);

        // Ensure the enthalpy values are valid
        if (isnan(h_1) || isnan(h_2) || isnan(h_3) || isnan(h_4)) {
            fprintf(stderr, "Error: Invalid enthalpy values\n");
            free(refrigerantDataLiq);
    	    free(refrigerantDataVap);
    	    free(refrigerantDataSup);
            return EXIT_FAILURE;
        }

        Q_evap = mass_flow_rate * (h_1 - h_4);
        total_energy_consumed += compressor_energy_consumed + defrost_energy_consumed + Q_evap;

        float temp_diff = AMBIENT_TEMPERATURE - temp_sensor.temperature;
        total_heat_removed += AIR_SPECIFIC_HEAT * mass_of_air * temp_diff / 3600.0;

        if ((temp_sensor.temperature <= 0 || humidity_sensor.humidity > 50) && !defrost_needed && (i * CHECK_INTERVAL - last_defrost_time >= MINIMUM_DEFROST_INTERVAL)) {
            printf("Conditions met for defrosting. Initiating defrost...\n");
            defrost_needed = 1;
            defrost_energy_consumed = 0.0;
            last_defrost_time = i * CHECK_INTERVAL;
        }

        if (defrost_needed) {
            simulate_heater_effect(&temp_sensor, &defrost_energy_consumed);
            total_energy_consumed += defrost_energy_consumed;

            printf("Defrost Energy Consumed: %.2f Wh\n", defrost_energy_consumed);

            defrost_needed = 0;
        }

        float cop = calculate_cop(h_1, h_2, h_3, h_4, mass_flow_rate);

        printf("Time: %d seconds\n", i * CHECK_INTERVAL);
        printf("Temperature: %.2f°C\n", temp_sensor.temperature);
        printf("Humidity: %.2f%%\n", humidity_sensor.humidity);
        printf("Compressor Energy Consumed: %.2f Wh\n", compressor_energy_consumed);
        printf("Defrost Energy Consumed: %.2f Wh\n", defrost_energy_consumed);
        printf("Evaporator Absorbed Energy: %.2f Wh\n", Q_evap);
        printf("Total Energy Consumed: %.2f Wh\n", total_energy_consumed);
        printf("Total Heat Removed: %.2f Wh\n", total_heat_removed);

        // Ensure total energy consumed is non-zero before calculating COP
        if (total_energy_consumed == 0.0) {
            printf("COP: N/A (Total energy consumed is zero)\n");
        } else {
            printf("COP: %.2f\n", cop);
        }
        printf("\n");

        usleep(CHECK_INTERVAL * 1000000);
    }

    /* Free dynamically allocated memory */
    free(refrigerantDataLiq);
    free(refrigerantDataVap);
    free(refrigerantDataSup);

    return 0;
}

