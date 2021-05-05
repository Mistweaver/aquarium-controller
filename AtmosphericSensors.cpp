// AtmosphericSensors.cpp

#include <DHTesp.h>  //DHT11 Library for ESP
#include <PubSubClient.h>


#define humidity_topic "sensor/humidity"
#define temperature_topic "sensor/temperature"

#define DHTpin 14    //D5 of NodeMCU is GPIO14

extern PubSubClient client;

DHTesp dht;
float humidity, temperature;

long lastAtmosUpdate = 0;
float diff = 1.0;

// setup the sensors
void setupAtmosphereSensors() {
    //  for DHT11 Connect DHT sensor to GPIO pin
    dht.setup(DHTpin, DHTesp::DHT11);

}

// check sensor measurements and if they are outside previous measurement bounds
bool checkAtmosphereSensorBounds(float newValue, float prevValue, float maxDiff) {
    return !isnan(newValue) && (newValue < prevValue - maxDiff || newValue > prevValue + maxDiff);
}


// sensor loop code
void updateAtmosphereSensors() {
    //Serial.println("Updating atmosphere sensors");
    
    long now = millis();
    // Serial.print("Now: ");
    // Serial.println(now);
    // Serial.print("Last Message: ");
    // Serial.println(lastMsg);
    if (now - lastAtmosUpdate > 1000) {
        lastAtmosUpdate = now;

        delay(dht.getMinimumSamplingPeriod());
        float newTemp = dht.getTemperature();
        float newHum = dht.getHumidity();
        
        //float dewPoint = dht.computeDewPoint(newTemp, newHum, false);
        //float heatIndex = dht.computeHeatIndex(newTemp, newHum, false);
        //float perception = dht.computePerception(newTemp, newHum, false);
        //float absoluteHumidity = dht.computeAbsoluteHumidity(newTemp, newHum, false);
        
        // Serial.print("Temperature:");
        // Serial.println(newTemp, 1);
        // Serial.print("Humidity:");
        // Serial.println(newHum, 1);
            
        if (checkAtmosphereSensorBounds(newTemp, temperature, diff)) {
            temperature = newTemp;
            // Serial.println("New temperature:");
            client.publish(temperature_topic, String(temperature).c_str(), true);
        }

        if (checkAtmosphereSensorBounds(newHum, humidity, diff)) {
            humidity = newHum;
            // Serial.println("New humidity:");
            client.publish(humidity_topic, String(humidity).c_str(), true);
        }
    }
}
