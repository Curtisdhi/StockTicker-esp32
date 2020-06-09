#include "Network.h"


void Network::setup() {
    preferences.begin("network", false);
    this->ssid = preferences.getString(SSID_EEPROM_ADDR.c_str(), "");
    this->password = preferences.getString(PASSWORD_EEPROM_ADDR.c_str(), "");
    this->apiKey = preferences.getString(APIKEY_EEPROM_ADDR.c_str(), "");
    preferences.end();

    Serial.println("Type c to configure WIFI.");
    Serial.println("Configured for: "+ this->ssid ?: "N/A");
    Serial.println("Type a to configure API key.");
}

void Network::update(unsigned long *milliseconds) {
    if (Serial) {
        this->handleUserInput();
    }
    if (this->ssid != "") {
        if (lastUpdate == 0 || (*milliseconds - lastUpdate) > UPDATE_RATE) {
            lastUpdate = *milliseconds;
            this->connect();
            this->fetchStockValue();
        }
    }
}

void Network::handleUserInput() {
    if (Serial.available() > 0) {
        switch (Serial.read()) {
            case 'c':
                Serial.setTimeout(10000);
                Serial.println("Configuring wifi...");
                Serial.println("Enter WIFI SSID:");
                this->ssid = Serial.readStringUntil('\n');
                this->ssid.trim();

                Serial.println("Enter WIFI Password:");
                this->password = Serial.readStringUntil('\n');
                this->password.trim();

                Serial.println("SSID: "+ this->ssid);
                Serial.println("Password: "+ this->password);
               
                if (this->connect()) {
                    preferences.begin("network", false);
                    preferences.putString(SSID_EEPROM_ADDR.c_str(), this->ssid);
                    preferences.putString(PASSWORD_EEPROM_ADDR.c_str(), this->password);
                    preferences.end();
                    this->fetchStockValue();
                }
                break;
            case 'a':
                Serial.setTimeout(10000);
                Serial.println("Configuring API...");
                Serial.println("Enter API Key:");

                this->apiKey = Serial.readStringUntil('\n');
                this->apiKey.trim();
                Serial.println("API key: "+ this->apiKey);

                preferences.begin("network", false);
                preferences.putString(APIKEY_EEPROM_ADDR.c_str(), this->apiKey);
                preferences.end();
            default: break;
        }
    }
}

boolean Network::connect() {
    if (this->ssid == "") {
        return false;
    }
    if (WiFi.status() == WL_CONNECTED) {
        return true;
    }
    Serial.println("Connecting to SSID: "+ this->ssid);
    WiFi.begin(this->ssid.c_str(), this->password.c_str());
 
    for (byte tries = 0; tries < MAX_TRIES && WiFi.status() != WL_CONNECTED; tries++) {
        delay(500);
        Serial.println("Establishing connection to WiFi..");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connected to network");
        return true;
    } else {
        Serial.println("Failed to connected to network.");
        Serial.println(WiFi.status(), DEC);
    }

    return false;
}

void Network::disconnect() {
    WiFi.disconnect(true);
}

void Network::fetchStockValue() {
    if (this->apiKey == NULL) {
        Serial.println("Api key has not been configured. Press a to configure.");
        return;
    }
    if (WiFi.status() == WL_CONNECTED) {
        String apiUrl = API_URL + this->apiKey;
        Serial.println("Fetching webpage: "+ apiUrl);

        http.begin(apiUrl);
        int httpCode = http.GET();
        if (httpCode > 0) {  
            this->response = this->getStockFromJSON(http.getString());
            
            b_hasResponse = true;

        } else {
            Serial.println("Error on HTTP request");
        }
        http.end();
    }
}

String Network::getStockFromJSON(String data) {
    const byte COLONS_TO_SEARCH = 1;
    String result = "";
    for (int strIndex = 0, colonIndex = 0; strIndex < data.length(); strIndex++) {
        char d = data[strIndex];
        if (d == ':') {
            colonIndex++;
        } else if (colonIndex == COLONS_TO_SEARCH) {
            if (d == ',') {
                break;
            }
            if (d != 0x20) {
                result += d;
            }
        }
    }

    return result.substring(0, result.length() - 6);
}

String Network::getResponse() {
    b_hasResponse = false;
    return response;
}

boolean Network::hasResponse() {
    return b_hasResponse;
}