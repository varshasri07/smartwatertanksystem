#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiClient.h>;
#include <ThingSpeak.h>;

#define BLYNK_PRINT Serial
 #define SENSOR D5
  float bill=0.0;
char auth[] = "zI96QujE3jPG027CG5WYC6-MVMaNIljE";
char ssid[] = "realme";
char pass[] = "123456789";
WiFiClient client;

unsigned long myChannelNumber = 1760805; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "1KBE4Y1PRP2YB7A2"; //Your Write API Key

//WidgetLCD lcd1(V1);
WidgetLED led1(V1);
BlynkTimer timer;
void resetvalues()
{
  bill=0.0;
}


int trig = D1;
int echo = D2;
int relay = D3;
int durasi, level;
int i=0;



int pinValue;
 long currentMillis = 0;
 long previousMillis = 0;
 int interval = 1000;
 //float bill=0.0;
 boolean ledState = LOW;
 float calibrationFactor = 4.5;
 volatile byte   pulseCount;
 byte pulse1Sec = 0;
 float flowRate;
 unsigned int flowMilliLitres;
 unsigned long totalMilliLitres;
 void IRAM_ATTR pulseCounter()
 {
 pulseCount++;
 }


void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(D0, OUTPUT);
  
  digitalWrite(relay, LOW);
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  WiFi.begin(ssid, pass);
  ThingSpeak.begin(client);
   pinMode(SENSOR, INPUT_PULLUP);
   
  
 pulseCount = 0;
 flowRate = 0.0;
 flowMilliLitres = 0;
 totalMilliLitres = 0;
 previousMillis = 0;


 attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
 
}
BLYNK_WRITE(V6)
{
   pinValue=param.asInt();
  Serial.println(pinValue);

}
  
void loop() {
 // lcd1.clear();
 // lcd1.print(0, 0, "level: ");
  digitalWrite(trig, LOW);
  delayMicroseconds(8);

  digitalWrite(trig, HIGH);
  delayMicroseconds(8);

  digitalWrite(trig, LOW);
  durasi = pulseIn(echo, HIGH); //
  level = (durasi / 2) / 29.1;  // 
  Serial.print(level);
  Serial.println("Cm");
  //lcd1.print(8, 0, level);
 // lcd1.print(12, 0, "Cm");
  if (level <= 8&&level>6   ){
    
    delay(100);
    Blynk.notify ("high");       
  }

  else if (level <= 5){ 
    
    delay(100);
    
    Blynk.notify ("!!Very High!!");
    
    digitalWrite(relay, LOW);
  }
   else if (level >= 37){ 
    delay(100);
    
   
    Blynk.notify ("low!!!!");
    
    
  }

 // Blynk.virtualWrite(V5, level);
   currentMillis = millis();
 if (currentMillis - previousMillis > interval) {
 pulse1Sec = pulseCount;
 pulseCount = 0;
 flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) /calibrationFactor;
 previousMillis = millis();
 flowMilliLitres = (flowRate / 60) * 1000;
 totalMilliLitres += flowMilliLitres;
 bill = totalMilliLitres*(320.0/20000000);
 if(pinValue==1){
  bill=0.0;
 }
 Serial.print("Flow rate: ");
 Serial.print(int(flowRate));
 Serial.print("L/min");
 Serial.print("\t");
 Serial.print("Output Liquid Quantity: ");
 Serial.print(totalMilliLitres);
 Serial.print("mL / ");
 Serial.print(totalMilliLitres / 1000);
 Serial.println("L");
 Serial.println("\t bill");
   Serial.println(bill);
 Blynk.virtualWrite(V2, flowRate);
 Blynk.virtualWrite(V3, totalMilliLitres);
 Blynk.virtualWrite(V4, bill);
 Blynk.virtualWrite(V5,level);
 timer.setInterval(120000,resetvalues);
 int waterlevel=40-level;

 
  Blynk.run();
 

   // ThingSpeak.writeField(myChannelNumber, 1,bill, myWriteAPIKey); //Update in ThingSpeak
   ThingSpeak.setField(1,bill);
    ThingSpeak.writeFields(myChannelNumber,  myWriteAPIKey); 
     ThingSpeak.setField(2,waterlevel);
    ThingSpeak.writeFields(myChannelNumber,  myWriteAPIKey); 
 
  delay(200);
 }
 }
