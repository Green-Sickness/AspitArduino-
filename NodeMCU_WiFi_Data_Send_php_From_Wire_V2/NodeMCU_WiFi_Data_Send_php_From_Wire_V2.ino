/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-http-request
 */


#include <ESP8266WiFi.h> // henter data grundlaget til af tilkoble til WiFi
#include <ESP8266HTTPClient.h>
#include<Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial link(D0, 3); // Rx, Tx

  char cString[20];
  byte chPos = 0;
  byte ch = 0;
  char dataStr[6];

int    HTTP_PORT   = 80;
String HTTP_METHOD = "POST";
char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME   = "/trigger";
String queryString = "?value1=26&value2=70";;

#define HOST "aspitcloud.dk"; // Enter host url with out "http://" / "/" and the start and end

const char* ssid= "ASPIT_GUEST"; // SSID'en til vores netværk
const char* password= ""; // adgangs koden til det netværk

String sendval, sendval2, sendval3, PostData;

int x;
void setup() {
  link.begin(9600); //setup software serial
  Serial.begin(9600); /* begin serial for debug */
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output


  Serial.begin(9600);// this is 
  Serial.println("Communication Started \n\n");
  

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); // starter Wifi med adressen og adgangskoden
  
  Serial.print("Connecting to");
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );
}

// the loop function runs over and over again forever
void loop() {
delay(10000);
  // kode til af hente data in fra Aurdino Uno
  //////////////////////////////////////////////

  
 while(link.available())
   {
    //read incoming char by char:
     ch = link.read();
     cString[chPos] = ch;
     chPos++;     
     
   Serial.println("Link found?");
   delay(10);   
   }
   cString[chPos] = 0; //terminate cString
   chPos = 0;
   
   Serial.print(cString);
  
  



  
  // kode til af sende data til en SQL Server
  //////////////////////////////////////////////
  
  HTTPClient http; 
  WiFiClient wifiClient;
  
  sendval = "8.00"; // vi er nød til konvertere Int's om til string
  sendval2 = "40.64";
  sendval3 = "7980.0"; 
  
  PostData = "Temp=" + sendval + "&Luft=" + sendval2 + "&Lyd=" + sendval3; // det her er det data vi sender op til vores php side

  bool isbegin = http.begin(wifiClient, "osca1877.aspitcloud.dk",80, "/post"); // WIFIClient, "URL":Port/Filsti efter grund navn that links to a php file that can take the data
  printf("is begin is ==" + isbegin); // does start
  
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  int httpCode = http.POST(PostData); //Send post request to PHP fole and store server response
  Serial.println("Values are, SendvalInt " + sendval);
  Serial.println(PostData);

  
  // if connection eatablished then do this
  if(httpCode == 200) { // 200 means correct
  Serial.println("Values uploaded succesfully"); 
  Serial.println(httpCode);
  String webpage = http.getString();
  Serial.println(webpage + "\n"); // payload fra hjemme side 
  }
  else 
  {
    Serial.println(httpCode); 
    Serial.println("Failed to upload Values. \n"); 
    http.end(); return;
  }
   
  // kode brugt til af få en lys på NodeMCU'en til af blinke
  /////////////////////////////////////////////////////////////////
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)
  delay(1000);                      // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(10000);                      // Wait for two seconds (to demonstrate the active low LED)
}
