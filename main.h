
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h> // For boolean type

/* Simulation parameters */
#define SIMULATION_DURATION 86400 /* Simulate for 24 hours (in seconds) */
#define CHECK_INTERVAL 5 /* Check sensors every 5 seconds */


/* Constants */
#define COLD_ROOM_VOLUME 72.0 // in cubic meters (6m x 4m x 3m)
#define AIR_DENSITY 1.2 // in kg/m^3
#define SPECIFIC_HEAT_CAPACITY_AIR 1005.0 // in J/kg·°C
#define COMPRESSOR_POWER 2.0 // Example value in kW
#define HEATER_POWER 3.0 // Example value in kW
#define CHECK_INTERVAL 1 // Check interval in seconds
#define SIMULATION_DURATION 10 // Total simulation duration in seconds

/* Sensor Structures */
typedef struct {
    float temperature;
} TemperatureSensor;

typedef struct {
    bool isOpen;
} DoorSensor;

typedef struct {
    float humidity;
} HumiditySensor;

/* System Components */
typedef struct {
    bool isOn;
    float power;
} Compressor;

typedef struct {
    bool isOn;
} Condenser;

typedef struct {
    bool isOn;
} Evaporator;

typedef struct {
    bool isOpen;
} ExpansionValve;

typedef struct {
    bool isOn;
} SolenoidValve;

typedef struct {
    bool isActive;
} HotGasBypass;


/* Control functions*/
float random_float(float min, float max);
float read_ambient_temperature();
float read_surrounding_temperature();

/* Prototypes */
float read_temperature(TemperatureSensor* temp_sensor); /* Reads the temperature from the temperature sensor */
bool read_door_sensor(DoorSensor* door_sensor); /* Reads the state of the door sensor (open/closed) */
float read_humidity(HumiditySensor* humidity_sensor); /* Reads the humidity from the humidity sensor */

float calculate_cop(float T_evap_in, float T_evap_out, float T_comp_in, float T_comp_out, float mass_flow_rate, float Cp_evap, float Cp_comp);


/* Prototypes */
void simulate_compressor_effect(TemperatureSensor* temp_sensor, Compressor* compressor, float external_temp, float* energy_consumed); /* Simulates the effect of the compressor on the internal temperature */
void simulate_condenser_effect(Condenser* condenser); /* Simulates the effect of the condenser */
void simulate_evaporator_effect(Evaporator* evaporator); /* Simulates the effect of the evaporator */
void simulate_expansion_valve_effect(ExpansionValve* expansion_valve); /* Simulates the effect of the expansion valve */
void simulate_solenoid_valve_effect(SolenoidValve* solenoid_valve); /* Simulates the effect of the solenoid valve */
void simulate_hot_gas_bypass_effect(HotGasBypass* hot_gas_bypass, TemperatureSensor* temp_sensor); /* Simulates the effect of the hot gas bypass */
void simulate_heater_effect(TemperatureSensor* temp_sensor, float* energy_consumed); /* Simulates the effect of the heater on the internal temperature */
void simulate_fan_effect(HumiditySensor* humidity_sensor); /* Simulates the effect of the fan on the internal humidity level */
void adjust_for_door_state(DoorSensor* door_sensor, float* temperature_adjustment); /* Adjusts the internal temperature based on the door state (open/closed) */

void initialize_sensors(TemperatureSensor* temp_sensor, DoorSensor* door_sensor, HumiditySensor* humidity_sensor); /* Initializes the sensors with initial values */
void initialize_components(Compressor* compressor, Condenser* condenser, Evaporator* evaporator, ExpansionValve* expansion_valve, SolenoidValve* solenoid_valve, HotGasBypass* hot_gas_bypass); /* Initializes the system components */

void update_sensors(TemperatureSensor* temp_sensor, DoorSensor* door_sensor, HumiditySensor* humidity_sensor); /* Updates the sensor readings */
void control_system(TemperatureSensor* temp_sensor, DoorSensor* door_sensor, HumiditySensor* humidity_sensor, Compressor* compressor, Condenser* condenser, Evaporator* evaporator, ExpansionValve* expansion_valve, SolenoidValve* solenoid_valve, HotGasBypass* hot_gas_bypass); /* Controls the system based on sensor readings */

void simulate_system(float external_temp); /* Simulates the entire system over the specified duration */

#endif
