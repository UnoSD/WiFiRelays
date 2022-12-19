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

const char* prettyWiFiStatus(wl_status_t status)
{
    switch (status)
    {
        case WL_NO_SHIELD      : return "WL_NO_SHIELD";
        case WL_IDLE_STATUS    : return "WL_IDLE_STATUS";
        case WL_NO_SSID_AVAIL  : return "WL_NO_SSID_AVAIL";
        case WL_SCAN_COMPLETED : return "WL_SCAN_COMPLETED";
        case WL_CONNECTED      : return "WL_CONNECTED";
        case WL_CONNECT_FAILED : return "WL_CONNECT_FAILED";
        case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
        case WL_DISCONNECTED   : return "WL_DISCONNECTED";
        default                : return "Unknown";
    }
}

IPAddress connect(ESP8266WiFiClass wifi, char* ssid, char* psk, bool initiated)
{
    auto status = wifi.status();
    
    switch (status)
    {
        case WL_CONNECTED:
            return wifi.localIP();
        case WL_DISCONNECTED:
            if (!initiated)
            {
                wifi.mode(WIFI_STA);
                wifi.begin(ssid, psk);
            }
            
            delay(500);
            
            return connect(wifi, ssid, psk, true);
        case WL_NO_SHIELD      :
        case WL_IDLE_STATUS    :
        case WL_NO_SSID_AVAIL  :
        case WL_SCAN_COMPLETED :
        case WL_CONNECT_FAILED :
        case WL_CONNECTION_LOST:
        default:
            Serial.println(status);

            return connect(wifi, ssid, psk, false);
    }
}

void setup(void)
{
    // Set up serial bauds
    Serial.begin(115200);

    // Set up WiFi
    Serial.println(connect(WiFi, SECRET_SSID, SECRET_PSK, false));

    // Set up API
    for (auto [ name, pinId ] : (tuple<string, int>[]) { { "d0", D0 },
                                                         { "d1", D1 },
                                                         { "d2", D2 } })
    {
        for (auto [ displayName, outputValueId ] : (tuple<string, int>[]) { { "on" , HIGH },
                                                                            { "off", LOW  } })
            server.on(("/" + name + "/" + displayName).c_str(), [=]() {
                digitalWrite(pinId, outputValueId);
                server.send(200, "text/plain", (name + " " + displayName).c_str());
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
