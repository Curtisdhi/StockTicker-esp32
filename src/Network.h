#pragma once
#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>
#include <HTTPClient.h>

const String SSID_EEPROM_ADDR = "W_SSID";
const String PASSWORD_EEPROM_ADDR = "W_PASS";
const String APIKEY_EEPROM_ADDR = "W_APIKEY";

const long UPDATE_RATE = 60000;
const byte MAX_TRIES = 20;

const String API_URL = "https://finnhub.io/api/v1/quote?symbol=AMD&token=";

class Network {
    private:
        Preferences preferences;
        HTTPClient http;
        String ssid;
        String password;
        String apiKey;
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
        String getStockFromJSON(String data);
        String getResponse();
        boolean hasResponse();
};