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

ESP8266WebServer server(80);

IPAddress connect(char* ssid, char* psk)
{
    auto status = WiFi.status();
    
    switch (status)
    {
        case WL_CONNECTED:
          return WiFi.localIP();
        case WL_DISCONNECTED:
          WiFi.mode(WIFI_STA);
          WiFi.begin(ssid, psk);
          
          return connect(ssid, psk);
        default:
          Serial.println(WiFi.status());
          delay(500);
          
          return connect(ssid, psk);
    }
}

void setup(void)
{
    // Set up serial bauds
    Serial.begin(115200);
    
    // Set up WiFi
    Serial.println(connect(SECRET_SSID, SECRET_PSK));

    // Set up API
    // Does C++ have cartesian product in standard library?
    // If so, do the product of pinMap and { on, off } and
    // use a single expression in loop
    for (auto [ name, pinId ] : (tuple<string, int>[]) { { "d0", D0 },
                                                         { "d1", D1 },
                                                         { "d2", D2 } })
    {
        for (auto [ displayName, outputValueId ] : (tuple<string, int>[]) { { "on" , HIGH },
                                                                            { "off", LOW  } })
            server.on(("/" + name + "/off").c_str(), [=]() {
                digitalWrite(pinId, LOW);
                server.send(200, "text/plain", (name + " off").c_str());
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
