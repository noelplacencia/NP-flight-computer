#include "altimeter.h"
#include <Wire.h>              // I2C communication
#include <Adafruit_BMP3XX.h>   // BMP380 sensor library

const double seaLevelPressure = 1013.25; // Standard sea level pressure in hPa

Adafruit_BMP3XX bmp;       // Create an instance of the BMP3XX sensor

static float starting_altitude = 0.0; // keep track of the starting altitude (used in sample_altimeter)

bool init_altimeter() {
    if (!bmp.begin_I2C()) {
        Serial.print("Error: BMP3XX sensor not found");
        return false; // Sensor not found
    }

// Configure sensor settings
bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
bmp.setPressureOversampling(BMP3_OVERSAMPLING_8X);
bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
bmp.setOutputDataRate(BMP3_ODR_100_HZ);

// Take initial reading for calibration
if (!bmp.performReading()) {
    Serial.print("Error: Failed to perform initial reading");
    return false; // Reading failed
}

// Set the baseline altitude using the initial reading
starting_altitude = bmp.readAltitude(seaLevelPressure);
Serial.print("Initial Altitude: ");
Serial.print(starting_altitude);
Serial.println(" meters");

return true; 
} 

float sample_altimeter() {
    if (!bmp.performReading()) {
        Serial.print("Error: Failed to perform reading");
        return -1; // Reading failed
    }

    float current_altitude = bmp.readAltitude(seaLevelPressure);
    return current_altitude - starting_altitude; // Return altitude relative to the baseline
}

float get_absolute_altitude() {
    if (!bmp.performReading()) {
        Serial.print("Error: Failed to perform reading");
        return -1; // Reading failed
    }

    float absolute_altitude = bmp.readAltitude(seaLevelPressure);
    return absolute_altitude; // Return absolute altitude
}


