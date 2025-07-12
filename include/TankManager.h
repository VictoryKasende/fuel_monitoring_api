#ifndef TANK_MANAGER_H
#define TANK_MANAGER_H

#include <Arduino.h>

struct TankData {
    int tankId;
    int sensorPin;
    int redLedPin;
    int yellowLedPin;
    int greenLedPin;
    float level;        // Percentage (0-100)
    int rawValue;       // Raw sensor reading
    String status;      // "LOW", "MEDIUM", "HIGH"
    unsigned long lastUpdate;
};

class TankManager {
private:
    TankData tanks[3];
    
    float calculatePercentage(int rawValue);
    String getLevelStatus(float percentage);
    void updateLEDs(int tankIndex);
    
public:
    TankManager();
    void init();
    void updateAllTanks();
    void updateTank(int tankId);
    TankData getTankData(int tankId);
    String getAllTanksJSON();
    String getTankJSON(int tankId);
    void setLEDStatus(int tankId, bool red, bool yellow, bool green);
    float getTankLevel(int tankId);
};

#endif // TANK_MANAGER_H
