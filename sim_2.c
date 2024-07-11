#include "main.h"


/* Function to simulate reading ambient temperature */
float read_ambient_temperature() {
    return random_float(15.0, 35.0); // Example range for ambient temperature
}

/* Function to simulate reading surrounding temperature */
float read_surrounding_temperature() {
    return random_float(10.0, 30.0); // Example range for surrounding temperature
}


/* Calculate enthalpy given temperature and specific heat */
float calculate_enthalpy(float temperature, float specific_heat) {
    return specific_heat * temperature;
}

/* Calculate COP using system performance */
float calculate_cop(float T_evap_in, float T_evap_out, float T_comp_in, float T_comp_out, float mass_flow_rate, float Cp_evap, float Cp_comp) {
    float h_evap_in = calculate_enthalpy(T_evap_in, Cp_evap);
    float h_evap_out = calculate_enthalpy(T_evap_out, Cp_evap);
    float h_comp_in = calculate_enthalpy(T_comp_in, Cp_comp);
    float h_comp_out = calculate_enthalpy(T_comp_out, Cp_comp);

    float Q_evap = mass_flow_rate * (h_evap_out - h_evap_in);
    float W_comp = mass_flow_rate * (h_comp_out - h_comp_in);

    return Q_evap / W_comp; // COP
}
