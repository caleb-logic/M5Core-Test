#include "weather.h"
#include <HTTPClient.h>

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