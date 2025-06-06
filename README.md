# HardwareManager für ESP32 mit ILI9341/ST7789 und XPT2046 Touch

Dieses Projekt stellt eine flexible Hardware-Abstraktionsschicht für ESP32-basierte Systeme mit TFT-Displays (ILI9341 oder ST7789) und Touch-Controller (XPT2046) bereit. Es unterstützt verschiedene Hardware-Profile und ermöglicht eine einfache Anpassung an unterschiedliche Boards und Display-Module.

## Features

- **Dynamische Display-Konfiguration** für ILI9341 und ST7789
- **Unterstützung für verschiedene Hardware-Profile** (z.B. ESP32_2432S028R)
- **Flexible Touch-Anpassung** inklusive Achsen-Invertierung und Rotations-Mapping
- **Backlight-Steuerung** (PWM und digital)
- **Automatische Initialisierung** von Display, Touch und Backlight
- **Einfache Erweiterbarkeit** für weitere Hardware-Profile

## Voraussetzung

- ESP32
- TFT_eSPI Bibliothek
- XPT2046_Touchscreen Bibliothek
- Passendes Display (ILI9341 oder ST7789) mit Touch (XPT2046)

## Aufbau

- `hardware_manager.cpp` / `.h`: Zentrale Hardware-Abstraktion und Initialisierung
- `TFT_Setup.h`: Hardware-abhängige Definitionen und Makros
- `config.h` / `hardware_hal.h`: Weitere Konfigurationen und Hardware-Profile

## Konfiguration

1. **Hardware-Profil wählen/erstellen:**  
   In `config.h` das gewünschte Profil auswählen oder ein neues Profil in den Hardware-Headern definieren.

2. **Display- und Touch-Pins sowie Profile anpassen:**  
   In `TFT_Setup.h` und dem jeweiligen Hardware-Profil die Pinbelegung, Displaygröße, Touch-Mappings und -Invertierungen setzen.

3. **Display-Treiber wählen:**  
   Je nach Display in `TFT_Setup.h` den passenden Treiber aktivieren:
   - `#define ILI9341_2_DRIVER` für viele China-ILI9341
   - `#define ST7789_DRIVER` für ST7789-Displays

4. **Optional:** Backlight-Steuerung aktivieren und anpassen.

## Beispiel: Hardware-Profil für ESP32_2432S028R

```c
#define HW_DISPLAY_CONTROLLER     ILI9341
#define HW_DISPLAY_WIDTH         320
#define HW_DISPLAY_HEIGHT        240
#define HW_DEFAULT_ROTATION      1    // Landscape

#define HW_TOUCH_MIN_X           200
#define HW_TOUCH_MAX_X           3900
#define HW_TOUCH_MIN_Y           200
#define HW_TOUCH_MAX_Y           3900

#define HW_TOUCH_INVERT_X        false
#define HW_TOUCH_INVERT_Y        true
```

## Touch-Mapping

Die Touch-Koordinaten werden abhängig von der Display-Rotation und Hardware-Konfiguration automatisch angepasst.  
Invertierungen und Mapping-Makros können pro Profil und Rotation gesetzt werden.

## Backlight-Steuerung

Unterstützt PWM- und digitale Ansteuerung.  
Die Einstellungen werden im Hardware-Profil festgelegt.

## Aufbau/Verkabelung

- **Display:** Je nach Modell an die entsprechenden SPI-Pins des ESP32 anschließen.
- **Touch:** XPT2046 ebenfalls an SPI (oft getrenntes CS & IRQ).
- **Backlight:** Optionaler Pin für Helligkeitsregelung.
# ESP32 Hardware-Kommissionierung & Test-Tool

Dieses eigenständige Programm dient zur schnellen Inbetriebnahme, zum Testen und zur Kalibrierung von ESP32 Touch-Displays (z.B. ILI9341/ST7789 + XPT2046).  
Du kannst damit sämtliche Display- und Touchfunktionen sowie Backlight und Rotation prüfen – direkt per Menü über den Seriellen Monitor.

---

## **Schnellstart**

1. **Hardware-Profil anpassen:**  
   In der Datei ganz oben `#define HARDWARE_PROFILE ESP32_DEIN_PROFILE` setzen (Profil siehe `config.h`/`hardware_hal.h`).

2. **Kompilieren & Flashen:**  
   Programm als `hardware_commissioning.ino` in die Arduino IDE laden, Board & Port wählen, hochladen.

3. **Seriellen Monitor öffnen:**  
   Baudrate auf 115200 stellen.

4. **Test-Menü nutzen:**  
   Per Tastendruck (1-9, 0) gewünschte Tests starten, Anweisungen im Terminal und auf dem Display folgen.

---

## **Test-Menü Übersicht**

| Taste | Test-Modus                     | Beschreibung                                                   |
|-------|-------------------------------|----------------------------------------------------------------|
| 1     | Display Test                  | Grundlegende Display-Ausgabe und Geometrie                     |
| 2     | Farb Test                     | Normale & invertierte Farben, RGB-Komponenten                  |
| 3     | Backlight Test                | Helligkeitsregelung (PWM/Digital)                              |
| 4     | Single Touch Test             | Einzel-Touch visualisieren und Rohdaten anzeigen               |
| 5     | Multi Touch Test              | Mehrere Touchpunkte (sofern Hardware unterstützt)              |
| 6     | Touch Kalibrierung            | Sammelt Min/Max-Rohwerte, gibt Profil-Defines für Mapping aus  |
| 7     | Orientierungs Test            | Testet alle Display-Rotationen und zeigt Markierungen/Ecken    |
| 8     | Stress Test                   | Viele schnelle Grafikoperationen zur Stabilitätsprüfung        |
| 9     | Hardware Info                 | Zeigt alle Profil- und Systeminfos im Terminal                 |
| 0     | Menü erneut anzeigen          | Zeigt das Hauptmenü erneut an                                  |
| q     | Test beenden                  | Bricht aktuellen Test ab und kehrt zum Menü zurück             |

---

## **Bedienung & Hinweise**

- Die Testauswahl erfolgt durch Tastendruck im Seriellen Monitor.
- Während eines Tests kannst du mit 'q' jederzeit abbrechen.
- Bei der **Touch-Kalibrierung** möglichst alle Ecken und die Mitte mehrfach berühren. Nach 30s werden die Werte als `#define`-Zeilen ausgegeben – diese kannst du direkt ins Hardware-Profil übernehmen!
- **Backlight-Test:** Die Helligkeit wird automatisch hoch- und runtergeregelt, der aktuelle Wert wird angezeigt.
- **Orientierungs-Test:** Nacheinander werden alle vier Rotationen gezeigt, mit farbigen Markern in den Ecken. So erkennst du, wie Touch und Anzeige zusammenpassen.
- **Stress-Test:** Führt viele zufällige Grafikoperationen aus. Nutzbar für Dauer- und Stabilitätstests.

---

## **Typische Anpassungen**

1. **Profil:**  
   Passe das Hardware-Profil in der Sketch-Datei oder in den Headern an, damit Pins, Displaygröße und Touch-Mapping stimmen.

2. **Touch-Kalibrierung:**  
   Nach dem Kalibrier-Test die ausgegebenen Werte in dein Profil übernehmen:
   ```c
   #define HW_TOUCH_MIN_X ...
   #define HW_TOUCH_MAX_X ...
   #define HW_TOUCH_MIN_Y ...
   #define HW_TOUCH_MAX_Y ...
   ```

3. **Mapping/Invertierung:**  
   Falls Touch und Anzeige gespiegelt sind: Die Invertierungs-Makros (`HW_TOUCH_INVERT_X`, `HW_TOUCH_INVERT_Y`) im Profil anpassen und erneut testen.

---

## **Problemlösung**

- **Display bleibt schwarz:** Verkabelung, Pin-Defines und Display-Treiber prüfen.
- **Touch reagiert nicht:** SPI-Pins, CS/IRQ, Touch-Defines und Mapping kontrollieren.
- **Falsche Ausrichtung:** Rotations- und Invertierungs-Makros im Profil ändern, ggf. Touch-Kalibrierung erneut durchführen.

## Lizenz

Dieses Projekt steht unter der MIT-Lizenz.

## Hinweise & Support

- Für neue Hardware-Profile einfach die bestehenden Header als Vorlage nutzen.
- Bei Problemen mit Touch-Mapping oder Display-Anzeige: Profil und Invertierungen prüfen.
- Fragen oder Beiträge bitte als Issue einstellen.

---
