// NetworkConfig.h

#include <PubSubClient.h>
// call this function to setup wifi
void setupNetwork();

// setup the wifi connection
void connectWifi();

// setup the MQTT connection
void connectMQTT();

// check WIFI connection and reconnect if lost
void checkWifi();

// check MQTT connection and reconnect if lost
void checkMQTT();

// execute this every loop to check network connection
void networkLoop();


// subscribes to MQTT messages from broker
void callback(char* topic, byte* payload, unsigned int length);
