
// lab4_ex1.cpp

#include <Arduino.h>
#include <WiFi.h>

#include <PubSubClient.h>

#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

#include <Servo.h>

//----------------------------------------------
// GLOBAL VARIABLES and CONSTANTS
const int RED_PIN = 26;
const int GREEN_PIN = 27;
const int BLUE_PIN = 14;
const int YELLOW_PIN = 12;
const int BUZZER_PIN = 32;
const int SERVO_PIN = 5;
const int BUTTON_PIN = 25;
const int LIGHT_SENSOR_PIN = 33; // Analog pin for light sensor

Servo myServo = Servo();

hd44780_I2Cexp lcd;
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

char ssid[] = "Wokwi-GUEST";
char password[] = "";

// MQTT Broker settings
const char* mqtt_broker = "mqtt.iotserver.uz";  // Free public MQTT broker
const int mqtt_port = 1883;
const char* mqtt_username = "userTTPU";  // username given in the telegram group
const char* mqtt_password = "";  // password given in the telegram group

const char* mqtt_topic_pub = "ttpu/lab5/test/out";   // Topic to publish
const char* mqtt_topic_sub = "ttpu/lab5/test/in";    // Topic to subscribe

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

// Function to connect to WiFi
void connectWiFi();
// Callback function for received MQTT messages
void mqttCallback(char* topic, byte* payload, unsigned int length);
// Function to connect/reconnect to MQTT broker
void connectMQTT();

//----------------------------------------------
// FUNCTIONS

//----------------------------------------------
// SETUP FUNCTION
void setup(void) 
{
    Serial.begin(115200);
    delay(1000);

    // LCD setup
    int status = lcd.begin(LCD_COLS, LCD_ROWS);
    if (status) {
        Serial.println("LCD initialization failed!");
        hd44780::fatalError(status);
    }

    lcd.noBacklight();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Initializing...");
    delay(1000);
}


//----------------------------------------------
// LOOP FUNCTION
void loop(void) 
{
    // your code here
}


//----------------------------------------------
// Connect to WiFi
void connectWiFi() {
  Serial.println("\nConnecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

//----------------------------------------------
// Callback function for received MQTT messages
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);
  
  // Convert payload to String
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  Serial.print("Message content: ");
  Serial.println(message);
  Serial.println("---");
}

//----------------------------------------------
// Function to connect/reconnect to MQTT broker
void connectMQTT() {
  while (!mqtt_client.connected()) {
    Serial.println("Connecting to MQTT broker...");
    
    String client_id = "esp32-client-" + String(WiFi.macAddress());
    
    if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT broker!");
      
      // Subscribe to topic
      mqtt_client.subscribe(mqtt_topic_sub);
      Serial.print("Subscribed to topic: ");
      Serial.println(mqtt_topic_sub);
    } else {
      Serial.print("MQTT connection failed, rc=");
      Serial.println(mqtt_client.state());
      Serial.println("Retrying in 5 seconds...");
      delay(5000);
    }
  }
}