#ifndef WEATHER_H
#define WEATHER_H

#include <Arduino.h>
#include <vector>
#include <string>
#include <HTTPClient.h>

// Function to split the weatherDescription string into substrings
std::vector<std::string> splitWeatherDescription(const std::string &weatherDescription);
String getCurrentWeather(String city);

#endif