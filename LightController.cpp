// LightController.cpp

#include "LightController.h"

#include <RBDdimmer.h>              // dimmer library for ac dimmer
#include <PubSubClient.h>

#define outputPin 13                    // pwm output
#define zerocross 5                     // for boards with CHANGEBLE input pins
#define light_topic "settings/lights"    // mqtt topic

dimmerLamp dimmer(outputPin, zerocross);  //initialase port for dimmer for ESP8266, ESP32, Arduino due boards

extern PubSubClient client;

/****** Lamp Variables ******/
volatile int i = 60;                 // Dimmer PWM value
const int LAMP_MAX = 240;           // Max lamp brightness
const int LAMP_MIN = 60;            // min lamp brightness
int outVal = 0;


void setupLights() {
    dimmer.begin(NORMAL_MODE, ON);    //dimmer initialisation: name.begin(MODE, STATE)
    outVal = map(i, LAMP_MAX, LAMP_MIN, 100, 0);
    Serial.print("Outval: ");
    Serial.println(outVal);
    client.publish(light_topic, String(outVal).c_str(), true);

}

void adjustLights() {
    /*int input = server.arg("level").toInt();
    if(input < LAMP_MAX && input > LAMP_MIN) {
        i = input;
        outVal = map(i, LAMP_MAX, LAMP_MIN,  100, 0); // analogRead(analog_pin), min_analog, max_analog, 100%, 0%);
        dimmer.setPower(outVal); // name.setPower(0%-100%)
    }*/
}

void getLightStatus() {
    outVal = map(i, LAMP_MIN, LAMP_MAX, 0, 100);
    client.publish(light_topic, String(outVal).c_str(), true);
}
