#include <Arduino.h>
#include "config.h"
#include "TankManager.h"
#include "WebServer.h"

// Global instances
TankManager tankManager;
FuelMonitoringServer webServer(WEB_SERVER_PORT, &tankManager);

// Timing variables
unsigned long lastUpdate = 0;

void setup() {
    
    // Initialize Serial communication
    Serial.begin(115200);
    Serial.println("\n=== Fuel Monitoring API Starting ===");
    
    // Initialize tank manager
    tankManager.init();
    Serial.println("Tank Manager initialized");
    
    // Initialize web server (includes WiFi connection)
    webServer.init();
    Serial.println("Web Server initialized");
    
  
    // Initial sensor reading
    tankManager.updateAllTanks();
    Serial.println("Initial sensor readings complete");
    
    Serial.println("=== System Ready ===");
    Serial.println("Access the web interface at: http://" + webServer.getIPAddress());
    Serial.println("API endpoints:");
    Serial.println("  - GET /api/status");
    Serial.println("  - GET /api/tanks");
    Serial.println("  - GET /api/tank/1");
    Serial.println("  - GET /api/tank/2");
    Serial.println("  - GET /api/tank/3");
}

void loop() {
    // AsyncWebServer handles requests automatically, no need for handleClient()
    
    // Update sensors every second
    unsigned long currentTime = millis();
    if (currentTime - lastUpdate >= UPDATE_INTERVAL_MS) {
        tankManager.updateAllTanks();
        lastUpdate = currentTime;
        
        // Optional: Print status to Serial for debugging
        if (Serial.available()) {
            String command = Serial.readString();
            command.trim();
            
            if (command == "status") {
                Serial.println("\n=== Tank Status ===");
                for (int i = 1; i <= 3; i++) {
                    TankData tank = tankManager.getTankData(i);
                    Serial.printf("Tank %d: %.1f%% (%s) - Raw: %d\n", 
                                tank.tankId, tank.level, tank.status.c_str(), tank.rawValue);
                }
                Serial.println("==================");
            }
        }
    }
    
    // Check WiFi connection
    if (!webServer.isConnected()) {
        Serial.println("WiFi connection lost! Attempting to reconnect...");
        // You could add WiFi reconnection logic here if needed
    }
    
    // Small delay to prevent excessive CPU usage
    delay(10);
}
