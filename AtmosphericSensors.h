// AtmosphericSensors.h

// setup the sensors
void setupAtmosphereSensors();

// check sensor measurements and if they are outside previous measurement bounds
bool checkAtmosphereSensorBounds(float newValue, float prevValue, float maxDiff);

// sensor loop code
void updateAtmosphereSensors();
