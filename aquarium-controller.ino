#include "NetworkConfig.h"
#include "AtmosphericSensors.h"
#include "TemperatureProbe.h"
#include "LightController.h"

void setup() {
    Serial.begin(115200);
    Serial.println("************************************");
    Serial.println("Starting Bryce's Aquarium Controller");
    Serial.println("************************************");

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
