#include "NetworkConfig.h"
#include "AtmosphericSensors.h"
#include "TemperatureProbe.h"
#include "LightController.h"

// globals
/**
 * 
 * Modes:
 * 0 = automatic
 * 1 = manual
 * 2 = evening display
 * 3 = thunderstorm
 * 
 **/

byte mode = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("Starting controller...");

    setupNetwork();

    setupAtmosphereSensors();

    setupTemperatureProbe();

    setupLights();
}

void loop() {
    networkLoop();

    updateAtmosphereSensors();

    updateProbe();
}
