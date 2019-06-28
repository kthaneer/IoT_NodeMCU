/*
  LAB C: ESP8266 AdaFruit IO 
  NodeMCU IoT Publish to Internet
*/

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// WiFi Access Point 
#define WLAN_SSID       "Saggezza"
#define WLAN_PASS       "sagg3zzan0w1r3"

// AdaFruit IO Connection Details 
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "thanneer"
#define AIO_KEY         "3d65849406e446dfa3a125ed8e9d2649"

// Setting up your WiFi Client and MQTT Client
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Publishing to the AdaFruit IO Feed. 
// MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish myFirstValue = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/NodeMCUTestFeed");

//We start at zero and when it gets to 10 we start over.
uint32_t xVal=-1;

//MQTT Connection 
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
 
}

void loop() {

  // Ensure the connection to the MQTT server is alive 
  MQTT_connect();

  // Now we can publish stuff!
  Serial.print(F("\nSending myValue "));
  Serial.print((xVal-1));
  Serial.print("...");
  if (! myFirstValue.publish(getVal())) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  delay(1000);
}

// Function to connect and reconnect as necessary to the MQTT server.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }

  Serial.println("MQTT Connected!");
}

//  Use this increment myVal
uint8_t getVal(){

  if(xVal == 11){
      xVal = 0;
  }
  return xVal++;  
}
