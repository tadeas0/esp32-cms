#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include "constants.h"

#define MAX_SAVED_CONN 10
#define MAX_FOUND_CONN 30
#define MAX_WIFI_AGE 10000
#define CONN_SAVE_DELAY 30000 // Saves connection to file every CONN_SAVE_DELAY milliseconds

struct WifiConnection
{
    String ssid;
    String password;
    int rssi;
    bool known;
};

int cmpConn(const void *a, const void *b);

class WifiManager
{
public:
    int scanNetworks();
    WifiConnection network(size_t i);
    const String getNetworksJson();
    bool loadCredsFromFile(String filepath);
    int getSavedConnSize();
    bool saveConnections(String filepath);
    bool deleteConnection(String ssid);
    void addConnection(String ssid, String password);
    void connect(String ssid, String password);
    bool autoConnect();
    bool isConnected();
    String getCurrentConnJson();
    String getStatusStr();
    friend WifiManager &theWifiManager();

private:
    WifiManager();
    void updateConnection(String ssid, String password);
    unsigned long lastUpdate;
    unsigned long lastSaved;
    size_t savedConnSize;
    size_t foundConnSize;
    WifiConnection savedConn[MAX_SAVED_CONN];
    WifiConnection foundConn[MAX_FOUND_CONN];
};

extern WifiManager &theWifiManager();

#endif