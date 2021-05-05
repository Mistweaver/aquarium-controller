// TemperatureProbe.h

// setup the temperature probe
void setupTemperatureProbe();

// check sensor measurements and if they are outside previous measurement bounds
bool checkTemperatureBounds(float newValue, float prevValue, float maxDiff);

// get the temperature from the probe
void updateProbe();
