#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Secret macros
// Create arduino_secrets.h containing:
// #define SECRET_SSID "Your WiFi SSID"
// #define SECRET_PSK  "Your WiFi password"
#include "arduino_secrets.h" 

const char*      ssid     = SECRET_SSID;
const char*      password = SECRET_PSK;
ESP8266WebServer server(80);

void setup(void) {
    // Set up serial bauds
    Serial.begin(115200);
    
    // Set up WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    // Wait for connection
    Serial.print("Connecting");
  
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());

    //for (auto pin : { D0, D1, D2 })
    //{
    //    server.on("/on" + i, []() {
    //        digitalWrite(pin, HIGH);
    //        server.send(200, "text/plain", "D{0} on");
    //    });
    //
    //    server.on("/off" + i, []() {
    //        digitalWrite(pin, LOW);
    //        server.send(200, "text/plain", "D{0} off");
    //    });
    //    
    //    pinMode(pim, OUTPUT);
    //}

    // Set up API
    server.on("/on0", []() {
        digitalWrite(D0, HIGH);
        server.send(200, "text/plain", "D0 on");
    });
    
    server.on("/off0", []() {
        digitalWrite(D0, LOW);
        server.send(200, "text/plain", "D0 off");
    });

    server.on("/on1", []() {
        digitalWrite(D1, HIGH);
        server.send(200, "text/plain", "D1 on");
    });
    
    server.on("/off1", []() {
        digitalWrite(D1, LOW);
        server.send(200, "text/plain", "D1 off");
    });

    server.on("/on2", []() {
        digitalWrite(D2, HIGH);
        server.send(200, "text/plain", "D2 on");
    });
    
    server.on("/off2", []() {
        digitalWrite(D2, LOW);
        server.send(200, "text/plain", "D2 off");
    });

    // Start web server
    server.begin();
    
    // Set up pins as output
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
}

void loop(void) {
    server.handleClient();
}
