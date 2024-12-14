#include <Arduino.h>
#include <WiFi.h>
#include <HttpClient.h>
#include <Servo.h>

#define MOTOR_PIN 33
#define BUTTON_PIN 37
#define LIGHT_SENSOR_PIN 15

// WiFi credentials
const char ssid[] = "SSID"; 
const char pass[] = "PASS"; 

// Flask server details
const char serverIP[] = "0.0.0.0"; 
const int serverPort = 5000;              // Flask server port

Servo motor;

void setup() {
    Serial.begin(9600);

    // Initialize motor and pins
    motor.attach(MOTOR_PIN);
    motor.write(0); // Start motor at 0 degrees
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LIGHT_SENSOR_PIN, INPUT);

    // Connect to WiFi
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void sendDataToServer(bool switchState, int lightSensorValue) {
    WiFiClient client;
    HttpClient http(client);

    // Prepare JSON payload
    String jsonPayload = "{\"switch\": \"" + String(switchState ? "ON" : "OFF") + "\", \"light\": " + String(lightSensorValue) + "}";
    Serial.println("Sending data: " + jsonPayload);

    // POST request to the server
    http.beginRequest();
    http.post(serverIP, serverPort, "/post-data"); // Flask endpoint
    http.sendHeader("Content-Type", "application/json");
    http.sendHeader("Content-Length", jsonPayload.length());
    http.endRequest();
    http.write((const uint8_t *)jsonPayload.c_str(), jsonPayload.length());

    // Handle server response
    int responseCode = http.responseStatusCode();
    if (responseCode > 0) {
        Serial.print("Server response code: ");
        Serial.println(responseCode);
    } else {
        Serial.println("Failed to get a response from the server.");
    }

    http.stop();
}

void loop() {
    // Check if the button is pressed (switch state)
    bool isButtonPressed = (digitalRead(BUTTON_PIN) == LOW); // LOW indicates button press
    String switchState = isButtonPressed ? "ON" : "OFF";

    // Read light sensor value
    int lightSensorValue = digitalRead(LIGHT_SENSOR_PIN);

    // Print values for debugging
    Serial.print("Switch: ");
    Serial.println(switchState);
    Serial.print("Light Sensor: ");
    Serial.println(lightSensorValue);

    // Send data to the server
    sendDataToServer(isButtonPressed, lightSensorValue);

    // Control motor based on light sensor
    if (lightSensorValue == 0) {
        motor.write(180); // Move motor to 180 degrees
        Serial.println("Motor moved to 180 degrees (low light)");
    } else {
        motor.write(0); // Reset motor to 0 degrees
        Serial.println("Motor reset to 0 degrees (sufficient light)");
    }

    delay(5000);
}
