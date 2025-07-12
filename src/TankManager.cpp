#include "TankManager.h"
#include "config.h"

TankManager::TankManager() {
    // Initialize tank configurations
    tanks[0] = {1, SENSOR_TANK_1, LED_TANK_1_RED, LED_TANK_1_YELLOW, LED_TANK_1_GREEN, 0.0, 0, "UNKNOWN", 0};
    tanks[1] = {2, SENSOR_TANK_2, LED_TANK_2_RED, LED_TANK_2_YELLOW, LED_TANK_2_GREEN, 0.0, 0, "UNKNOWN", 0};
    tanks[2] = {3, SENSOR_TANK_3, LED_TANK_3_RED, LED_TANK_3_YELLOW, LED_TANK_3_GREEN, 0.0, 0, "UNKNOWN", 0};
}

void TankManager::init() {
    // Initialize LED pins as outputs
    for (int i = 0; i < NUM_TANKS; i++) {
        pinMode(tanks[i].redLedPin, OUTPUT);
        pinMode(tanks[i].yellowLedPin, OUTPUT);
        pinMode(tanks[i].greenLedPin, OUTPUT);
        
        // Turn off all LEDs initially
        digitalWrite(tanks[i].redLedPin, LOW);
        digitalWrite(tanks[i].yellowLedPin, LOW);
        digitalWrite(tanks[i].greenLedPin, LOW);
    }
    
    Serial.println("TankManager initialized successfully");
}

void TankManager::updateAllTanks() {
    for (int i = 0; i < NUM_TANKS; i++) {
        updateTank(i + 1);
    }
}

void TankManager::updateTank(int tankId) {
    if (tankId < 1 || tankId > NUM_TANKS) return;
    
    int index = tankId - 1;
    
    // Read sensor value
    tanks[index].rawValue = analogRead(tanks[index].sensorPin);
    
    // Calculate percentage
    tanks[index].level = calculatePercentage(tanks[index].rawValue);
    
    // Update status
    tanks[index].status = getLevelStatus(tanks[index].level);
    
    // Update LEDs
    updateLEDs(index);
    
    // Update timestamp
    tanks[index].lastUpdate = millis();
}

float TankManager::calculatePercentage(int rawValue) {
    // Convert raw sensor value to percentage
    float percentage = map(rawValue, SENSOR_MIN_VALUE, SENSOR_MAX_VALUE, 0, 100);
    
    // Constrain to 0-100 range
    percentage = constrain(percentage, 0.0, 100.0);
    
    return percentage;
}

String TankManager::getLevelStatus(float percentage) {
    if (percentage < LOW_LEVEL_THRESHOLD) {
        return "LOW";
    } else if (percentage < MEDIUM_LEVEL_THRESHOLD) {
        return "MEDIUM";
    } else {
        return "HIGH";
    }
}

void TankManager::updateLEDs(int tankIndex) {
    float level = tanks[tankIndex].level;
    
    // Turn off all LEDs first
    digitalWrite(tanks[tankIndex].redLedPin, LOW);
    digitalWrite(tanks[tankIndex].yellowLedPin, LOW);
    digitalWrite(tanks[tankIndex].greenLedPin, LOW);
    
    // Turn on appropriate LED based on level
    if (level < LOW_LEVEL_THRESHOLD) {
        digitalWrite(tanks[tankIndex].redLedPin, HIGH);
    } else if (level < MEDIUM_LEVEL_THRESHOLD) {
        digitalWrite(tanks[tankIndex].yellowLedPin, HIGH);
    } else {
        digitalWrite(tanks[tankIndex].greenLedPin, HIGH);
    }
}

TankData TankManager::getTankData(int tankId) {
    if (tankId < 1 || tankId > NUM_TANKS) {
        // Return empty tank data for invalid ID
        return {0, 0, 0, 0, 0, 0.0, 0, "INVALID", 0};
    }
    
    return tanks[tankId - 1];
}

String TankManager::getAllTanksJSON() {
    String json = "{\"tanks\":[";
    
    for (int i = 0; i < NUM_TANKS; i++) {
        if (i > 0) json += ",";
        json += getTankJSON(i + 1);
    }
    
    json += "],\"timestamp\":" + String(millis()) + "}";
    return json;
}

String TankManager::getTankJSON(int tankId) {
    if (tankId < 1 || tankId > NUM_TANKS) {
        return "{\"error\":\"Invalid tank ID\"}";
    }
    
    TankData tank = tanks[tankId - 1];
    
    String json = "{";
    json += "\"tankId\":" + String(tank.tankId) + ",";
    json += "\"level\":" + String(tank.level, 2) + ",";
    json += "\"rawValue\":" + String(tank.rawValue) + ",";
    json += "\"status\":\"" + tank.status + "\",";
    json += "\"lastUpdate\":" + String(tank.lastUpdate);
    json += "}";
    
    return json;
}

void TankManager::setLEDStatus(int tankId, bool red, bool yellow, bool green) {
    if (tankId < 1 || tankId > NUM_TANKS) return;
    
    int index = tankId - 1;
    digitalWrite(tanks[index].redLedPin, red ? HIGH : LOW);
    digitalWrite(tanks[index].yellowLedPin, yellow ? HIGH : LOW);
    digitalWrite(tanks[index].greenLedPin, green ? HIGH : LOW);
}

float TankManager::getTankLevel(int tankId) {
    if (tankId < 1 || tankId > NUM_TANKS) return -1.0;
    return tanks[tankId - 1].level;
}
