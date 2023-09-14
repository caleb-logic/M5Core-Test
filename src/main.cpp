#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <M5CoreInk.h>
#include <vector>
#include <map>
#include "weather.h"

struct WifiCredentials
{
    const char *ssid;
    const char *password;
};

struct CityWeather
{
    const char *id;
    const char *name;
};

// Replace with your WiFi credentials
// Device will attempt to connect to them in order.
WifiCredentials wifiCredentials[] = {
    {"LogicNE", "N3metr12"},
    {"ssid2", "password2"},
    {"ssid3", "password3"}};

int loopCount = 0;

Ink_Sprite InkPageSprite(&M5.M5Ink);

void WriteLine(String line, int num)
{
    Serial.println(line);
    InkPageSprite.drawString(25, (15 * num), line.c_str());
    InkPageSprite.pushSprite();
    // delay(1000);
}

void connectWiFi()
{
    int wifiCount = sizeof(wifiCredentials) / sizeof(WifiCredentials);
    int wifiIndex = 0;

    while (WiFi.status() != WL_CONNECTED && wifiIndex <= wifiCount)
    {
        const char *ssid = wifiCredentials[wifiIndex].ssid;
        const char *password = wifiCredentials[wifiIndex].password;

        WiFi.begin(ssid, password);

        Serial.print("Connecting to WiFi ");
        Serial.println(ssid);

        delay(5000);

        wifiIndex++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Connected to WiFi");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.println("Failed to connect to WiFi");
    }
}


void setup()
{
    Serial.begin(9600);

    connectWiFi();

    M5.begin(true,false,true);

    M5.M5Ink.isInit();
    M5.M5Ink.clear();
    delay(1000);

    if (InkPageSprite.creatSprite(0, 0, 200, 200, true) != 0)
    {
        Serial.println("Ink Sprite create failed");
    }
    else
    {
        Serial.println("creatSprite success");
    }
}

void GetWeather(CityWeather cities[], int lastLine){
    int cityCount =  sizeof(cities) / sizeof(cities);
    int cityIndex = 0;

    M5.M5Ink.clear();

    if (InkPageSprite.creatSprite(0, 0, 200, 200, true) != 0)
    {
        Serial.println("Ink Sprite create failed");
    }
    else
    {
        Serial.println("creatSprite success");
    }

    while (cityIndex <= cityCount)
    {

        String Name = cities[cityIndex].name;
        String Id = cities[cityIndex].id;
        String WeatherData = getCurrentWeather(Id);
        DynamicJsonDocument JsonDoc(1024);

        deserializeJson(JsonDoc, WeatherData);

        float Temperature = JsonDoc["main"]["temp"];
        String WeatherDescription = JsonDoc["weather"][0]["description"].as<String>();

        WriteLine(Name, lastLine++);
        WriteLine("Temperature: " + String(Temperature) + " F", lastLine++);
        WriteLine("Description:", lastLine++);

        std::vector<std::string> splits = splitWeatherDescription(WeatherDescription.c_str());
        for (int i = 0; i < splits.size(); i++)
        {
            WriteLine(splits[i].c_str(), lastLine++);
        }

        WriteLine("", lastLine++);

        cityIndex++;
    }
}

void loop()
{
    // Enter your city IDs and names here
    CityWeather cities[] = {
        {"5063342", "Aurora"},
        {"5069773", "Harvard"}};

    int lastLine = 1;

    if(M5.BtnMID.wasPressed()){
        // It's over 9000!
        WriteLine("It's over 9000!", lastLine++);
        loopCount = 9001;
        M5.Speaker.setBeep(5000,2);
        M5.Speaker.setVolume(100);
        M5.Speaker.beep();
        M5.update();
        delay(500);
        M5.Speaker.mute();
    }

    if (loopCount == 0 || loopCount > 1800){
        GetWeather(cities, lastLine);
    }
    
    if(loopCount > 1800){
        loopCount = 1;
    }else{
        loopCount++;
    }

    // Delay for 1/2 second
    delay(500);
    M5.update();

    // Delay for 5 seconds
    // delay(5000);

    // Delay for 15 minutes
    // delay(900000);

}