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

## Lizenz

Dieses Projekt steht unter der MIT-Lizenz.

## Hinweise & Support

- Für neue Hardware-Profile einfach die bestehenden Header als Vorlage nutzen.
- Bei Problemen mit Touch-Mapping oder Display-Anzeige: Profil und Invertierungen prüfen.
- Fragen oder Beiträge bitte als Issue einstellen.

---
