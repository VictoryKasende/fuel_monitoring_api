# 🛢️ Fuel Monitoring API

Un système professionnel de monitoring de carburant utilisant ESP32-S3 avec 3 capteurs d'eau et 9 LEDs indicatrices.

## 🚀 Fonctionnalités

- **3 Réservoirs** avec monitoring en temps réel
- **9 LEDs indicatrices** (3 par réservoir : Rouge, Jaune, Vert)
- **API REST** pour accès aux données
- **Interface Web** intuitive et responsive
- **Mise à jour en temps réel** (chaque seconde)
- **WiFi intégré** pour accès distant
- **AsyncWebServer** pour performance optimale

## 📊 Système de Niveaux

| Niveau | Pourcentage | LED | Status |
|--------|-------------|-----|--------|
| Bas | < 30% | 🔴 Rouge | LOW |
| Moyen | 30-70% | 🟡 Jaune | MEDIUM |
| Haut | > 70% | 🟢 Vert | HIGH |

## 🔌 Connexions Hardware ESP32-S3

### Capteurs (GPIO Analogiques)
- **Tank 1**: GPIO1 (ADC1_CH0)
- **Tank 2**: GPIO2 (ADC1_CH1)  
- **Tank 3**: GPIO3 (ADC1_CH2)

### LEDs (GPIO Digitaux)
| Réservoir | Rouge | Jaune | Vert |
|-----------|-------|-------|------|
| Tank 1 | GPIO10 | GPIO11 | GPIO12 |
| Tank 2 | GPIO13 | GPIO14 | GPIO15 |
| Tank 3 | GPIO16 | GPIO17 | GPIO18 |

## 🌐 API Endpoints

### Status du Système
```http
GET /api/status
```

### Tous les Réservoirs
```http
GET /api/tanks
```

### Réservoir Spécifique
```http
GET /api/tank/1
GET /api/tank/2
GET /api/tank/3
```

## 📱 Interface Web

Accédez à l'interface web via l'adresse IP de l'ESP32:
```
http://192.168.x.xxx/
```

L'interface affiche :
- **Dashboard temps réel** avec barres de progression
- **Niveaux visuels** avec couleurs d'état
- **Auto-refresh** chaque seconde
- **Information système** et endpoints API

## ⚙️ Configuration

1. **Modifier le WiFi** dans `include/config.h`:
```cpp
#define WIFI_SSID "VOTRE_WIFI"
#define WIFI_PASSWORD "VOTRE_MOT_DE_PASSE"
```

2. **Ajuster les seuils** si nécessaire:
```cpp
#define LOW_LEVEL_THRESHOLD 20
#define MEDIUM_LEVEL_THRESHOLD 70
#define HIGH_LEVEL_THRESHOLD 100
```

3. **Calibrer les capteurs ESP32-S3**:
```cpp
#define SENSOR_MIN_VALUE 0      // Réservoir vide
#define SENSOR_MAX_VALUE 4095   // Réservoir plein (12-bit ADC)
```

## 🔧 Installation

1. **Cloner le projet**
2. **Ouvrir avec PlatformIO/VS Code**
3. **Modifier la configuration WiFi**
4. **Ajouter utilisateur au groupe dialout** (Linux):
   ```bash
   sudo usermod -a -G dialout $USER
   ```
5. **Redémarrer la session**
6. **Compiler et télécharger**

### Via VS Code:
- `Ctrl+Shift+P` → "PlatformIO: Upload"
- Ou utiliser les boutons dans la barre PlatformIO

### Via Terminal:
```bash
pio run --target upload
pio device monitor
```

## 📊 Réponse JSON

### Exemple de réponse `/api/tanks`:
```json
{
  "tanks": [
    {
      "tankId": 1,
      "level": 85.30,
      "rawValue": 3470,
      "status": "HIGH",
      "lastUpdate": 12345678
    },
    {
      "tankId": 2,
      "level": 45.20,
      "rawValue": 1851,
      "status": "MEDIUM",
      "lastUpdate": 12345678
    },
    {
      "tankId": 3,
      "level": 15.80,
      "rawValue": 647,
      "status": "LOW",
      "lastUpdate": 12345678
    }
  ],
  "timestamp": 12345678
}
```

### Exemple de réponse `/api/status`:
```json
{
  "status": "online",
  "system": "Fuel Monitoring API",
  "version": "1.0.0",
  "uptime": 123456,
  "wifi_connected": true,
  "ip_address": "192.168.1.100",
  "free_heap": 250000,
  "timestamp": 123456
}
```

## 🐛 Debug

### Moniteur Série
```bash
pio device monitor
```

### Commandes série disponibles:
- Tapez `status` dans le moniteur série pour voir l'état des réservoirs

### Logs système:
```
=== Fuel Monitoring API Starting ===
Tank Manager initialized
Connecting to WiFi....
WiFi connected!
IP address: 192.168.1.100
HTTP server started on port 80
Initial sensor readings complete
=== System Ready ===
```

## 📁 Structure du Projet

```
fuel_monitoring_api/
├── include/
│   ├── config.h          # Configuration générale + pins GPIO
│   ├── TankManager.h     # Gestion des réservoirs et LEDs
│   └── WebServer.h       # AsyncWebServer et API REST
├── src/
│   ├── main.cpp          # Programme principal
│   ├── TankManager.cpp   # Logique capteurs + LEDs
│   └── WebServer.cpp     # AsyncWebServer + interface web
├── platformio.ini        # Configuration PlatformIO + bibliothèques
└── README.md            # Documentation
```

## 🔄 Cycle de Fonctionnement

1. **Initialisation** du système et pins GPIO
2. **Connexion WiFi** automatique
3. **Démarrage AsyncWebServer** sur port 80
4. **Lecture capteurs ADC** (chaque seconde)
5. **Mise à jour LEDs** selon les niveaux
6. **Traitement requêtes API** asynchrone

## ⚡ Performance ESP32-S3

- **Fréquence de mise à jour**: 1 Hz (1 seconde)
- **Résolution ADC**: 12 bits (0-4095)
- **Temps de réponse API**: < 50ms
- **AsyncWebServer**: Gestion multiple clients
- **Mémoire libre**: ~250KB+
- **WiFi**: WPA2 sécurisé

## 🛠️ Développement

Pour étendre le système:

1. **Ajouter plus de réservoirs**: 
   - Modifier `NUM_TANKS` dans `config.h`
   - Ajouter pins GPIO dans `config.h`
   - Mettre à jour `TankManager.cpp`

2. **Changer les seuils**: 
   - Ajuster `LOW_LEVEL_THRESHOLD` et `MEDIUM_LEVEL_THRESHOLD`

3. **Nouveaux endpoints**: 
   - Ajouter routes dans `WebServer.cpp::init()`

4. **Alertes/Notifications**: 
   - Implémenter dans `TankManager.cpp`
   - Ajouter endpoints d'alerte

5. **Interface mobile**: 
   - Modifier HTML dans `handleRoot()`
   - Améliorer CSS responsive

## � Dépannage

### Problèmes de compilation:
```bash
# Nettoyer le projet
rm -rf .pio
pio run
```

### Problèmes de port série:
```bash
# Vérifier les ports
ls -la /dev/tty*
# Ajouter permissions
sudo usermod -a -G dialout $USER
```

### Problèmes WiFi:
- Vérifier SSID/password dans `config.h`
- Redémarrer ESP32-S3 (bouton RESET)
- Vérifier la portée WiFi

## 📚 Bibliothèques Utilisées

- **ArduinoJson@^6.21.3** - Parsing/génération JSON
- **AsyncTCP-esphome@^2.1.4** - TCP asynchrone ESP32
- **ESPAsyncWebServer-esphome@3.2.2** - Serveur web async

## 📞 Support

Pour toute question ou amélioration:
- Documentation PlatformIO: https://platformio.org/
- ESP32-S3 Datasheet: https://www.espressif.com/
- AsyncWebServer: https://github.com/me-no-dev/ESPAsyncWebServer

---
**Développé avec ❤️ pour le monitoring professionnel de carburant ESP32-S3**
