#include "Network.h"


void Network::setup() {
    preferences.begin("network", false);
    this->ssid = preferences.getString(SSID_EEPROM_ADDR.c_str(), "");
    this->password = preferences.getString(PASSWORD_EEPROM_ADDR.c_str(), "");
    preferences.end();

    Serial.println("Type c to configure WIFI.");
    Serial.println("Configured for: "+ this->ssid ?: "N/A");
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
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Fetching webpage: "+ API_URL);

        http.begin(API_URL);
        int httpCode = http.GET();
        if (httpCode > 0) {  
            this->response = this->getStockFromCSV(http.getString());
            
            b_hasResponse = true;

        } else {
            Serial.println("Error on HTTP request");
        }
        http.end();
    }
}

String Network::getStockFromCSV(String data) {
    const byte COLONS_TO_SEARCH = 3;
    String result = "";
    for (int strIndex = 0, commaIndex = 0; strIndex < data.length(); strIndex++) {
        char d = data[strIndex];
        if (d == ':') {
            commaIndex++;
        } else if (commaIndex == COLONS_TO_SEARCH) {
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