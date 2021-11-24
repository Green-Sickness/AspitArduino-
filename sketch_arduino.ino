#include <dht.h> // dette er et library som brugs til at få data'erne fra vores temperature/luftfugtighedssensor

dht DHT; // opretter et objekt som udnytter dht library

// Definere vores pins som er i brug og til hvad
#define DHT11_PIN 7
#define LED_RED 13
#define LED_BLUE 12
#define SOUND A0

// opretter højeste grænser som vores målinger må have
int templimt = 27;
int humiditylimt = 50;

void setup() {
  //Definere at LED'erne skal sende data ikke modtage
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  // Start serial monitor/plotter
  Serial.begin(9600);
}

void loop() {

  
  // chk er en variable som modtager data fra vores temperature/luftfugtighedssensor
  int chk = DHT.read11(DHT11_PIN);

  // Hvis chk er værdien 0 så udskriver den de følgende data'er i "if statement'et"
  if (chk == 0){
    // prints our Temp every 2 secs
    Serial.print("\nTemperature = ");
    Serial.println(DHT.temperature);
    checklimts(0);

    Serial.print("\n");
    // Prints our humidity every 2 secs
    Serial.print("Humidity = ");
    Serial.println(DHT.humidity);
    checklimts(1);

    Serial.print("\n");
  }

  // Printer en "error message" hvis vores modul ikke kunne læse noget 
  else {
    Serial.println("Error reading DHT11-module Failed!");
    
  }

  // sets a delay to our loop 
  delay(2000);
}

void checklimts(int options){

  // Cheker hvis vores temperature er for høj og i det tilfælde melder det i serial monitor'en 
  if (DHT.temperature > templimt && options == 0){
     Serial.print("Temperature is too HIGH!!\n");

     // tænder vores Røde led pære hvis temperaturen overskrider grænsen 
     digitalWrite(LED_RED, HIGH);
  }
  else if (options == 0){
    // Slukker LED'en når temperaturen falder under grænsen igen
    digitalWrite(LED_RED, LOW);
  }

  
  // Cheker hvis vores luftfugtigheden er for høj og i det tilfælde melder det i serial monitor'en
  if (DHT.humidity > humiditylimt && options == 1){
     Serial.print("Humidity is too HIGH!!");

     // Tænder vores blå led pære hvis luftfugtigheden overskrider grænsen 
     digitalWrite(LED_BLUE, HIGH);
  }
  else if (options == 1){
      // Slukker LED'en når Luftfugtigheden falder under grænsen igen
     digitalWrite(LED_BLUE, LOW);
  }
}
