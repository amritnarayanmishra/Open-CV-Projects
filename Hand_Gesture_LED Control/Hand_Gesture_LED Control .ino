#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Redmi 14C 5G";
const char* password = "nercyvgwz8rm29g";

WebServer server(80);

#define LED1 2   // D4
#define LED2 4   // D2

void handleRoot() {
  server.send(200, "text/plain", "ESP32 Gesture LED System");
}

void ledOn1() { digitalWrite(LED1, HIGH); server.send(200, "text/plain", "D4 ON"); }
void ledOff1() { digitalWrite(LED1, LOW); server.send(200, "text/plain", "D4 OFF"); }
void ledOn2() { digitalWrite(LED2, HIGH); server.send(200, "text/plain", "D2 ON"); }
void ledOff2() { digitalWrite(LED2, LOW); server.send(200, "text/plain", "D2 OFF"); }
void allOn() { digitalWrite(LED1, HIGH); digitalWrite(LED2, HIGH); server.send(200, "text/plain", "ALL ON"); }
void allOff() { digitalWrite(LED1, LOW); digitalWrite(LED2, LOW); server.send(200, "text/plain", "ALL OFF"); }

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  server.on("/", handleRoot);
  server.on("/d4on", ledOn1);
  server.on("/d4off", ledOff1);
  server.on("/d2on", ledOn2);
  server.on("/d2off", ledOff2);
  server.on("/allon", allOn);
  server.on("/alloff", allOff);

  server.begin();
}

void loop() {
  server.handleClient();
}
