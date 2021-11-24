#include <dht.h> // dette er et library som brugs til at få data'erne fra vores temperature/luftfugtighedssensor

dht DHT; // opretter et objekt som udnytter dht library

// Definere vores pins som er i brug og til hvad
#define DHT11_PIN 7
#define LED_RED 13
#define LED_BLUE 12
#define LED_YELLOW 11

#define SOUND_BUZZER 10
#define SOUND_READ A0

#define BUTTON_BUZZ 8

// opretter højeste grænser som vores målinger må have
int templimt = 27;
int humiditylimt = 50;

int period = 2000;
unsigned long time_now = 0;

int Buzzperiod = 2000;
unsigned long time_nowBuzz = 0;

int buttonState = 0;  

void setup() {
  //Definere at LED'erne skal sende data ikke modtage
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);

  // Knappen og buzzeren
  pinMode(BUTTON_BUZZ, INPUT);
  pinMode(SOUND_BUZZER, OUTPUT);

  // Definere at vores SOUND  
  pinMode(SOUND_READ, INPUT);


  // Start serial monitor/plotter
  Serial.begin(9600);
}

void loop() {
    time_now = millis();
   
    // Kører voes temperature og luftfugtigheds sensor ude hvert 2000ms 
    checkTnH();

    // Kører alt kode i en zero delay loop
    while(millis() < time_now + period){
      checkSound();
      buttonState = digitalRead(BUTTON_BUZZ);
    }
   ;  
}

void checkTnH() {
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

int adc;
int dB, PdB; //the variable that will hold the value read from the microphone each time

void checkSound() {
  float Analog, lastAnalog;

  float lastdiff;

  adc = analogRead(SOUND_READ); //Read the ADC value from amplifer
  dB = (adc+83.2073) / 11.003; //Convert ADC value to dB using Regression values
  
  Analog = analogRead (SOUND_READ);

  double diff = abs(Analog - lastAnalog);
  
  if (diff >= 550) {
  Serial.print("dB-Level=");
  Serial.println(dB, DEC);
  digitalWrite(LED_YELLOW, HIGH);
  } else{
      digitalWrite(LED_YELLOW, LOW);
  }
  lastAnalog = Analog;
}
