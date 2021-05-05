// NetworkConfig.cpp

#include "NetworkConfig.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

/*#define mqtt_server "BROKER IP"
#define mqtt_user "MQTT USER"
#define mqtt_password "MQTT PASSWORD"

#define ssid "SSID"
#define password "NETWORK PASSWORD"*/

WiFiClient espClient;
PubSubClient client(espClient);

void setupNetwork() {
    connectWifi();
    connectMQTT();
}

void connectWifi() {
    delay(10);
    Serial.println('\n');
    Serial.print("Connecting to ");
    Serial.print(ssid); 
    WiFi.begin(ssid, password);             // Connect to the network

    int i = 0;
    while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
        delay(1000);
        Serial.print('.');
        i++;
    }

    Serial.println('\n');
    Serial.println("Connection established!");  
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}

void connectMQTT() {
    Serial.print("Attempting MQTT connection...");
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);

    while (!client.connected()) {
        // Attempt to connect
        // If you do not want to use a username and password, change next line to
        // if (client.connect("ESP8266Client")) {
        if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
            Serial.println("MQTT connection established!");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void checkWifi() {
    
}

void checkMQTT() {
    if (!client.connected()) {
        connectMQTT();
    }


}

void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}

void networkLoop() {
    checkWifi();
    checkMQTT();
    client.loop();
}
