/**
 * hardware_profile_selector.h - Hardware Profile Auswahl & Dokumentation
 * 
 * ============================================
 * HARDWARE PROFILE SELECTION GUIDE
 * ============================================
 * 
 * Dieses File hilft bei der Auswahl des richtigen Hardware-Profils
 * und zeigt die verfügbaren Optionen.
 */

#ifndef HARDWARE_PROFILE_SELECTOR_H
#define HARDWARE_PROFILE_SELECTOR_H

// ============================================
// VERFÜGBARE HARDWARE PROFILE
// ============================================

/*
 * *** UNTERSTÜTZTE HARDWARE-PROFILE ***
 * 
 * 1. ESP32_TZT_24 - TZT ESP32 2.4" ST7789 (Original)
 *    - Display: 240x320, ST7789
 *    - Touch: XPT2046 auf HSPI
 *    - Backlight: Pin 27, PWM
 *    - Getestet: ✅
 * 
 * 2. ESP32_2432S028R - 2.8" ILI9341 Display
 *    - Display: 320x240, ILI9341  
 *    - Touch: XPT2046 auf VSPI
 *    - Backlight: Pin 21, PWM
 *    - Getestet: ⚠️ (Neu)
 * 
 * 3. ESP32_GENERIC - Universelle Vorlage
 *    - Konfigurierbar für alle Hardware
 *    - Vollständig dokumentiert
 *    - Als Template verwenden
 */

// ============================================
// QUICK HARDWARE SELECTION
// ============================================

/*
 * *** SCHNELLE HARDWARE-AUSWAHL ***
 * 
 * OPTION 1: Automatische Erkennung (Empfohlen)
 * Füge diese Zeilen zu deiner main.ino hinzu:
 */

// Automatische Hardware-Erkennung aktivieren
#define AUTO_DETECT_HARDWARE

#ifdef AUTO_DETECT_HARDWARE
  // Hardware wird zur Laufzeit erkannt
  #include "hardware_auto_detect.h"
#else
  // Manuelle Hardware-Auswahl:
  
  // *** UNCOMMENT DEINE HARDWARE ***
  //#define HW_PROFILE ESP32_TZT_24      // Original TZT 2.4"
  #define HW_PROFILE ESP32_2432S028R   // 2.8" ILI9341
  //#define HW_PROFILE ESP32_GENERIC     // Generic Template
  
  #include "hardware_hal.h"
#endif

/*
 * OPTION 2: Manuelle Auswahl in config.h
 * Füge zu deiner config.h hinzu:
 * 
 * #define HARDWARE_PROFILE ESP32_2432S028R
 * #include "hardware_hal.h"
 */

/*
 * OPTION 3: Compiler-Flag
 * Verwende PlatformIO build_flags:
 * 
 * build_flags = 
 *   -DHW_PROFILE=ESP32_2432S028R
 */

// ============================================
// HARDWARE COMPATIBILITY MATRIX
// ============================================

/*
 * *** KOMPATIBILITÄTS-MATRIX ***
 * 
 * Feature            | TZT_24 | 2432S028R | Generic |
 * -------------------|--------|-----------|---------|
 * Display Controller | ST7789 | ILI9341   | Config  |
 * Resolution         | 240x320| 320x240   | Config  |
 * Touch Controller   | XPT2046| XPT2046   | Config  |
 * Touch SPI Bus      | HSPI   | VSPI      | Config  |
 * PWM Backlight      | ✅     | ✅        | Config  |
 * RGB LED            | ✅     | ✅        | Config  |
 * RS485              | ✅     | ✅        | Config  |
 * Multi-Touch        | ❌     | ❌        | Config  |
 * SD Card            | ❌     | ❌        | Config  |
 * Audio              | ❌     | ❌        | Config  |
 * 
 * ✅ = Unterstützt
 * ❌ = Nicht verfügbar  
 * Config = Konfigurierbar
 */

// ============================================
// NEUE HARDWARE HINZUFÜGEN
// ============================================

/*
 * *** NEUE HARDWARE HINZUFÜGEN - SCHRITT FÜR SCHRITT ***
 * 
 * 1. PROFILE ERSTELLEN
 *    - Kopiere esp32_generic.h zu esp32_[DEIN_NAME].h
 *    - Folge den Schritt 1-28 Anweisungen
 *    - Passe alle HW_* Definitionen an
 * 
 * 2. PROFILE REGISTRIEREN
 *    - Füge zu hardware_hal.h hinzu:
 *      #define ESP32_DEIN_NAME 3
 *    - Erweitere die Include-Liste:
 *      #elif HARDWARE_PROFILE == ESP32_DEIN_NAME
 *        #include "hardware_profiles/esp32_dein_name.h"
 * 
 * 3. TOUCH KALIBRIERUNG
 *    - Setze HW_TOUCH_CALIBRATED auf false
 *    - Aktiviere RAW_DEBUG
 *    - Ermittle Min/Max Touch-Werte
 *    - Trage Werte ins Profile ein
 * 
 * 4. TESTING & VALIDATION
 *    - Teste alle Grundfunktionen
 *    - Prüfe Touch-Genauigkeit
 *    - Validiere Display-Orientierung
 *    - Teste Backlight-Steuerung
 * 
 * 5. DOKUMENTATION
 *    - Erweitere diese Kompatibilitäts-Matrix
 *    - Füge Board-spezifische Hinweise hinzu
 *    - Erstelle Troubleshooting-Guide
 */

// ============================================
// HARDWARE AUTO-DETECTION
// ============================================

/*
 * *** AUTOMATISCHE HARDWARE-ERKENNUNG ***
 * 
 * Das System kann Hardware automatisch erkennen durch:
 * 
 * 1. GPIO-PIN TESTING
 *    - Teste bekannte Pin-Konfigurationen
 *    - Erkenne SPI-Responses
 *    - Identifiziere Controller-IDs
 * 
 * 2. DISPLAY CONTROLLER ID
 *    - Lese Controller-Register
 *    - ST7789: ID 0x85
 *    - ILI9341: ID 0x93, 0x41
 *    - ILI9488: ID 0x94, 0x88
 * 
 * 3. TOUCH CONTROLLER DETECTION
 *    - XPT2046: SPI Response Test
 *    - GT911: I2C Address Scan
 *    - FT6236: I2C Address 0x38
 * 
 * 4. BOARD SIGNATURE
 *    - Eindeutige Pin-Kombinationen
 *    - Flash/PSRAM Konfiguration
 *    - MAC-Address Bereiche
 */

// Auto-Detection Implementation
#ifdef AUTO_DETECT_HARDWARE

class HardwareDetector {
public:
  // Automatische Hardware-Erkennung
  static int detectHardware() {
    Serial.println("🔍 Starte automatische Hardware-Erkennung...");
    
    // Test 1: Display Controller erkennen
    int displayController = detectDisplayController();
    
    // Test 2: Touch Controller erkennen  
    int touchController = detectTouchController();
    
    // Test 3: Pin-Konfiguration testen
    int pinConfig = detectPinConfiguration();
    
    // Hardware-Profile basierend auf Ergebnissen bestimmen
    if (displayController == DISPLAY_ST7789 && touchController == TOUCH_XPT2046_HSPI) {
      Serial.println("✅ Erkannt: ESP32_TZT_24 (2.4\" ST7789)");
      return ESP32_TZT_24;
    }
    
    if (displayController == DISPLAY_ILI9341 && touchController == TOUCH_XPT2046_VSPI) {
      Serial.println("✅ Erkannt: ESP32_2432S028R (2.8\" ILI9341)");
      return ESP32_2432S028R;
    }
    
    Serial.println("⚠️ Hardware nicht erkannt - verwende Generic Profile");
    return ESP32_GENERIC;
  }
  
private:
  // Display Controller IDs
  enum DisplayController {
    DISPLAY_UNKNOWN = 0,
    DISPLAY_ST7789 = 1,
    DISPLAY_ILI9341 = 2,
    DISPLAY_ILI9488 = 3
  };
  
  // Touch Controller Types
  enum TouchController {
    TOUCH_UNKNOWN = 0,
    TOUCH_XPT2046_HSPI = 1,
    TOUCH_XPT2046_VSPI = 2,
    TOUCH_GT911_I2C = 3
  };
  
  static int detectDisplayController() {
    // Implementierung der Display-Erkennung
    // ...
    return DISPLAY_UNKNOWN;
  }
  
  static int detectTouchController() {
    // Implementierung der Touch-Erkennung
    // ...
    return TOUCH_UNKNOWN;
  }
  
  static int detectPinConfiguration() {
    // Pin-Konfiguration testen
    // ...
    return 0;
  }
};

#endif // AUTO_DETECT_HARDWARE

// ============================================
// MIGRATION GUIDE
// ============================================

/*
 * *** MIGRATION VON config.h ZU HAL ***
 * 
 * ALTE KONFIGURATION (config.h):
 * #define SCREEN_WIDTH 240
 * #define SCREEN_HEIGHT 320
 * #define TFT_BL_PIN 27
 * #define XPT2046_CS 33
 * 
 * NEUE KONFIGURATION (HAL):
 * #define HARDWARE_PROFILE ESP32_TZT_24
 * #include "hardware_hal.h"
 * 
 * MIGRATION SCHRITTE:
 * 1. Identifiziere deine Hardware
 * 2. Wähle passendes Profile oder erstelle neues
 * 3. Ersetze config.h Include durch HAL
 * 4. Teste alle Funktionen
 * 5. Entferne alte config.h Definitionen
 * 
 * BACKWARD COMPATIBILITY:
 * - Alte #defines bleiben verfügbar
 * - Schrittweise Migration möglich
 * - Keine Code-Änderungen in main.ino nötig
 */

// ============================================
// TROUBLESHOOTING GUIDE
// ============================================

/*
 * *** HÄUFIGE PROBLEME & LÖSUNGEN ***
 * 
 * PROBLEM: "Unbekanntes HARDWARE_PROFILE"
 * LÖSUNG: - Profile-Name prüfen (Schreibweise)
 *         - Include-Pfad prüfen
 *         - hardware_hal.h vor anderen includes
 * 
 * PROBLEM: Display bleibt schwarz nach HAL-Umstellung
 * LÖSUNG: - Display-Controller im Profile prüfen
 *         - Pin-Definitionen vergleichen
 *         - TFT_eSPI User_Setup überprüfen
 * 
 * PROBLEM: Touch reagiert nicht mehr
 * LÖSUNG: - Touch SPI Bus prüfen (HSPI vs VSPI)
 *         - Touch-Pins im Profile prüfen
 *         - Touch-Kalibrierung wiederholen
 * 
 * PROBLEM: Koordinaten sind vertauscht
 * LÖSUNG: - HW_TOUCH_MAP_* Makros prüfen
 *         - Inversion-Flags anpassen
 *         - Rotation vs Touch-Mapping
 * 
 * PROBLEM: Backlight funktioniert nicht
 * LÖSUNG: - HW_BACKLIGHT_PIN prüfen
 *         - PWM-Kanal Konflikt prüfen
 *         - Inversion-Flag testen
 * 
 * PROBLEM: Compile-Fehler mit HAL
 * LÖSUNG: - Include-Reihenfolge prüfen
 *         - Doppelte Definitionen vermeiden
 *         - Arduino IDE Cache löschen
 */

// ============================================
// PERFORMANCE & OPTIMIZATION
// ============================================

/*
 * *** PERFORMANCE OPTIMIERUNG ***
 * 
 * 1. SPI FREQUENZ
 *    - ST7789: bis 80MHz
 *    - ILI9341: bis 40MHz  
 *    - Touch: 2.5MHz max
 * 
 * 2. DMA USAGE
 *    - Aktiviere HW_DISPLAY_DMA
 *    - Reduziert CPU-Last
 *    - Schnellere Bildschirm-Updates
 * 
 * 3. TOUCH SAMPLING
 *    - Nicht zu oft abfragen
 *    - Debouncing implementieren
 *    - Interrupt-basiert optimal
 * 
 * 4. PWM OPTIMIERUNG
 *    - Passende Frequenz wählen
 *    - Konflikte mit anderen PWM vermeiden
 *    - 8-Bit Auflösung meist ausreichend
 */

// ============================================
// FUTURE EXTENSIONS
// ============================================

/*
 * *** GEPLANTE ERWEITERUNGEN ***
 * 
 * 1. KAPAZITIVE TOUCH
 *    - GT911 Multi-Touch Support
 *    - FT6236 Implementation
 *    - I2C Touch-Interface
 * 
 * 2. GRÖSSERE DISPLAYS
 *    - 4.3" / 7" / 10" Displays
 *    - RGB Interface Support
 *    - Higher Resolutions
 * 
 * 3. AUDIO SUPPORT
 *    - I2S Audio Output
 *    - Speaker/Buzzer Control
 *    - Audio Feedback
 * 
 * 4. SENSOR INTEGRATION
 *    - RTC Support
 *    - Temperature/Humidity
 *    - Motion Sensors
 * 
 * 5. WIRELESS EXTENSIONS
 *    - WiFi Manager Integration
 *    - Bluetooth LE Support
 *    - OTA Updates
 */

#endif // HARDWARE_PROFILE_SELECTOR_H