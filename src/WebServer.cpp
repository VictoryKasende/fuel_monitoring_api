#include "WebServer.h"
#include "config.h"

FuelMonitoringServer::FuelMonitoringServer(int port, TankManager* tm) 
    : server(port), tankManager(tm) {
}

void FuelMonitoringServer::init() {
    // Initialize WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    // Setup server routes
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
        handleRoot(request);
    });
    
    server.on(API_STATUS_ENDPOINT, HTTP_GET, [this](AsyncWebServerRequest *request){
        handleAPIStatus(request);
    });
    
    server.on(API_ALL_TANKS_ENDPOINT, HTTP_GET, [this](AsyncWebServerRequest *request){
        handleAPIAllTanks(request);
    });
    
    server.on("/api/tank/1", HTTP_GET, [this](AsyncWebServerRequest *request){
        AsyncWebServerResponse *response = request->beginResponse(200, "application/json", tankManager->getTankJSON(1));
        setCORSHeaders(response);
        request->send(response);
    });
    
    server.on("/api/tank/2", HTTP_GET, [this](AsyncWebServerRequest *request){
        AsyncWebServerResponse *response = request->beginResponse(200, "application/json", tankManager->getTankJSON(2));
        setCORSHeaders(response);
        request->send(response);
    });
    
    server.on("/api/tank/3", HTTP_GET, [this](AsyncWebServerRequest *request){
        AsyncWebServerResponse *response = request->beginResponse(200, "application/json", tankManager->getTankJSON(3));
        setCORSHeaders(response);
        request->send(response);
    });
    
    server.onNotFound([this](AsyncWebServerRequest *request){
        handleNotFound(request);
    });
    
    // Start server
    server.begin();
    Serial.println("HTTP server started on port " + String(WEB_SERVER_PORT));
}

void FuelMonitoringServer::handleRoot(AsyncWebServerRequest *request) {
    String html = "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "    <title>Fuel Monitoring API</title>";
    html += "    <meta charset=\"UTF-8\">";
    html += "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
    html += "    <style>";
    html += "        body { font-family: Arial, sans-serif; margin: 40px; background-color: #f5f5f5; }";
    html += "        .container { max-width: 800px; margin: 0 auto; background: white; padding: 30px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }";
    html += "        h1 { color: #333; text-align: center; }";
    html += "        .tank { margin: 20px 0; padding: 20px; border: 2px solid #ddd; border-radius: 8px; }";
    html += "        .tank-header { font-size: 18px; font-weight: bold; margin-bottom: 10px; }";
    html += "        .level-bar { width: 100%; height: 30px; background-color: #eee; border-radius: 15px; overflow: hidden; margin: 10px 0; }";
    html += "        .level-fill { height: 100%; transition: width 0.5s ease; }";
    html += "        .level-text { text-align: center; font-weight: bold; color: white; line-height: 30px; }";
    html += "        .low { background-color: #e74c3c; }";
    html += "        .medium { background-color: #f39c12; }";
    html += "        .high { background-color: #27ae60; }";
    html += "        .api-info { background-color: #ecf0f1; padding: 20px; border-radius: 8px; margin-top: 30px; }";
    html += "        .endpoint { margin: 10px 0; font-family: monospace; background: #34495e; color: white; padding: 8px; border-radius: 4px; }";
    html += "        .refresh-btn { background-color: #3498db; color: white; padding: 10px 20px; border: none; border-radius: 5px; cursor: pointer; margin: 10px 0; }";
    html += "        .refresh-btn:hover { background-color: #2980b9; }";
    html += "    </style>";
    html += "</head>";
    html += "<body>";
    html += "    <div class=\"container\">";
    html += "        <h1>Fuel Monitoring System</h1>";
    html += "        <button class=\"refresh-btn\" onclick=\"updateData()\">Actualiser</button>";
    html += "        <div id=\"tanks-container\">";
    html += "            <!-- Tanks will be populated by JavaScript -->";
    html += "        </div>";
    html += "        <div class=\"api-info\">";
    html += "            <h3>API Endpoints:</h3>";
    html += "            <div class=\"endpoint\">GET /api/status - Etat du systeme</div>";
    html += "            <div class=\"endpoint\">GET /api/tanks - Tous les reservoirs</div>";
    html += "            <div class=\"endpoint\">GET /api/tank/1 - Reservoir 1</div>";
    html += "            <div class=\"endpoint\">GET /api/tank/2 - Reservoir 2</div>";
    html += "            <div class=\"endpoint\">GET /api/tank/3 - Reservoir 3</div>";
    html += "        </div>";
    html += "    </div>";
    html += "    <script>";
    html += "        function updateData() {";
    html += "            fetch('/api/tanks')";
    html += "                .then(response => response.json())";
    html += "                .then(data => {";
    html += "                    const container = document.getElementById('tanks-container');";
    html += "                    container.innerHTML = '';";
    html += "                    data.tanks.forEach(tank => {";
    html += "                        const tankDiv = document.createElement('div');";
    html += "                        tankDiv.className = 'tank';";
    html += "                        let statusClass = tank.status.toLowerCase();";
    html += "                        tankDiv.innerHTML = ";
    html += "                            '<div class=\"tank-header\">Reservoir ' + tank.tankId + '</div>' +";
    html += "                            '<div>Niveau: ' + tank.level.toFixed(1) + '%</div>' +";
    html += "                            '<div class=\"level-bar\">' +";
    html += "                                '<div class=\"level-fill ' + statusClass + '\" style=\"width: ' + tank.level + '%\">' +";
    html += "                                    '<div class=\"level-text\">' + tank.status + '</div>' +";
    html += "                                '</div>' +";
    html += "                            '</div>' +";
    html += "                            '<div>Valeur brute: ' + tank.rawValue + '</div>' +";
    html += "                            '<div>Derniere mise a jour: ' + new Date(tank.lastUpdate).toLocaleTimeString() + '</div>';";
    html += "                        container.appendChild(tankDiv);";
    html += "                    });";
    html += "                })";
    html += "                .catch(error => console.error('Erreur:', error));";
    html += "        }";
    html += "        setInterval(updateData, 1000);";
    html += "        updateData();";
    html += "    </script>";
    html += "</body>";
    html += "</html>";
    
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", html);
    setCORSHeaders(response);
    request->send(response);
}

void FuelMonitoringServer::handleAPIStatus(AsyncWebServerRequest *request) {
    String json = "{";
    json += "\"status\":\"online\",";
    json += "\"system\":\"Fuel Monitoring API\",";
    json += "\"version\":\"1.0.0\",";
    json += "\"uptime\":" + String(millis()) + ",";
    json += "\"wifi_connected\":" + String(WiFi.status() == WL_CONNECTED ? "true" : "false") + ",";
    json += "\"ip_address\":\"" + WiFi.localIP().toString() + "\",";
    json += "\"free_heap\":" + String(ESP.getFreeHeap()) + ",";
    json += "\"timestamp\":" + String(millis());
    json += "}";
    
    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", json);
    setCORSHeaders(response);
    request->send(response);
}

void FuelMonitoringServer::handleAPIAllTanks(AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", tankManager->getAllTanksJSON());
    setCORSHeaders(response);
    request->send(response);
}

void FuelMonitoringServer::handleAPITank(AsyncWebServerRequest *request) {
    String uri = request->url();
    int tankId = uri.substring(uri.lastIndexOf('/') + 1).toInt();
    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", tankManager->getTankJSON(tankId));
    setCORSHeaders(response);
    request->send(response);
}

void FuelMonitoringServer::handleNotFound(AsyncWebServerRequest *request) {
    String json = "{";
    json += "\"error\":\"Endpoint not found\",";
    json += "\"message\":\"The requested API endpoint does not exist\",";
    json += "\"available_endpoints\":[";
    json += "\"/api/status\",";
    json += "\"/api/tanks\",";
    json += "\"/api/tank/1\",";
    json += "\"/api/tank/2\",";
    json += "\"/api/tank/3\"";
    json += "]";
    json += "}";
    
    AsyncWebServerResponse *response = request->beginResponse(404, "application/json", json);
    setCORSHeaders(response);
    request->send(response);
}

void FuelMonitoringServer::setCORSHeaders(AsyncWebServerResponse *response) {
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
}

bool FuelMonitoringServer::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String FuelMonitoringServer::getIPAddress() {
    return WiFi.localIP().toString();
}
