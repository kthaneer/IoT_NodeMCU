/*
  LAB D: ESP8266 DHT22 - Temperature and Humidity with AdaFruit IO 
  NodeMCU IoT Collect Temperature and Humidity and Publish to Internet
*/

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "DHT.h"     // including the library of DHT22 temperature and humidity sensor   

// WiFi Access Point 
#define WLAN_SSID       "Saggezza"
#define WLAN_PASS       "sagg3zzan0w1r3"

// AdaFruit IO Connection Details 
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "thanneer"
#define AIO_KEY         "3d65849406e446dfa3a125ed8e9d2649"

// Setting up the DHT Sensor 
#define DHTTYPE DHT22 
#define dht_dpin 5
DHT dht(dht_dpin, DHTTYPE); 

// Setting up your WiFi Client and MQTT Client
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Publishing to the AdaFruit IO Feed. 
// MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish myFirstValue = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/NodeMCUTestFeed");

//MQTT Connection 
void MQTT_connect();

void setup() {

  dht.begin();
  Serial.begin(115200);
  Serial.println("Humidity and temperature\n\n");
  delay(700);

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

    // Reading Humidity 
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();         
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);
    
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%, Temperature: "));
    Serial.print(t);
    Serial.print(F("°C/"));
    Serial.print(f);
    Serial.print(F("°F & Heat Index: "));
    Serial.print(hic);
    Serial.print(F("°C/"));
    Serial.print(hif);
    Serial.println(F("°F"));
  
    // Now we can publish stuff!
    if (! myFirstValue.publish(hif)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
    }
    
    // Wait a few seconds between measurements.
    delay(2000);  
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
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
