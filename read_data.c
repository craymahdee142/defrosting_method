#include "main.h"

int numRowsLiq = 0;
int numRowsVap = 0;
int numRowsSup = 0;

// Function to initialize refrigerant data
int initialize_refrigerant_data() {
    refrigerantDataLiq = readData("sat_liq.txt", &numRowsLiq);
    refrigerantDataVap = readData("sat_vap.txt", &numRowsVap);
    refrigerantDataSup = readData("sup.txt", &numRowsSup);

    // Check and print which files were successfully read
    if (refrigerantDataLiq == NULL) {
        printf("Failed to read sat_liq.txt\n");
    } else {
        printf("Successfully read sat_liq.txt\n");
    }

    if (refrigerantDataVap == NULL) {
        printf("Failed to read sat_vap.txt\n");
    } else {
        printf("Successfully read sat_vap.txt\n");
    }

    if (refrigerantDataSup == NULL) {
        printf("Failed to read sup.txt\n");
    } else {
        printf("Successfully read sup.txt\n");
    }

    // Return failure if any of the data reads failed
    if (refrigerantDataLiq == NULL || refrigerantDataVap == NULL || refrigerantDataSup == NULL) {
        printf("Failed to initialize refrigerant data\n");
        return 0;
    }

    return 1; // Return 1 on success, 0 on failure
}

RefrigerantProperties* readData(const char* filename, int* numRows) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    printf("File opened successfully: %s\n", filename);

    // Skip header line
    char header[256];
    if (fgets(header, sizeof(header), file) == NULL) {
        perror("Failed to read header line");
        fclose(file);
        return NULL;
    }
    printf("Header: %s", header);

    // Count the number of data rows
    *numRows = 0;
    double temp1, temp2, temp3, temp4;
    while (fscanf(file, "%lf %lf %lf %lf", &temp1, &temp2, &temp3, &temp4) == 4) {
        (*numRows)++;
    }

    if (feof(file)) {
        printf("End of file reached\n");
    } else if (ferror(file)) {
        perror("Error reading file");
    } else {
        printf("Failed to read number of rows: unknown error\n");
    }

    printf("Number of data rows: %d\n", *numRows);

    if (*numRows <= 0) {
        fprintf(stderr, "No data rows found\n");
        fclose(file);
        return NULL;
    }

    // Allocate memory for data
    RefrigerantProperties* data = (RefrigerantProperties*)malloc(sizeof(RefrigerantProperties) * (*numRows));
    if (!data) {
        perror("Failed to allocate memory");
        fclose(file);
        return NULL;
    }

    // Rewind file to read data again
    rewind(file);

    // Skip header line again
    if (fgets(header, sizeof(header), file) == NULL) {
        perror("Failed to read header line on rewind");
        free(data);
        fclose(file);
        return NULL;
    }

    // Read data from file
    for (int i = 0; i < *numRows; i++) {
        if (fscanf(file, "%lf %lf %lf %lf",
                   &data[i].pressure, &data[i].temperature,
                   &data[i].enthalpy, &data[i].entropy) != 4) {
            perror("Invalid data format");
            printf("Error reading line %d\n", i + 1);  // Debug line number
            free(data);
            fclose(file);
            return NULL;
        }
        // Debug print
        /*printf("Read data: pressure = %.2f kPa, temperature = %.2f°C, enthalpy = %.2f, entropy = %.2f\n",
               data[i].pressure, data[i].temperature, data[i].enthalpy, data[i].entropy);*/
    }

    fclose(file);
    return data;
}

