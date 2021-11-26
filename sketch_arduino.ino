#include <dht.h> // dette er et library som brugs til at få data'erne fra vores temperature/luftfugtighedssensor

dht DHT; // opretter et objekt som udnytter dht library

// Definere vores pins som er i brug og til hvad
#define DHT11_PIN 7
#define LED_RED 13
#define LED_BLUE 12
#define LED_YELLOW 11

#define SOUND_BUZZER 10
#define SOUND_READ A0

// Denne define er til vores kanp på boardet
#define BUTTON_BUZZ 8

// opretter højeste grænser som vores målinger må have
int templimt = 30;
int humiditylimt = 55;

int humidity, temp;

int adc;
int dB;

int period = 2000;
unsigned long time_now = 250;

bool allowBuzz = true;

bool overValueTemp, overValueHum, overValueSound;

bool alerted = false;

int buttonState = 0;
int lastButtonState = 0;

void setup()
{
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
    link.begin(9600);
}

void loop()
{
    time_now = millis();

    // Kører voes temperature og luftfugtigheds sensor ude hvert 2000ms
    checkTnH();

    // Kører alt kode i en zero delay loop
    while (millis() < time_now + period)
    {
        alert(true);
        buttonState = digitalRead(BUTTON_BUZZ);

        if (buttonState == 1 && lastButtonState == 0)
        {
            // Denne if statement / else, er ligesom en off and on switch fordi hvis allowBuzz er True bliver den false og hvis den er false bliver den True
            if (allowBuzz)
            {
                allowBuzz = false;
            }
            else
            {
                allowBuzz = true;
            }
        }
        lastButtonState = buttonState;
        checkSound();
    }
};

void checkTnH()
{
    // chk er en variable som modtager data fra vores temperature/luftfugtighedssensor
    int chk = DHT.read11(DHT11_PIN);

    // Hvis chk er værdien 0 så udskriver den de følgende data'er i "if statement'et"
    if (chk == 0)
    {
        temp = DHT.temperature;
        // prints our Temp every 2 secs
        Serial.print("\nTemperature = ");
        Serial.println(DHT.temperature);
        checklimts(0);

        Serial.print("\n");
        // Prints our humidity every 2 secs
        humidity = DHT.humidity;
        Serial.print("Humidity = ");
        Serial.println(DHT.humidity);
        checklimts(1);

        Serial.print("\n");
    }

    // Printer en "error message" hvis vores modul ikke kunne læse noget
    else
    {
        Serial.println("Error reading DHT11-module Failed!");
    }
}

void checklimts(int options)
{
    // Cheker hvis vores temperature er for høj og i det tilfælde melder det i serial monitor'en
    if (DHT.temperature > templimt && options == 0)
    {
        Serial.print("Temperature is too HIGH!!\n");

        // tænder vores Røde led pære hvis temperaturen overskrider grænsen
        digitalWrite(LED_RED, HIGH);
        // Sætter vores bool til en false for at fortælle at den er over den tilldte værdi
        overValueTemp = true;
    }
    else if (options == 0)
    {
        // Slukker LED'en når temperaturen falder under grænsen igen
        digitalWrite(LED_RED, LOW);

        // Sætter vores bool til en false for at fortælle at den ikke er over den tilldte værdi
        overValueTemp = false;
    }

    // Cheker hvis vores luftfugtigheden er for høj og i det tilfælde melder det i serial monitor'en
    if (DHT.humidity > humiditylimt && options == 1)
    {
        Serial.print("Humidity is too HIGH!!");

        // Tænder vores blå led pære hvis luftfugtigheden overskrider grænsen
        digitalWrite(LED_BLUE, HIGH);
        // Sætter vores bool til en true for at fortælle at den er over den tilldte værdi
        overValueHum = true;
    }
    else if (options == 1)
    {
        // Slukker LED'en når Luftfugtigheden falder under grænsen igen
        digitalWrite(LED_BLUE, LOW);

        // Sætter vores bool til en false for at fortælle at den ikke er over den tilldte værdi
        overValueHum = false;
    }
}

bool flag = false;
int cont = 0;
void alert(bool shouldbuzz)
{
    // Her checker vi hvis den må og skulle buzz og derefter hvis nogle af tingene er over den tilldt værdi, Hvis værdien af nogle af dem sender vi et Buzz.
    if (allowBuzz && shouldbuzz && (overValueTemp || overValueHum || overValueSound))
    {
        tone(SOUND_BUZZER, 500, 500);

        tone(SOUND_BUZZER, 0, 3000);

        tone(SOUND_BUZZER, 500, 500);
    }
}

void checkSound()
{
    float Analog, lastAnalog;

    float lastdiff;

    adc = analogRead(SOUND_READ); //Read the ADC value from amplifer
    dB = (adc + 83.2073) / 11.003; //Convert ADC value to dB using Regression values

    Analog = analogRead(SOUND_READ);

    double diff = abs(Analog - lastAnalog);

    if (diff >= 605)
    {
        Serial.print("dB-Level=");
        Serial.println(dB, DEC);
        alert(true);
        overValueSound = true;
        digitalWrite(LED_YELLOW, HIGH);
    }
    else
    {
        overValueSound = false;
        digitalWrite(LED_YELLOW, LOW);
    }
    lastAnalog = Analog;
}
