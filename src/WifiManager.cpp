#include "WifiManager.h"

WifiManager::WifiManager() : savedConnSize(0), foundConnSize(0) {}

int WifiManager::scanNetworks()
{
    int currentTime = millis();
    int n = WiFi.scanComplete();
    if ((currentTime - lastUpdate) > MAX_WIFI_AGE || n == -2)
    {
        Serial.println("Starting scan");
        lastUpdate = currentTime;
        WiFi.scanNetworks(true);
        return -2;
    }

    if (n != -1)
        Serial.println("Found " + String(n) + " networks");

    n = min(n, MAX_FOUND_CONN);
    foundConnSize = n;

    for (int i = 0; i < n; i++)
    {
        WifiConnection tmp;
        tmp.ssid = WiFi.SSID(i);
        tmp.known = false;
        tmp.password = "";
        tmp.rssi = WiFi.RSSI(i);
        for (size_t j = 0; j < savedConnSize; j++)
        {
            if (savedConn[j].ssid == tmp.ssid)
            {
                tmp.known = true;
                tmp.password = savedConn[j].password;
            }
        }
        foundConn[i] = tmp;
    }
    return n;
}

WifiConnection WifiManager::network(size_t i)
{
    return foundConn[i];
}

int WifiManager::getSavedConnSize()
{
    return savedConnSize;
}

bool WifiManager::saveConnections(String filepath)
{
    Serial.println("Saving connections to " + filepath);
    lastSaved = millis();
    // TODO: error handling
    File file = LittleFS.open(filepath, "w");
    for (size_t i = 0; i < savedConnSize; i++)
    {
        file.write(savedConn[i].ssid.c_str());
        file.write('/');
        file.write(savedConn[i].password.c_str());
        file.write('\n');
    }
    file.close();
    return true;
}

bool WifiManager::loadCredsFromFile(String filepath)
{
    Serial.println("Loading wifi credentials from file");
    savedConnSize = 0;
    File file = LittleFS.open(filepath, "r");
    String f = file.readString();
    for (int i = 0; i < MAX_SAVED_CONN; i++)
    {
        int fslash = f.indexOf('/');
        int nl = f.indexOf('\n');
        if (fslash == -1)
        {
            file.close();
            Serial.println("Could not load wifi credentials (bad format)");
            return false;
        }
        String ssid = f.substring(0, fslash);
        if (nl == -1)
            nl = f.length();

        String pass = f.substring(fslash + 1, nl);
        savedConn[i] = {ssid, pass, 0, true};
        f.remove(0, nl + 1);
        savedConnSize++;
        if (f.length() == 0)
        {
            file.close();
            Serial.println("Loaded wifi credentials");
            return true;
        }
    }
    file.close();
    Serial.println("Loaded wifi credentials");
    return true;
}

void WifiManager::addConnection(String ssid, String password)
{
    assert(savedConnSize + 1 < MAX_SAVED_CONN);
    savedConn[savedConnSize].ssid = ssid;
    savedConn[savedConnSize].password = password;
    savedConn[savedConnSize].known = true;
    savedConnSize++;
    saveConnections(WIFI_CREDS);
}

int cmpConn(const void *a, const void *b)
{
    return (*(WifiConnection *)b).rssi - (*(WifiConnection *)a).rssi;
}

bool WifiManager::deleteConnection(String ssid)
{
    Serial.println("Deleting connection " + ssid);
    if (savedConnSize == 0)
        return false;

    for (size_t i = 0; i < savedConnSize; i++)
    {
        if (savedConn[i].ssid == ssid)
        {
            savedConn[i] = savedConn[savedConnSize - 1];
            savedConnSize--;
            saveConnections(WIFI_CREDS);
            return true;
        }
    }
    return false;
}

bool WifiManager::autoConnect()
{
    Serial.println("Autoconnecting");

    while (scanNetworks() < 0)
    {
        delay(300);
    }

    Serial.println("Finished scanning");

    qsort(foundConn, foundConnSize, sizeof(WifiConnection), cmpConn);
    for (size_t i = 0; i < foundConnSize; i++)
    {
        if (foundConn[i].known)
        {
            connect(foundConn[i].ssid, foundConn[i].password);
            int status = WiFi.waitForConnectResult();
            if (status == WL_CONNECTED)
            {
                Serial.println("Connected to: " + foundConn[i].ssid);
                return true;
            }
        }
    }
    Serial.println("Failed to autoconnect");
    return false;
}

void WifiManager::connect(String ssid, String password)
{
    Serial.println("Connecting to: " + ssid);

    if (WiFi.isConnected())
        WiFi.disconnect();

    WiFi.begin(ssid, password);
    updateConnection(ssid, password);
}

bool WifiManager::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

String WifiManager::getStatusStr()
{
    switch (WiFi.status())
    {
    case WL_CONNECTED:
        return "connected";
    case WL_DISCONNECTED:
        return "disconnected";
    case WL_NO_SHIELD:
        return "no wifi shield";
    case WL_IDLE_STATUS:
        return "idle";
    case WL_CONNECT_FAILED:
        return "connect failed";
    case WL_NO_SSID_AVAIL:
        return "no ssid available";
    case WL_SCAN_COMPLETED:
        return "scan completed";
    case WL_CONNECTION_LOST:
        return "connection lost";
    default:
        return "unknown";
    }
}

String WifiManager::getCurrentConnJson()
{
    // TODO: Add RSSI
    String isConn = isConnected() ? "true" : "false";
    String res;
    res += "{";
    res += "\"ssid\":\"" + WiFi.SSID() + "\",";
    res += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
    res += "\"is_connected\":" + isConn + ",";
    res += "\"rssi\":" + String(WiFi.RSSI()) + ",";
    res += "\"status\":\"" + getStatusStr() + "\"";
    res += "}";

    return res;
}

void WifiManager::updateConnection(String ssid, String password)
{
    for (size_t i = 0; i < savedConnSize; i++)
    {
        if (savedConn[i].ssid == ssid)
        {
            savedConn[i].password = password;
            saveConnections(WIFI_CREDS);
            return;
        }
    }
    addConnection(ssid, password);
}

const String WifiManager::getNetworksJson()
{
    scanNetworks();
    // TODO: Add RSSI
    String res = "[";
    for (size_t i = 0; i < foundConnSize; i++)
    {
        WifiConnection c = network(i);
        if (res.length() > 1)
            res += ",";
        res += "{";
        res += "\"ssid\":\"" + c.ssid + "\",";
        res += "\"password\":\"" + c.password + "\",";
        res += "\"rssi\":" + String(c.rssi) + ",";
        String knownStr = c.known ? "true" : "false";
        res += "\"known\":" + knownStr;
        res += "}";
    }
    res += "]";

    return res;
}

WifiManager &theWifiManager()
{
    static WifiManager wm;
    return wm;
}