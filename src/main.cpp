#include <Arduino.h>
#include <altimeter.h>

bool apogee_reached = false; // Flag to indicate if apogee has been reached
float previous_altitude = 0.0; // Variable to store the previous altitude reading
int consecutive_drops = 0; // Counter for consecutive drops in altitude
const int drop_threshold = 3; // Number of consecutive drops to trigger apogee detection

bool flight_finished = false; // Flag to indicate if the flight is finished
unsigned long landed_timer = 0; // Timer for landing detection
const float landing_altitude_threshold =  3.0; // meters
const int landing_time_threshold = 5000; // 5 seconds to consider as landed

void setup() {
  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect
  
  Serial.println("Initializing flight software...");

  if (!init_altimeter()) {
    Serial.println("Failed to initialize altimeter.");
    while (1); // Stop here if initialization fails
  }

  Serial.println("Altimeter initialized successfully.");
}

void loop() {
  float current_altitude = sample_altimeter();
  if (current_altitude != -1) {
    Serial.print("Current Altitude: ");
    Serial.print(current_altitude);
    Serial.println(" meters");

    // Apogee detection logic
    if (!apogee_reached) {
      if (current_altitude < previous_altitude) {
        consecutive_drops++;
      } else {
        consecutive_drops = 0; // Reset counter if it's not a drop
      }

      if (consecutive_drops >= drop_threshold) {
        apogee_reached = true; // Set the flag to indicate apogee has been reached
        Serial.println("Apogee reached!");
        Serial.print("Apogee Altitude: ");
        Serial.print(current_altitude);
        Serial.println(" meters");
        // Add parachute deployment logic
      }
    }

    // Landing detection logic
    if (!flight_finished) {
      if (current_altitude < landing_altitude_threshold) {
        if (landed_timer == 0) {
          landed_timer = millis(); // Start the timer
        } else if (millis() - landed_timer >= landing_time_threshold) {
          flight_finished = true; // Set the flag to indicate flight is finished
          Serial.println("Flight Completed!");
          // Add logic for post-flight actions (e.g., data logging, recovery, etc.)
        }
      } else {
        landed_timer = 0; // Reset timer if altitude is above threshold
      }
    }

    // Update previous_altitude with the current reading
    previous_altitude = current_altitude;

  } else {
    Serial.println("Failed to read altitude.");
  }

  delay(100); // read every 100ms
}

