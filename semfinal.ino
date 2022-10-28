#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>

 
#include <WebServer.h>
#include <ESPmDNS.h>
 
#include <SPI.h>
#include <Ethernet.h>
#include <WiFiMulti.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include "Fabrice.h"

#define LED_PIN   15 // ESP32 pin GIOP15 connected to LED's pin
#define SOUND_SPEED 0.034
#define DISTANCE_THRESHOLD 50 // centimeters
#define COUNT_LOW 0
#define COUNT_HIGH 8888
#define TIMER_WIDTH 16

// our tank height is 200cm

// The below are variables, which can be changed

const int trigPin = 5;
const int echoPin = 18;
int Status = 1;
long duration;
float distanceCm;
float tank_level;

const int Motorpin = 33;
 

int Motor_state = LOW;
int Manual_state = LOW;
 
WiFiClient client;
 

WebServer server(80);

WiFiMulti wifiMulti;

char SServer[] ="172.16.4.130"; // local IP address

IPAddress ip(172,16,4,130); 
//WiFiClient client; 



// setting the LCD address to 0x3F for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F,16,2);  
 

const char* ssid = "Fabrice";
const char* password = "12345678900";

//int SERVO_PIN = 13;
//Servo servo;


//WebServer server(80);
const char* ssidAP="ESP32-Access-Point";
const char* passwordAP="123456789";
 


void setup() {
  Serial.begin (115200);       // initialize serial port
  pinMode(trigPin, OUTPUT); // set ESP32 pin to output mode
  pinMode(echoPin, INPUT);  // set ESP32 pin to input mode
  pinMode(LED_PIN, OUTPUT);  // set ESP32 pin to output mode
  pinMode(Motorpin, OUTPUT);  // set ESP32 pin to output mode


  lcd.init();  //initialize the LCD
  lcd.clear(); 
   
  //Print a message to the LCD
  lcd.backlight();
  //lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Tracking system");
  lcd.setCursor(2,1);
  lcd.print("Water level");
  delay(50);
   
 
  WiFi.mode(WIFI_AP);
  wifiMulti.addAP("Fabrice", "12345678900");
 
  delay(100);
  WiFi.softAP(ssidAP,passwordAP);

  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
  server.on("/", Homepage);
  server.on("/Manualurl",Manual);
  server.on("/Motorurl",TurnMotor);
  server.on("/Valuesurl",CurrentValues);
  server.onNotFound(handleNotFound);

}

void loop() {
  server.handleClient();
  if(Manual_state == LOW){
    
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distanceCm = duration * SOUND_SPEED/2;
  tank_level= 100-distanceCm;
  
  server.handleClient();
if(tank_level <15){
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(Motorpin, HIGH); // turn on motor
  server.handleClient();
    while (tank_level <95){
      // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Setting the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  
  // Reading the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  
  // Calculating the distance
  
    distanceCm = duration * SOUND_SPEED/2;
    tank_level= 100-distanceCm;
      Serial.print("Tank Level: ");
      Serial.println(100-distanceCm);
      Sending_To_phpmyadmindatabase();
       server.handleClient();
    }
    
}else{
   digitalWrite(LED_PIN, LOW);
   digitalWrite(Motorpin, LOW);
}
  }
  //end if statement for the manual state
 else {
    server.handleClient(); // manage HTTP requests 
     
  }
  
   
  
      
  // Prints the distance in the Serial Monitor
  Serial.print("Tank Level: ");
  Serial.println(100-distanceCm);
   
  lcd.clear();
  lcd.print("Duration: ");
  lcd.print(duration);
  lcd.setCursor(0,1);
  lcd.print("Tank Level: ");
  lcd.print(100-distanceCm);  
  

  Sending_To_phpmyadmindatabase(); 
   
}
void Homepage(){
 
  server.send(200, "text/html", page);
  
}
void Manual(){
   Manual_state = !Manual_state;
 
  String str = "ON";    //modification below
  if(Manual_state == LOW) str = "OFF";
  server.send(200, "text/plain", str);
  Serial.println("Manual State");
}
void TurnMotor(){
  Motor_state = !Motor_state;
  if((Motor_state)&&(Manual_state == HIGH))digitalWrite(Motorpin, Motor_state);
   
  String str = "ON";    //very little returned
  if(Motor_state == LOW) str = "OFF";
  server.send(200, "text/plain", str);
  Serial.println("Motor");
}
void CurrentValues(){
  String str = String(tank_level);   //only this is sent
  server.send(200, "text/plain", str);
  Serial.println("Water Level");
}
void handleNotFound(){
  server.send(404,"text/plain", "Automatic mode: Server Not found");
}
 void Sending_To_phpmyadmindatabase() {  //CONNECTING WITH MYSQL
  delay(5000);
   if((wifiMulti.run() == WL_CONNECTED)) {
    
   Serial.print("[HTTP] begin...\n");
   if (client.connect(SServer, 80)) {
    Serial.println("connected");

   
   Serial.print("GET /tank_level/insert.php?tank_evel=");
    client.print("GET /tank_level/insert.php?tank_level=");     //YOUR URL
    Serial.println(tank_level);
    client.print(tank_level);
    
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host:172.16.4.130");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
 }else {
      Serial.printf("[HTTP} Unable to connect\n");
   }
   }
