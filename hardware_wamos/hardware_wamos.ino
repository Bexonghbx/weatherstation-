
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
// IMPORT ALL REQUIRED LIBRARIES

#include <math.h>
   
//**********ENTER IP ADDRESS OF SERVER******************//

#define HOST_IP     "172.16.193.156"       // REPLACE WITH IP ADDRESS OF SERVER ( IP ADDRESS OF COMPUTER THE BACKEND IS RUNNING ON) 
#define HOST_PORT   "8080"            // REPLACE WITH SERVER PORT (BACKEND FLASK API PORT)
#define route       "api/update"      // LEAVE UNCHANGED 
#define idNumber    "620148117"       // REPLACE WITH YOUR ID NUMBER 

// WIFI CREDENTIALS
#define SSID        "MonaConnect"       // "REPLACE WITH YOUR WIFI's SSID"   
#define password    ""                  // "REPLACE WITH YOUR WiFi's PASSWORD" 
//#define SSID        "UNTC-Connect"      // "REPLACE WITH YOUR WIFI's SSID"   
//#define password    "risenlord^19"      // "REPLACE WITH YOUR WiFi's PASSWORD" 
//#define SSID        "DESKTOP-PJ8NO24 8386"      // "REPLACE WITH YOUR WIFI's SSID"   
//#define password    "7(3P4i45"          // "REPLACE WITH YOUR WiFi's PASSWORD" 

#define stay        100
 
//**********PIN DEFINITIONS******************//

 
#define espRX         10
#define espTX         11
#define espTimeout_ms 300
#define trig          5
#define echo          4
 
 
/* Declare your functions below */
void espSend(char command[]);
void espUpdate(char mssg[]);
void espInit(void);
void wtLevel(void);
long wtHeight(void);
long wtReserv(void);
 

SoftwareSerial esp(espRX, espTX); 
 
long tme, dst, dH = 77.763, eH = 94.5, fR = 30.75, wH, wR;

void setup(){

  Serial.begin(115200); 
  // Configure GPIO pins here
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);


  espInit();  
 
}

void loop(){ 

  wtLevel();

  wH = wtHeight();

  wR = wtReserv();

  Serial.print("Distance ");
  Serial.print(dst);
  Serial.print("in, Water Height ");
  Serial.print(wH);
  Serial.print("in, Reserves ");
  Serial.print(wR);
  Serial.println(" gal");

   
  // send updates with schema ‘{"id": "student_id", "type": "ultrasonic", "radar": 0, "waterheight": 0, "reserve": 0, "percentage": 0}’
  StaticJsonDocument<1000> doc; 

  char message[800]   = {0};

  doc["id"]           = "620148117"; 
  doc["type"]         = "ultrasonic";
  doc["radar"]        = dst;
  doc["waterheight"]  = wH;
  doc["reserve"]      = wR;
  doc["percentage"]   = wR/10;
  
  serializeJson(doc, message);  
  
  espUpdate(message);    

  delay(000);  
}

 
void espSend(char command[] ){   
    esp.print(command); // send the read character to the esp    
    while(esp.available()){ Serial.println(esp.readString());}    
}


void espUpdate(char mssg[]){ 
    char espCommandString[50] = {0};
    char post[1000]            = {0};

    snprintf(espCommandString, sizeof(espCommandString),"AT+CIPSTART=\"TCP\",\"%s\",%s\r\n",HOST_IP,HOST_PORT); 
    espSend(espCommandString);    //starts the connection to the server
    delay(stay);

    // GET REQUEST 
    // snprintf(post,sizeof(post),"GET /%s HTTP/1.1\r\nHost: %s\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %d\r\n\r\n%s\r\n\r\n",route,HOST_IP,strlen(mssg),mssg);

    // POST REQUEST
    snprintf(post,sizeof(post),"POST /%s HTTP/1.1\r\nHost: %s\r\nContent-Type: application/json\r\nContent-Length: %d\r\n\r\n%s\r\n\r\n",route,HOST_IP,strlen(mssg),mssg);
  
    snprintf(espCommandString, sizeof(espCommandString),"AT+CIPSEND=%d\r\n", strlen(post));
    espSend(espCommandString);    //sends post length
    delay(stay);
    Serial.println(post);
    espSend(post);                //sends POST request with the parameters 
    delay(stay);
    espSend("AT+CIPCLOSE\r\n");   //closes server connection
   }

void espInit(){
    char connection[100] = {0};
    esp.begin(115200); 
    Serial.println("Initiallizing");
    esp.println("AT"); 
    delay(1000);
    esp.println("AT+CWMODE=1");
    delay(1000);
    while(esp.available()){ Serial.println(esp.readString());} 

    snprintf(connection, sizeof(connection),"AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,password);
    esp.print(connection);

    delay(3000);  //gives ESP some time to get IP

    if(esp.available()){   Serial.print(esp.readString());}
    
    Serial.println("\nFinish Initializing");    
   
}

//***** Design and implement all util functions below ******
void wtLevel(){
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  tme = pulseIn(echo, HIGH);
  dst = tme/58.2;
}

long wtHeight(){
  return max(0, 2*eH - (dst+dH));
}

long wtReserv(){
  return wH*3.14*fR*fR;
}