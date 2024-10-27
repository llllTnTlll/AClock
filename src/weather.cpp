#include "weather.hpp"


void getWeatherInfo(void *pvParameters)
{
    // 等待Wifi连接
    while (WiFi.status() != WL_CONNECTED)
    {
        vTaskDelay(pdTICKS_TO_MS(2000));
    }

    HTTPClient http;
    // https://api.openweathermap.org/data/3.0/onecall?lat={lat}&lon={lon}&exclude={part}&appid={API key}
    String url = "https://api.openweathermap.org/data/3.0/onecall?lat=" + lat + "&lon=" + lon + "&exclude=minutely" + "&appid=" + apiKey;
    http.begin(url);

    int httpResponseCode = http.GET();
    if (httpResponseCode > 0)
    {
        String payload = http.getString();
        Serial.println("Weather Data: " + payload);

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (!error)
        {
            float temperature = doc["main"]["temp"];  // 当前温度
            float humidity = doc["main"]["humidity"]; // 当前湿度
            String weather = doc["weather"][0]["main"];
        }
    }
}