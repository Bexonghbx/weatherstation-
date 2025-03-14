//##################################################################################################################
//##                                      ELET2415 DATA ACQUISITION SYSTEM CODE                                   ##
//##                                                                                                              ##
//##################################################################################################################

 

// IMPORT ALL REQUIRED LIBRARIES
#include <rom/rtc.h>
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "DHT.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP280.h"



//IMPORT IMAGES
//#include "uwiweather.h"


#ifndef _WIFI_H 
#include <WiFi.h>
#include <HTTPClient.h>
#endif

#ifndef STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#include <stdio.h>
#endif

#ifndef ARDUINO_H
#include <Arduino.h>
#endif 

#ifndef ARDUINOJSON_H
#include <ArduinoJson.h>
#endif
 


// DEFINE VARIABLES


#define TFT_DC    17
#define TFT_CS    5
#define TFT_RST   16
#define TFT_CLK   18
#define TFT_MOSI  23
#define TFT_MISO  19
#define btn1      26
#define btn2      25
#define btn3      33
#define SOILMST   34
#define SOILPWR   15
#define BMP_SCK   22
#define BMP_MISO  0
#define BMP_MOSI  21 
#define BMP_CS    4
#define DHTPIN    14
#define DHTTYPE DHT22


 


// MQTT CLIENT CONFIG  
static const char* pubtopic       = "620148117";                    // Add your ID number here
static const char* subtopic[]     = {"620148117_sub","/elet2415"};  // Array of Topics(Strings) to subscribe to
static const char* mqtt_server    = "yanacreations.com";                // Broker IP address or Domain name as a String 
static uint16_t mqtt_port         = 1883;

// WIFI CREDENTIALS
const char* ssid                  = "MonaConnect"; // Add your Wi-Fi ssid
const char* password              = ""; // Add your Wi-Fi password 
//const char* ssid                  = "UNTC-Connect"; // Add your Wi-Fi ssid
//const char* password              = "risenlord^19"; // Add your Wi-Fi password 
//const char* ssid                  = "DESKTOP-PJ8NO24 8386"; // Add your Wi-Fi ssid
//const char* password              = "7(3P4i45"; // Add your Wi-Fi password 



// TASK HANDLES 
TaskHandle_t xMQTT_Connect          = NULL; 
TaskHandle_t xNTPHandle             = NULL;  
TaskHandle_t xLOOPHandle            = NULL;  
TaskHandle_t xUpdateHandle          = NULL;
TaskHandle_t xButtonCheckeHandle    = NULL; 


// FUNCTION DECLARATION   
void checkHEAP(const char* Name);   // RETURN REMAINING HEAP SIZE FOR A TASK
void initMQTT(void);                // CONFIG AND INITIALIZE MQTT PROTOCOL
unsigned long getTimeStamp(void);   // GET 10 DIGIT TIMESTAMP FOR CURRENT TIME
void callback(char* topic, byte* payload, unsigned int length);
void initialize(void);
bool publish(const char *topic, const char *payload); // PUBLISH MQTT MESSAGE(PAYLOAD) TO A TOPIC
void vButtonCheck( void * pvParameters );
void vUpdate( void * pvParameters ); 

 

//############### IMPORT HEADER FILES ##################
#ifndef NTP_H
#include "NTP.h"
#endif

#ifndef MQTT_H
#include "mqtt.h"
#endif




/* Initialize class objects*/
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);


 
/* Functions */
void barRead(void);
void dhtRead(void);
void mstRead(void);
void tftUpdate(void);
double convert_Celsius_to_fahrenheit(double c);
double convert_fahrenheit_to_Celsius(double f);
double calcHeatIndex(double T, double R);
bool isNumber(double number);
void serialUpdate(void);


/* Varialbles */
double a_max = 0, a_min = 1000, heatindex, heatindexc, temp, tempf, humid, pressure, c1 = -42.379, c2= 2.04901523, c3 = 10.14333127, c4 = -0.22475541, c5 = -6.83783 * 0.001, c6 = -5.481717 * 0.01, c7 = 1.22874 * 0.001, c8 = 8.5282 * 0.0001, c9 = -1.99 * 0.000001;
uint8_t alti;
int moist;


 
void setup() {
    Serial.begin(115200);  // INIT SERIAL  
    bmp.begin(0x76); 
    dht.begin();
    tft.begin();
  
    
  // CONFIGURE THE ARDUINO PINS OF THE 7SEG AS OUTPUT
 
  /* Configure all others here */
  /*pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);*/
  pinMode(SOILPWR, OUTPUT);
  digitalWrite(SOILPWR, LOW);
  tft.fillScreen(ILI9341_WHITE);
  tft.drawCircle(120, 160, 20, ILI9341_GREEN);
  vTaskDelay(1000 / portTICK_PERIOD_MS);  
  tft.drawCircle(120, 160, 50, ILI9341_GREEN);
  vTaskDelay(1000 / portTICK_PERIOD_MS);  
  tft.drawCircle(120, 160, 100, ILI9341_GREEN);
  vTaskDelay(1000 / portTICK_PERIOD_MS);  
  //tft.drawRGBBitmap(0,0, uwiweather, 240, 320);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(1);
  tft.setCursor(15, 210);
  tft.print("Temperature (C)");
  tft.setCursor(15, 245);
  tft.print("Heat Index (F)");
  tft.setCursor(15, 280);
  tft.print("Pressure (kPa)");
  tft.setCursor(135, 210);
  tft.print("Humidity (%)");
  tft.setCursor(135, 245);
  tft.print("Moisture (%)");
  tft.setCursor(135, 280);
  tft.print("Alitude (m)");
  tft.fillRoundRect(15, 220, 48, 20, 4, ILI9341_BLACK);
  tft.fillRoundRect(15, 255, 48, 20, 4, ILI9341_BLACK);
  tft.fillRoundRect(15, 290, 48, 20, 4, ILI9341_BLACK);
  tft.fillRoundRect(135, 220, 48, 20, 4, ILI9341_BLACK);
  tft.fillRoundRect(135, 255, 48, 20, 4, ILI9341_BLACK);
  tft.fillRoundRect(135, 290, 48, 20, 4, ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE,ILI9341_BLACK);
  initialize();           // INIT WIFI, MQTT & NTP 
  //vButtonCheckFunction(); // UNCOMMENT IF USING BUTTONS THEN ADD LOGIC FOR INTERFACING WITH BUTTONS IN THE vButtonCheck FUNCTION
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}
  


void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelay(2000 / portTICK_PERIOD_MS);  
}



  
//####################################################################
//#                          UTIL FUNCTIONS                          #       
//####################################################################
void vButtonCheck( void * pvParameters )  {
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );     
      
    for( ;; ) {
        // Add code here to check if a button(S) is pressed
        // then execute appropriate function if a button is pressed  

        // 1. Implement button1  functionality
        if (!digitalRead(btn1)){

        }

        // 2. Implement button2  functionality
        else if (!digitalRead(btn2)){

        }

        // 3. Implement button3  functionality
        else if (!digitalRead(btn3)){

        }
       
        vTaskDelay(200 / portTICK_PERIOD_MS);  
    }
}

void vUpdate( void * pvParameters )  {
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );    
 
    for( ;; ) {
          // Task code goes here.   
          // PUBLISH to topic every second.         
          dhtRead();
          barRead();
          mstRead();
          tftUpdate();
          serialUpdate();


        if (isNumber(temp)&&((a_max-a_min)>=100)){

          JsonDocument doc; // Create JSon object
          char message[1100]  = {0};

          // Add key:value pairs to JSon object
          doc["id"]               = "620148117";
          doc["timestamp"]        = getTimeStamp();
          doc["humidity"]         = humid;
          doc["temperature"]      = temp;
          doc["temperatureFar"]   = tempf;
          doc["heatindex"]        = heatindex;
          doc["heatindexCel"]     = heatindexc;
          doc["airpressure"]      = pressure;
          doc["altitude"]         = alti;
          doc["soilmoisture"]     = moist;

          serializeJson(doc, message);  // Seralize / Covert JSon object to JSon string and store in char* array

          if(mqtt.connected() ){
            publish(pubtopic, message);
            Serial.println("");
            Serial.println(message);
          }

        }
            
        vTaskDelay(2000 / portTICK_PERIOD_MS);  
    }
}

unsigned long getTimeStamp(void) {
          // RETURNS 10 DIGIT TIMESTAMP REPRESENTING CURRENT TIME
          time_t now;         
          time(&now); // Retrieve time[Timestamp] from system and save to &now variable
          return now;
}

void callback(char* topic, byte* payload, unsigned int length) {
  // ############## MQTT CALLBACK  ######################################
  // RUNS WHENEVER A MESSAGE IS RECEIVED ON A TOPIC SUBSCRIBED TO
  
  Serial.printf("\nMessage received : ( topic: %s ) \n",topic ); 
  char *received = new char[length + 1] {0}; 
  
  for (int i = 0; i < length; i++) { 
    received[i] = (char)payload[i];    
  }

  // PRINT RECEIVED MESSAGE
  Serial.printf("Payload : %s \n",received);

 
  // CONVERT MESSAGE TO JSON
  StaticJsonDocument<1000> doc;
  DeserializationError error = deserializeJson(doc, received);  

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }


  // PROCESS MESSAGE

}

bool publish(const char *topic, const char *payload){   
     bool res = false;
     try{
        res = mqtt.publish(topic,payload);
        // Serial.printf("\nres : %d\n",res);
        if(!res){
          res = false;
          throw false;
        }
     }
     catch(...){
      Serial.printf("\nError (%d) >> Unable to publish message\n", res);
     }
  return res;
}

//***** Utility Functions ******

double convert_Celsius_to_fahrenheit(double c){    
    // CONVERTS INPUT FROM °C TO °F. RETURN RESULTS  
    return (c * (9.0/5) + 32);
}

double convert_fahrenheit_to_Celsius(double f){    
    // CONVERTS INPUT FROM °F TO °C. RETURN RESULT  
    return ( (5.0/9) * (f - 32)); 
}

double calcHeatIndex(double T, double R){
    // CALCULATE AND RETURN HEAT INDEX USING EQUATION FOUND AT https://byjus.com/heat-index-formula/#:~:text=The%20heat%20index%20formula%20is,an%20implied%20humidity%20of%2020%25
    return  c1 + (c2*T) + (c3*R) + (c4*T*R) + (c5*T*T) + (c6*R*R) + (c7*T*T*R) + (c8*T*R*R) + (c9*T*T*R*R);
}

bool isNumber(double number){       
        char item[20];
        snprintf(item, sizeof(item), "%f\n", number);
        if( isdigit(item[0]) )
          return true;
        return false; 
} 

void barRead(){
  pressure = bmp.readPressure();

  alti = bmp.readAltitude(29.92);
}

void dhtRead(){
  double c = dht.readTemperature() + bmp.readTemperature();

  temp = c/2.0;

  humid = dht.readHumidity();
  
  tempf = convert_Celsius_to_fahrenheit(temp);

  heatindex = calcHeatIndex(tempf, humid);

  heatindexc = convert_fahrenheit_to_Celsius(heatindex);
}

void mstRead(){
  digitalWrite(SOILPWR, HIGH);	// Turn the sensor ON
	delay(10);	// Allow power to settle
  double anaMoist = analogRead(SOILMST);
	digitalWrite(SOILPWR, LOW);		// Turn the sensor OFF
  a_max = max(a_max,anaMoist);
  a_min = min(a_min,anaMoist);
	moist = ((1+a_max- anaMoist)/ (1+a_max-a_min)*1.0) * 100.0;	// Read the analog value from sensor
  /*Serial.print("Moisture: ");
  Serial.println(anaMoist);
  Serial.print("Max: ");
  Serial.println(a_max);
  Serial.print("Min: ");
  Serial.println(a_min);*/
}

void tftUpdate(){
  tft.setCursor(19, 227);
  tft.print(temp);tft.print("  ");
  tft.setCursor(19, 262);
  tft.print(heatindex);tft.print("  ");
  tft.setCursor(19, 297);
  tft.print(pressure/1000);tft.print("  ");
  tft.setCursor(139, 227);
  tft.print(humid);tft.print("  ");
  tft.setCursor(139, 262);
  tft.print(moist);tft.print("  ");
  tft.setCursor(139, 297);
  tft.print(alti);tft.print("  ");
}
void serialUpdate(){
  Serial.print("Temperature (C): ");
  Serial.println(temp);
  Serial.print("Heat Index (F): ");
  Serial.println(heatindex);
  Serial.print("Pressure (kPa): ");
  Serial.println(pressure/1000);
  Serial.print("Humidity (%): ");
  Serial.println(moist);
  Serial.print("Moisture (%): ");
  Serial.println(humid);
  Serial.print("Alitude (m): ");
  Serial.println(alti);
  Serial.println("\n");
}