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

int    HTTP_PORT   = 80;
String HTTP_METHOD = "POST";
char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME   = "/trigger";
String queryString = "?value1=26&value2=70";;

#define HOST "aspitcloud.dk"; // Enter host url with out "http://" / "/" and the start and end

const char* ssid= "ASPIT_GUEST"; // SSID'en til vores netværk
const char* password= ""; // adgangs koden til det netværk

String sendval,PostData;
int x;
void setup() {
  pinMode (13, OUTPUT);//Connect LED to pin 13
  Wire.begin(9);//9 here is the address(Mentioned even in the master board code) 
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output


  Serial.begin(9600);
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

void receiveEvent(int bytes) {
  x = Wire.read();//Receive value from master board
  Serial.print(x);
}

// the loop function runs over and over again forever
void loop() {
delay(10000);
  // kode til af hente data in fra Aurdino Uno
  //////////////////////////////////////////////
  
  
if (x > 88) {//I took the threshold as 88,you can change it to whatever you want
    analogRead(A0);
    delay(200);
  }
  else{
    analogRead(A0);
    delay(400);
  }
  



  
  // kode til af sende data til en SQL Server
  //////////////////////////////////////////////
  
  HTTPClient http; 
  WiFiClient wifiClient;
  
  sendval = "Testdata"; // vi er nød til konvertere Int's om til string
  
  PostData = "name=" + sendval;

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
