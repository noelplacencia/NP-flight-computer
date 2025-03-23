#pragma once
#include <Arduino.h>

// Initializes the altimeter and sets the baseline altitude
// Returns true if initialization is successful, false otherwise
bool init_altimeter();

//Returns the current altitude in meters relative to the baseline
// Returns -1 if the reading fails
float sample_altimeter(); 

// Returns altitude above sea level in meters
// Returns -1 if the reading fails
float get_absolute_altitude(); 