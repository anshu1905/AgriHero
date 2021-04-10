//AgriHero NodeMCU MQTT Code
//This code uses test.mosquitto.org as its mqtt server
//Be sure to comment out the dummy values given for parameters to be published via MQTT, 
//before uploadeing this code with sensors connected to the NodeMCU

#include <ESP8266WiFi.h> // Enables the ESP8266 to connect to the local network (via WiFi)
#include <PubSubClient.h> // Allows us to connect to, and publish to the MQTT broker

// WiFi
//************ENTER YOUR SSID AND PASSWORD BEFORE UPLOADING!*******************************
const char* ssid = "PrimroseB";
const char* wifi_password = "anirudhananya"; // Your personal network password

// MQTT
const char* mqtt_server = "test.mosquitto.org";  // IP of the MQTT broker
const char* humidity_topic = "agrihero/humidity";
const char* temperature_topic = "agrihero/temperature";
const char* rains="agrihero/rain";
const char* soil_moisture="agrihero/soilmoisture";
const char* gas_pres="agrihero/gaspressure";
const char* colo="agrihero/color";
const char* mqtt_username = ""; // MQTT username, leave blank if not specified
const char* mqtt_password = ""; // MQTT password, leave blank if not specified
const char* clientID = "client_farm1"; // MQTT client ID

// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;

// 1883 is the listener port for the Broker
PubSubClient client(mqtt_server, 1883, wifiClient); 

// Custom function to connet to the MQTT broker via WiFi
void connect_MQTT(){
  

  // Connect to MQTT Broker
  // client.connect returns a boolean value to let us know if the connection was successful.
  // If the connection is failing, make sure you are using the correct MQTT Username and Password (Setup Earlier in the Instructable)
  if (client.connect(clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }
}

//////////////////////////////////////////////////////////////////////////////////

//RGB sensor
#define S0 D5
#define S1 D6
#define S2 D7
#define S3 D8
#define sensorOut D3

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;
String redFrequencyStr;
String greenFrequencyStr;
String blueFrequencyStr;

//Temperature And Humidity Sensor 
#include "DHT.h"
#define DHTPIN D1
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

//Rain Sensor 
#define rainDigital D2

//Soil Moisture Sensor
#define SensorPin A0 

//Gas Sensor   
int Gas_digital D0;   

String col;

void setup() {
  Serial.begin(115200); 
  dht.begin();
  
  pinMode(rainDigital,INPUT);

  pinMode(SensorPin,INPUT);
  
  pinMode(Gas_digital, INPUT);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  pinMode(sensorOut, INPUT);
  
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Connect to the WiFi
  WiFi.begin(ssid, wifi_password);

  // Wait until the connection has been confirmed before continuing
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Debugging - Output the IP Address of the ESP8266
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  

}

void loop() {

  connect_MQTT();
  
  int humidity = dht.readHumidity();
  int temperature = dht.readTemperature();

  
  int rainDigitalVal = digitalRead(rainDigital);

  int soilsensorValue = analogRead(SensorPin);

  int gassensorDigital = digitalRead(Gas_digital);

  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  redFrequency = pulseIn(sensorOut, LOW);
  redFrequencyStr = String(redFrequency);
  delay(100);
  
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  greenFrequency = pulseIn(sensorOut, LOW);
  greenFrequencyStr = String(greenFrequency);
  delay(100);
 
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  blueFrequency = pulseIn(sensorOut, LOW);
  blueFrequencyStr = String(blueFrequency);
  delay(1000);

  //Temporary dummy values
humidity = 75;
temperature = 30;
rainDigitalVal = 23;
soilsensorValue = 15;
gassensorDigital = 56;
redFrequency=34;
redFrequencyStr = String(redFrequency);
greenFrequency=132;
greenFrequencyStr = String(greenFrequency);
blueFrequency=18;
blueFrequencyStr = String(blueFrequency);
  
  int value[3]={redFrequency,greenFrequency,blueFrequency};
  char color[3]={'R','G','B'};
  for (int i=1;i<3;i++)
  {
    if(value[i]<value[0])
    {
      value[0]=value[i];
      color[0]=color[i];
    }
  }
  String col="["+redFrequencyStr+","+greenFrequencyStr+","+blueFrequencyStr+"]";
//  return col;
  delay(1000);
  




  //For MQTT
  String hs="Hum: "+String((float)humidity)+" % ";
  String ts="Temp: "+String((float)temperature)+" C ";
  String rain="Rain: "+String((float)rainDigitalVal);
  String soil="Soil: "+String((float)soilsensorValue);
  String gas="Gas: "+String((float)gassensorDigital);
  String co="Color: "+col;

////////////////////////////////////////////////////////////////////////////////////////////////////

    // PUBLISH to the MQTT Broker (topic = Temperature, defined at the beginning)
  if (client.publish(temperature_topic, String(temperature).c_str())) {
    Serial.println("Temperature sent!");
  }
  // Again, client.publish will return a boolean value depending on whether it succeded or not.
  // If the message failed to send, we will try again, as the connection may have broken.
  else {
    Serial.println("Temperature failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(temperature_topic, String(temperature).c_str());
  }

///////////////////////////////////////////////////////////////////////////////////////////////////////

  // PUBLISH to the MQTT Broker (topic = Humidity, defined at the beginning)
  if (client.publish(humidity_topic, String(humidity).c_str())) {
    Serial.println("Humidity sent!");
  }
  // Again, client.publish will return a boolean value depending on whether it succeded or not.
  // If the message failed to send, we will try again, as the connection may have broken.
  else {
    Serial.println("Humidity failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(humidity_topic, String(humidity).c_str());
  }

///////////////////////////////////////////////////////////////////////////////////////////////////////


  // PUBLISH to the MQTT Broker (topic = Rain, defined at the beginning)
  if (client.publish(rains, String(rainDigitalVal).c_str())) {
    Serial.println("Rain Data sent!");
  }
  // Again, client.publish will return a boolean value depending on whether it succeded or not.
  // If the message failed to send, we will try again, as the connection may have broken.
  else {
    Serial.println("Rain Data failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(rains, String(rainDigitalVal).c_str());
  }

///////////////////////////////////////////////////////////////////////////////////////////////////////

  // PUBLISH to the MQTT Broker (topic = Soil Moisture, defined at the beginning)
  if (client.publish(soil_moisture, String(soilsensorValue).c_str())) {
    Serial.println("Soil Moisture sent!");
  }
  // Again, client.publish will return a boolean value depending on whether it succeded or not.
  // If the message failed to send, we will try again, as the connection may have broken.
  else {
    Serial.println("Soil Moisture failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(soil_moisture, String(soilsensorValue).c_str());
  }

///////////////////////////////////////////////////////////////////////////////////////////////////////

  // PUBLISH to the MQTT Broker (topic = Gas, defined at the beginning)
  if (client.publish(gas_pres, String(gassensorDigital).c_str())) {
    Serial.println("Gas Presence sent!");
  }
  // Again, client.publish will return a boolean value depending on whether it succeded or not.
  // If the message failed to send, we will try again, as the connection may have broken.
  else {
    Serial.println("Gas Presence failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(gas_pres, String(gassensorDigital).c_str());
  }

///////////////////////////////////////////////////////////////////////////////////////////////////////

  // PUBLISH to the MQTT Broker (topic = color, defined at the beginning)
  if (client.publish(colo, String(col).c_str())) {
    Serial.println(col);
    Serial.println("Rgb sent!");
  }
  // Again, client.publish will return a boolean value depending on whether it succeded or not.
  // If the message failed to send, we will try again, as the connection may have broken.
  else {
    Serial.println("Gas Presence failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(color, String(col).c_str());
  }

///////////////////////////////////////////////////////////////////////////////////////////////////////

  client.disconnect();  // disconnect from the MQTT broker
  delay(15000);       // print new values every 1.5 Minute
}
