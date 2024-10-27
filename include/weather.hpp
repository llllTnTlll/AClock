#include<WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

static const String lon = "130.48";
static const String lat = "33.52";
static const String apiKey = "485d81ec91b0e8ec7e766fa74df79dab";

void getWeatherInfo(void *pvParameters);