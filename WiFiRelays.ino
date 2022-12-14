#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "string"

// Secret macros
// Create arduino_secrets.h containing:
// #define SECRET_SSID "Your WiFi SSID"
// #define SECRET_PSK  "Your WiFi password"
#include "arduino_secrets.h" 

using namespace std;

const char*      ssid     = SECRET_SSID;
const char*      password = SECRET_PSK;
ESP8266WebServer server(80);

void setup(void)
{
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

    // Set up API
    // Does C++ have cartesian product in standard library?
    // If so, do the product of pinMap and { on, off } and
    // use a single expression in loop
    for (auto [ name, pinId ] : (tuple<string, int>[]) { { "d0", D0 },
                                                         { "d1", D1 },
                                                         { "d2", D2 } })
    {
        server.on(("/" + name + "/off").c_str(), [=]() {
            digitalWrite(pinId, LOW);
            server.send(200, "text/plain", (name + " off").c_str());
        });
       
        server.on(("/" + name + "/on").c_str(), [=]() {
            digitalWrite(pinId, HIGH);
            server.send(200, "text/plain", (name + " on").c_str());
        });
        
        pinMode(pinId, OUTPUT);
    }

    // Start web server
    server.begin();
}

void loop(void)
{
    server.handleClient();
}
