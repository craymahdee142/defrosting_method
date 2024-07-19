#include "main.h"




float calculate_enthalpy(float temperature, float ambient_temperature, float surrounding_temperature, float pressure) {
    float enthalpy = 0.0;

    // Determine which refrigerant data to use based on pressure
    if (pressure <= PRESSURE_LOW ) {
        printf("Using liquid refrigerant data for pressure: %.2f\n", pressure);
        enthalpy = getEnthalpyFromPressure(pressure, refrigerantDataLiq, numRowsLiq);
    } else if (pressure <= PRESSURE_HIGH) {
        printf("Using vapor refrigerant data for pressure: %.2f\n", pressure);
        enthalpy = getEnthalpyFromPressure(pressure, refrigerantDataVap, numRowsVap);
    } else {
        printf("Using superheated refrigerant data for pressure: %.2f\n", pressure);
        enthalpy = getEnthalpyFromPressure(pressure, refrigerantDataSup, numRowsSup);
    }

    if (enthalpy < 0) {
        fprintf(stderr, "Error: Invalid enthalpy calculated\n");
    } else {
        // Add any additional adjustments based on temperature and ambient/surrounding conditions
        enthalpy += temperature * 0.01; // Example adjustment
        enthalpy += ambient_temperature * 0.005;
        enthalpy += surrounding_temperature * 0.003;
    }

    printf("Calculated enthalpy: %.2f\n", enthalpy);
    return enthalpy;
}




// Function to calculate enthalpy at evaporator outlet (h_1)
float calculate_enthalpy_evaporator(float pressure, float ambient_temperature) {
    return getEnthalpyFromPressure(pressure, refrigerantDataLiq, numRowsLiq);
}

// Function to calculate enthalpy at compressor outlet (h_2)
float calculate_enthalpy_compressor_outlet(float pressure, float ambient_temperature) {
    return getEnthalpyFromPressure(pressure, refrigerantDataVap, numRowsVap);
}

// Function to calculate enthalpy at condenser inlet (h_3)
float calculate_enthalpy_condenser(float pressure, float surrounding_temperature) {
    return getEnthalpyFromPressure(pressure, refrigerantDataSup, numRowsSup);
}

// Function to calculate enthalpy at expansion valve inlet (h_4)
float calculate_enthalpy_expansion_valve(float pressure, float surrounding_temperature) {
    return getEnthalpyFromPressure(pressure, refrigerantDataLiq, numRowsLiq);
}



double getEnthalpyFromPressure(double pressure, RefrigerantProperties *data, int dataSize) {
    if (data == NULL || dataSize <= 0) {
        fprintf(stderr, "Error: Invalid data or data size\n");
        return -1.0; // Indicate an error
    }

    // Print the data size and first few entries for debugging
    printf("Data Size: %d\n", dataSize);
    for (int i = 0; i < dataSize && i < 5; ++i) {
        printf("Data[%d]: P: %.2f, T: %.2f, H: %.2f, S: %.2f\n",
               i, data[i].pressure, data[i].temperature, data[i].enthalpy, data[i].entropy);
    }

    // Check if pressure is out of range
    if (pressure < data[0].pressure || pressure > data[dataSize - 1].pressure) {
        fprintf(stderr, "Error: Pressure out of range: %.2f\n", pressure);
        return -1.0;
    }

    for (int i = 0; i < dataSize - 1; i++) {
        if (pressure >= data[i].pressure && pressure <= data[i + 1].pressure) {
            // Linear interpolation
            double p1 = data[i].pressure;
            double p2 = data[i + 1].pressure;
            double h1 = data[i].enthalpy;
            double h2 = data[i + 1].enthalpy;

            // Print debug information
            printf("Interpolating between data[%d] (P: %.2f, H: %.2f) and data[%d] (P: %.2f, H: %.2f) for pressure: %.2f\n",
                   i, p1, h1, i + 1, p2, h2, pressure);

            return h1 + ((h2 - h1) / (p2 - p1)) * (pressure - p1);
        }
    }

    // If pressure is outside the data range, return an error value
    fprintf(stderr, "Error: Pressure interpolation failed for pressure: %.2f\n", pressure);
    return -1.0;
}



// Function to get pressure from a dataset based on temperature
float getPressureFromDataset(const char* datasetFile, float temperature) {
    static RefrigerantProperties* data_liq = NULL;
    static RefrigerantProperties* data_vap = NULL;
    static RefrigerantProperties* data_sup = NULL;
    static int size_liq = 0;
    static int size_vap = 0;
    static int size_sup = 0;

    RefrigerantProperties* data = NULL;
    int size = 0;

    if (strcmp(datasetFile, "sat_liq.txt") == 0) {
        data = data_liq;
        size = size_liq;
    } else if (strcmp(datasetFile, "sat_vap.txt") == 0) {
        data = data_vap;
        size = size_vap;
    } else if (strcmp(datasetFile, "sup.txt") == 0) {
        data = data_sup;
        size = size_sup;
    } else {
        fprintf(stderr, "Error: Unknown dataset file %s\n", datasetFile);
        exit(EXIT_FAILURE);
    }

    if (data == NULL) {
        if (strcmp(datasetFile, "sat_liq.txt") == 0) {
            data_liq = readData(datasetFile, &size_liq);
            data = data_liq;
            size = size_liq;
        } else if (strcmp(datasetFile, "sat_vap.txt") == 0) {
            data_vap = readData(datasetFile, &size_vap);
            data = data_vap;
            size = size_vap;
        } else if (strcmp(datasetFile, "sup.txt") == 0) {
            data_sup = readData(datasetFile, &size_sup);
            data = data_sup;
            size = size_sup;
        }
    }

    // Find the closest temperature in the dataset
    int closest_index = 0;
    float closest_diff = fabs(data[0].temperature - temperature);
    for (int i = 1; i < size; i++) {
        float diff = fabs(data[i].temperature - temperature);
        if (diff < closest_diff) {
            closest_diff = diff;
            closest_index = i;
        }
    }

    // Return the pressure corresponding to the closest temperature
    return data[closest_index].pressure;
}





float calculate_cop(float h_1, float h_2, float h_3, float h_4, float mass_flow_rate) {
    // Calculate the cooling capacity (Q_evap) and work done by the compressor (W_comp)
    float Q_evap = mass_flow_rate * (h_1 - h_4);  // Cooling capacity in the evaporator
    float W_comp = mass_flow_rate * (h_2 - h_1);

    if (W_comp == 0) {
        return 0.0;
    }

    float cop = Q_evap / W_comp;
    return cop;
}

