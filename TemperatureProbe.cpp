#include "TemperatureProbe.h"

#include <OneWire.h>                
#include <DallasTemperature.h>      
#include <PubSubClient.h>

#define ONE_WIRE_BUS 12                          //D6 of NodeMCU is GPIO12
#define probe_topic "sensor/temperatureprobe"

extern PubSubClient client;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

long lastProbeUpdate = 0;
extern float diff;

float probeTemp;

void setupTemperatureProbe() {
    sensors.begin();         // start temperature probe
}

// check sensor measurements and if they are outside previous measurement bounds
bool checkTemperatureBounds(float newValue, float prevValue, float maxDiff) {
    return !isnan(newValue) && (newValue < prevValue - maxDiff || newValue > prevValue + maxDiff);
}


void updateProbe() {
    long now = millis();
    if (now - lastProbeUpdate > 1000) {
        lastProbeUpdate = now;
        sensors.requestTemperatures();
        float newTemp = sensors.getTempCByIndex(0);

        if(checkTemperatureBounds(newTemp, probeTemp, diff)) {
            probeTemp = newTemp;
            Serial.print("New probe temperature: ");
            Serial.println(probeTemp, 1);
            client.publish(probe_topic, String(probeTemp).c_str(), true);
        
        }
    }

}
