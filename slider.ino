#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "Slider"; 
const char* password = "password";

ESP8266WebServer server(80);
int x;
const int enable = 16; /*D0*/
const int stepping = 2; /*D4*/
const int dir = 0; /*D3*/
const int m3 = 5; /*D1*/
const int m2 = 4; /*D2*/
const int m1 = 14; /*D5*/
const int StepperSpeed = 1;
const int sliderWidth = 6200; //2' slider
int direct = 1;
int sliderWidthMulti = 0;
String webPage = "<html> <head> <style> button {   background-color: #4CAF50;  padding: 12px 28px;   width: 100%;  height: 20%;  border-radius: 8px;   border: 0px;  font-size: 16px;  color: white;   } h1 {  text-align: center;   }  </style> </head> <body> <h1>Novaspirit Slider v1</h1> <p><button onmousedown=location.href=\"/normal\">nornmal</button></p> <p><button onmousedown=location.href=\"/slow\">slow</button></p> <p><button onmousedown=location.href=\"/slower\">slower</button></p> <p><button onmousedown=location.href=\"/slowest\">slowest</button></p> </body> </html>";

void handleRoot() {
  server.send(200, "text/html", webPage);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void){
  pinMode(enable, OUTPUT);
  pinMode(stepping, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(m3, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m1, OUTPUT);
  digitalWrite(enable, 0);
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  
  
  
  IPAddress apip = WiFi.softAPIP();
  Serial.println("apip");
  /* Wait for connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  */
  server.on("/", handleRoot);
  server.on("/normal", [](){
    server.send(200, "text/html", webPage);
    if (direct == 0)
    {
      digitalWrite(dir, 1);
      direct = 1;
    }else
    {
      digitalWrite(dir, 0);
      direct = 0;
    }
    digitalWrite(m1, 1);
    digitalWrite(m2, 0);
    digitalWrite(m3, 0);
    for(x=0; x < sliderWidth; x++){
      digitalWrite(stepping, 1);
      digitalWrite(stepping, 0);
      delay(StepperSpeed);
    }
  });
  server.on("/slow", [](){
    server.send(200, "text/html", webPage);
    if (direct == 0)
    {
      digitalWrite(dir, 1);
      direct = 1;
    }else
    {
      digitalWrite(dir, 0);
      direct = 0;
    }
    digitalWrite(m1, 0);
    digitalWrite(m2, 1);
    digitalWrite(m3, 0);
    sliderWidthMulti = sliderWidth * 2;
    for(x=0; x < sliderWidthMulti; x++){
      digitalWrite(stepping, 1);
      digitalWrite(stepping, 0);
      delay(StepperSpeed);
    }
  });
  server.on("/slower", [](){
    server.send(200, "text/html", webPage);
        if (direct == 0)
    {
      digitalWrite(dir, 1);
      direct = 1;
    }else
    {
      digitalWrite(dir, 0);
      direct = 0;
    }
    digitalWrite(m1, 1);
    digitalWrite(m2, 1);
    digitalWrite(m3, 0);
    sliderWidthMulti = sliderWidth * 4;
    for(x=0; x < sliderWidthMulti; x++){
      digitalWrite(stepping, 1);
      digitalWrite(stepping, 0);
      delay(StepperSpeed);
    }
  });
  server.on("/slowest", [](){
    server.send(200, "text/html", webPage);
    if (direct == 0)
    {
      digitalWrite(dir, 1);
      direct = 1;
    }else
    {
      digitalWrite(dir, 0);
      direct = 0;
    }
    digitalWrite(m1, 1);
    digitalWrite(m2, 1);
    digitalWrite(m3, 1);
    sliderWidthMulti = sliderWidth * 8;
    for(x=0; x < sliderWidthMulti; x++){
      digitalWrite(stepping, 1);
      digitalWrite(stepping, 0);
      delay(StepperSpeed);
    }
  });
  

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
