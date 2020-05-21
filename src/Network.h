#pragma once
#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>
#include <HTTPClient.h>

const String SSID_EEPROM_ADDR = "W_SSID";
const String PASSWORD_EEPROM_ADDR = "W_PASS";

const long UPDATE_RATE = 60000;
const byte MAX_TRIES = 20;

const String API_URL = "https://financialmodelingprep.com/api/v3/quote/AMD?apikey=678874e6accbbc37fc94ef942c8a2315";

class Network {
    private:
        Preferences preferences;
        HTTPClient http;
        String ssid;
        String password;
        unsigned long lastUpdate;
        String response;
        boolean b_hasResponse;
        
    public:
        void setup();
        void update(unsigned long *milliseconds);
        void handleUserInput();
        boolean connect();
        void disconnect();
        void fetchStockValue();
        String getStockFromCSV(String data);
        String getResponse();
        boolean hasResponse();
};