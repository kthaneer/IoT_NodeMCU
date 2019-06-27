/*
  LAB F: Auto Water  
  Final Working Product 
*/

// Serial Monitor Bit Rate Setup 
#define MONITOR_BIT_RATE 115200

// ESP8266EX Setup 
#define GPIO_000 00
#define GPIO_001 01
#define GPIO_002 02
#define GPIO_003 03
#define GPIO_004 04
#define GPIO_005 05
#define GPIO_006 06
#define GPIO_007 07
#define GPIO_008 08
#define GPIO_009 09
#define GPIO_010 10
#define GPIO_011 11
#define GPIO_012 12
#define GPIO_013 13
#define GPIO_014 14
#define GPIO_015 15
#define GPIO_016 16

#define ESP8266_PIN_00 GPIO_016
#define ESP8266_PIN_01 GPIO_005
#define ESP8266_PIN_02 GPIO_004
#define ESP8266_PIN_03 GPIO_000
#define ESP8266_PIN_04 GPIO_002
#define ESP8266_PIN_05 GPIO_014
#define ESP8266_PIN_06 GPIO_012
#define ESP8266_PIN_07 GPIO_013
#define ESP8266_PIN_08 GPIO_015

// Note: The setup function runs once when you press reset or power the board
void setup() {

  Serial.begin(MONITOR_BIT_RATE);
  Serial.println("Auto Water \n");
 
  // Initialize digital pin LED_BUILTIN as an output
  pinMode(ESP8266_PIN_05, OUTPUT);
  pinMode(ESP8266_PIN_07, OUTPUT);
}

bool ledState = HIGH;

// The loop function runs over and over again forever
void loop() {

    //Read the soil moisture 
    int voltage = analogRead(A0);

    Serial.print("ADC Value: ");
    Serial.println(voltage);

    //Turn on the motor if the soil moisture is greater than 460 
    if (voltage > 460) {
      Serial.println(F("Turning on the motor!"));
      ledState = HIGH;
    }
    else
    {
      Serial.println(F("Turning off the motor!"));
      ledState = LOW;
    }
    
    digitalWrite(ESP8266_PIN_05, ledState);
    digitalWrite(ESP8266_PIN_07, !ledState);
    
    delay(1000);
}
