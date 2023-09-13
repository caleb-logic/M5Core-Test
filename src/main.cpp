#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <M5CoreInk.h>
#include <vector>
#include <map>

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

String getCurrentWeather(String city)
{
    String apiKey = "bddf0cd56c584112a50d217a543ee815"; // Replace with your OpenWeatherMap API key

    String url = "http://api.openweathermap.org/data/2.5/weather?id=" + city + "&appid=" + apiKey + "&units=imperial";

    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();

    String response = "";

    if (httpCode == HTTP_CODE_OK)
    {
        response = http.getString();
    }
    else
    {
        Serial.println("Failed to get weather data");
    }

    http.end();
    return response;
}

// Function to split the weatherDescription string into substrings
std::vector<std::string> splitWeatherDescription(const std::string &weatherDescription)
{
    std::vector<std::string> splits;
    int startPos = 0;

    for (int i = 20; i < weatherDescription.length(); i += 20)
    {
        std::string split = weatherDescription.substr(startPos, i - startPos);
        int spacePos = split.find_last_of(' ');

        if ((i - spacePos) < 5)
        {
            split = weatherDescription.substr(startPos, spacePos);
            i = startPos + spacePos;
        }

        splits.push_back(split);
        startPos = i;
    }
    splits.push_back(weatherDescription.substr(startPos));
    return splits;
}

void setup()
{
    Serial.begin(9600);

    connectWiFi();

    M5.begin();
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

void loop()
{
    // Enter your city IDs and names here
    CityWeather cities[] = {
        {"5063342", "Aurora"},
        {"5069773", "Harvard"}};

    int lastLine = 1;

    int cityCount =  sizeof(cities) / sizeof(cities);
    int cityIndex = 0;

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

    // Delay for 5 seconds
    // delay(5000);

    // Delay for 15 minutes
    delay(900000);

    M5.M5Ink.clear();

    if (InkPageSprite.creatSprite(0, 0, 200, 200, true) != 0)
    {
        Serial.println("Ink Sprite create failed");
    }
    else
    {
        Serial.println("creatSprite success");
    }
}