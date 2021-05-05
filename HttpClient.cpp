
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>       // Include the WebServer library
#include <OneWire.h>                // Temperature probe
#include <DallasTemperature.h>      // Temperature probe
#include <RBDdimmer.h>              // dimmer library for ac dimmer
#include <ArduinoJson.h>            // JSON library for REST responses

#define outputPin 12                // pwm output
#define zerocross 5                 // for boards with CHANGEBLE input pins
#define ONE_WIRE_BUS 6              // Temperature Probe data wire pin
#define DHTPIN 1
#define DHTTYPE DHT11               // DHT 11

/****** WIFI Setting ******/
const char* ssid[] = "SSID_HERE";                 // "Bag End"
const char* password[] = "PASSWORD_HERE";         // "FrodoThePastaKing"
const char* http_username[] = "USERNAME_HERE";
const char* http_password[] = "PASSWORD_HERE";

/****** Intialize Wifi, Temperature and Dimmer Variables ******/
ESP8266WiFiMulti wifiMulti;               // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
ESP8266WebServer server(80);              // Create a webserver object that listens for HTTP request on port 80
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
dimmerLamp dimmer(outputPin, zerocross);  //initialase port for dimmer for ESP8266, ESP32, Arduino due boards
DHT dht(DHTPIN, DHTTYPE);
StaticJsonBuffer<200> sensorData;         // Pre-allocated memory pool for storing the JSONObject tree.

void handleRoot();                        // function prototypes for HTTP handlers
void handleNotFound();
void adjustLights();

/****** Lamp Variables ******/
volatile int i = 0;                 // Dimmer PWM value
const int LAMP_MAX = 240;           // Max lamp brightness
const int LAMP_MIN = 60;            // min lamp brightness
int outVal = 0;


/****** Start the server ******/
void setupHttpClient(void){
  Serial.begin(115200);             // Start the Serial communication to send messages to the computer
  dimmer.begin(NORMAL_MODE, ON);    //dimmer initialisation: name.begin(MODE, STATE)
  sensors.begin();                  // Start temperature sensors
  dht.begin();
  
  delay(10);
  Serial.println('\n');

  wifiMulti.addAP(ssid, password);   // add Wi-Fi networks you want to connect to
  // wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
  // wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

  if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  server.on("/lights", HTTP_POST, adjustLights);
  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}




void loop(void){
  server.handleClient(); // Listen for HTTP requests from clients
}

void adjustLights() {
  if(!server.hasArg("level") || server.arg("level") == NULL) { // If the POST request doesn't have username and password data
    server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    return;
  }
  if(server.hasArg("level")) { // If both the username and the password are correct
    int input = server.arg("level").toInt();
    if(input < LAMP_MAX && input > LAMP_MIN) {
      i = input;
      outVal = map(i, LAMP_MAX, LAMP_MIN,  100, 0); // analogRead(analog_pin), min_analog, max_analog, 100%, 0%);
      dimmer.setPower(outVal); // name.setPower(0%-100%)
      server.send(200, "text/html", "<p>Lights set to " + String(i) + " : " + String(outVal) +  "%</p>");
    } else {
      server.send(400, "text/plain", "400: Invalid Request");
    }
  }
}

/**
 * Data response package for light, temperature, humidity, etc
 *  {
 *    "waterTemp": number
 *    "atmosphereTemp": number
 *    "humidity": number
 *    "brightness": number
 *  }
 */

void handleRoot() {
  sensors.requestTemperatures();
  float waterTemp = sensors.getTempCByIndex(0);
  outVal = map(i, LAMP_MAX, LAMP_MIN,  100, 0);
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  sensorData["waterTemp"] = waterTemp;
  sensorData["temperature"] = t;
  sensorData["humidity"] = h;
  sensorData["brightness"] = outVal;

  char buffer[sensorData];
  serializeJson(sensorData, buffer);
  
  server.send(200, "application/json", buffer);
  // server.send(200, "text/plain", "#WT" + String(waterTemp) + "#L" + String(outVal) +  "%#AT" + String(t) + "#H" + String(h) + "#HI" + String(hic));   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
