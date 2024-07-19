#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include <time.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h> // For boolean type

#define heater_efficiency 0.85 // Example efficiency of the heating system

/* Simulation parameters */
#define SIMULATION_DURATION 3600 /* Simulate for 24 hours (in seconds) */
#define CHECK_INTERVAL 5 /* Check sensors every 5 seconds */
#define MINIMUM_DEFROST_INTERVAL 60

/* Constants */
#define COLD_ROOM_VOLUME 0.54 // in cubic meters (6m x 4m x 3m)
#define AIR_DENSITY 1.2 // in kg/m^3
#define AIR_SPECIFIC_HEAT 1005.0 // in J/kg·°C
#define COMPRESSOR_POWER 2000 // Example value in kW
#define HEATER_POWER 1000.0 // Example value in kW
#define AMBIENT_TEMPERATURE 25.0 /* Ambient temperature in degrees Celsius */

#define PRESSURE_LOW 320   /*Low pressure in kpa */
#define PRESSURE_HIGH 1200 /*High pressure in kpa */
#define MAX_DATA_ROWS 1000


typedef struct {
    double temperature;
    double pressure;
    double enthalpy;
    double entropy;
} RefrigerantProperties;

// Global variables for refrigerant data
static RefrigerantProperties* refrigerantDataLiq = NULL;
static RefrigerantProperties* refrigerantDataVap = NULL;
static RefrigerantProperties* refrigerantDataSup = NULL;

extern int numRowsLiq;
extern int numRowsVap;
extern int numRowsSup;


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

/* Control functions */
float random_float(float min, float max);
float read_ambient_temperature();
float read_surrounding_temperature();
int read_humidity(HumiditySensor* humidity_sensor);
float random_float(float min, float max);

/* Prototypes */
float read_temperature(); /* Reads the temperature from the temperature sensor */
bool read_door_sensor(DoorSensor* door_sensor); /* Reads the state of the door sensor (open/closed) */
float calculate_cop(float h_1, float h_2, float h_3, float h_4, float mass_flow_rate);


//RefrigerantProperties* readLiquidDataset(const char* filename, int* numRows);
//RefrigerantProperties* readVaporDataset(const char* filename, int* numRows);
//RefrigerantProperties* readSuperheatedDataset(const char* filename, int* numRows);


double getEnthalpyFromPressure(double pressure, RefrigerantProperties *data, int dataSize);
int initialize_refrigerant_data();
//float getPressureFromDataset(float temperature);
float getPressureFromDataset(const char* datasetFile, float temperature);

RefrigerantProperties* readData(const char* filename, int* numRows);
RefrigerantProperties* readLiquidDataset(int* numRows);
RefrigerantProperties* readVaporDataset(int* numRows);
RefrigerantProperties* readSuperheatedDataset(int* numRows);


// Function prototypes for dynamic enthalpy calculations

float calculate_enthalpy_evaporator(float pressure, float ambient_temperature);
float calculate_enthalpy_compressor_outlet(float pressure, float ambient_temperature);
float calculate_enthalpy_condenser(float pressure, float surrounding_temperature);
float calculate_enthalpy_expansion_valve(float pressure, float surrounding_temperature);

float calculate_enthalpy(float temperature, float ambient_temperature, float surrounding_temperature, float pressure);


/* Simulate effects */
void simulate_compressor_effect(float* temperature, float ambient_temperature, float* energy_consumed);
void simulate_condenser_effect(float* temperature, float surrounding_temperature);
void simulate_evaporator_effect(float* temperature, float surrounding_temperature);
void simulate_expansion_valve_effect(ExpansionValve* expansion_valve);
void simulate_solenoid_valve_effect(SolenoidValve* solenoid_valve);
void simulate_hot_gas_bypass_effect(HotGasBypass* hot_gas_bypass, TemperatureSensor* temp_sensor);
void simulate_heater_effect(TemperatureSensor* temp_sensor, float* defrost_energy_consumed);
void simulate_fan_effect(HumiditySensor* humidity_sensor);
void adjust_for_door_state(DoorSensor* door_sensor, float* temperature_adjustment);

void initialize_sensors(TemperatureSensor* temp_sensor, DoorSensor* door_sensor, HumiditySensor* humidity_sensor);
void initialize_components(Compressor* compressor, Condenser* condenser, Evaporator* evaporator, ExpansionValve* expansion_valve, SolenoidValve* solenoid_valve, HotGasBypass* hot_gas_bypass);

void update_sensors(TemperatureSensor* temp_sensor, DoorSensor* door_sensor, HumiditySensor* humidity_sensor);
void control_system(TemperatureSensor* temp_sensor, DoorSensor* door_sensor, HumiditySensor* humidity_sensor, Compressor* compressor, Condenser* condenser, Evaporator* evaporator, ExpansionValve* expansion_valve, SolenoidValve* solenoid_valve, HotGasBypass* hot_gas_bypass, float* energy_consumed);
void simulate_system(float external_temp);

#endif

