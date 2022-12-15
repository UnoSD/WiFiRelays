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

IPAddress connect(ESP8266WiFiClass wifi, char* ssid, char* psk)
{
    auto status = wifi.status();
    
    switch (status)
    {
        case WL_CONNECTED:
            return wifi.localIP();
        case WL_DISCONNECTED:
            wifi.mode(WIFI_STA);
            wifi.begin(ssid, psk);
            
            return connect(wifi, ssid, psk);
        case WL_NO_SHIELD      :
        case WL_IDLE_STATUS    :
        case WL_NO_SSID_AVAIL  :
        case WL_SCAN_COMPLETED :
        case WL_CONNECT_FAILED :
        case WL_CONNECTION_LOST:
        default:
            Serial.println(status);
            delay(500);

            return connect(wifi, ssid, psk);
    }
}

void setup(void)
{
    // Set up serial bauds
    Serial.begin(115200);
    
    // Set up WiFi
    Serial.println(connect(WiFi, SECRET_SSID, SECRET_PSK));

    // Set up API
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
