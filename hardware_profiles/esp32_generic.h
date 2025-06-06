/**
 * esp32_generic.h - Generic Hardware Profile Template
 * 
 * ============================================
 * HARDWARE PROFILE TEMPLATE & DOCUMENTATION
 * ============================================
 * 
 * Diese Datei dient als Vorlage für neue Hardware-Profile.
 * Kopieren Sie diese Datei und passen Sie die Werte an Ihre Hardware an.
 * 
 * VERWENDUNG:
 * 1. Kopiere diese Datei zu: esp32_[DEIN_HARDWARE_NAME].h
 * 2. Ändere alle HW_* Definitionen entsprechend deiner Hardware
 * 3. Füge das neue Profile zur hardware_hal.h hinzu
 * 4. Verwende: #define HARDWARE_PROFILE ESP32_DEIN_HARDWARE_NAME
 * 
 * BEISPIELE FÜR VERSCHIEDENE HARDWARE:
 * - ESP32-2432S028R (2.8" ILI9341)
 * - ESP32-3248S035R (3.5" ILI9488) 
 * - ESP32-4827S043R (4.3" ST7262)
 * - ESP32-8048S070C (7" ST7262)
 * - Custom Boards
 */

#ifndef ESP32_GENERIC_H
#define ESP32_GENERIC_H

// ============================================
// HARDWARE PROFILE INFORMATION
// ============================================

// *** SCHRITT 1: GRUNDLEGENDE PROFIL-INFORMATIONEN ***
#define HW_PROFILE_NAME "ESP32-Generic"
#define HW_PROFILE_VERSION "1.0"
#define HW_PROFILE_DESCRIPTION "Generic ESP32 Display Template"

// String-Versionen für Compile-Time Messages
#define HW_PROFILE_NAME_STR "ESP32-Generic"
#define HW_DISPLAY_WIDTH_STR "320"          // Als String für pragma
#define HW_DISPLAY_HEIGHT_STR "240"         // Als String für pragma
#define HW_DISPLAY_CONTROLLER_STR "ILI9341" // Controller als String
#define HW_TOUCH_CONTROLLER_STR "XPT2046"   // Touch als String

// ============================================
// DISPLAY CONFIGURATION
// ============================================

// *** SCHRITT 2: DISPLAY CONTROLLER FESTLEGEN ***
// Häufige Controller:
// - ST7789  (meist 240x320)
// - ILI9341 (meist 320x240)
// - ILI9488 (meist 320x480)
// - ST7262  (meist 800x480)
#define HW_DISPLAY_CONTROLLER ILI9341

// *** SCHRITT 3: DISPLAY-AUFLÖSUNG ***
// WICHTIG: Portrait vs Landscape berücksichtigen
// Portrait:  Schmaler x Höher (z.B. 240x320)
// Landscape: Breiter x Niedriger (z.B. 320x240)
#define HW_DISPLAY_WIDTH 320
#define HW_DISPLAY_HEIGHT 240

// *** SCHRITT 4: STANDARD-ORIENTIERUNG ***
// 0 = Portrait normal (USB oben)
// 1 = Landscape links (USB links)  
// 2 = Portrait invertiert (USB unten)
// 3 = Landscape rechts (USB rechts)
#define HW_DEFAULT_ROTATION 1    // Landscape für 320x240

// *** SCHRITT 5: FARB-EINSTELLUNGEN ***
// Manche Displays haben invertierte Farben
#define HW_COLORS_INVERTED false
#define HW_DISPLAY_INVERSION_OFF true

// *** SCHRITT 6: DISPLAY SPI PINS ***
// Standard ESP32 SPI Pins (anpassen falls nötig):
#define HW_DISPLAY_MISO 12    // Standard oder -1 wenn nicht verwendet
#define HW_DISPLAY_MOSI 13    // SPI Data Out
#define HW_DISPLAY_SCLK 14    // SPI Clock
#define HW_DISPLAY_CS 15      // Chip Select (SS)
#define HW_DISPLAY_DC 2       // Data/Command
#define HW_DISPLAY_RST -1     // Reset (-1 = mit Arduino Reset verbunden)

// Alternative Pin-Konfigurationen:
// ESP32-WROOM-32:   MISO=12, MOSI=13, SCLK=14
// ESP32-S3:         MISO=13, MOSI=11, SCLK=12
// Custom:           Beliebige GPIO-Pins

// *** SCHRITT 7: DISPLAY SPI EINSTELLUNGEN ***
#define HW_DISPLAY_DMA true               // DMA verwenden (Empfohlen)
#define HW_DISPLAY_SPI_FREQ 40000000      // 40MHz (ILI9341 Standard)
#define HW_DISPLAY_SPI_READ_FREQ 20000000 // 20MHz für Lesen

// Frequenz-Empfehlungen:
// ST7789:  80MHz (schnell)
// ILI9341: 40MHz (Standard)
// ILI9488: 27MHz (langsamer wegen 18-bit)
// ST7262:  20MHz (große Displays)

// ============================================
// BACKLIGHT CONFIGURATION
// ============================================

// *** SCHRITT 8: BACKLIGHT-STEUERUNG ***
#define HW_BACKLIGHT_PIN 21             // GPIO für Backlight
#define HW_BACKLIGHT_INVERTED false     // true = LOW = an, false = HIGH = an
#define HW_BACKLIGHT_PWM_CHANNEL 0      // LEDC Kanal für PWM
#define HW_BACKLIGHT_PWM_FREQ 5000      // PWM Frequenz (1kHz - 10kHz)
#define HW_BACKLIGHT_PWM_RESOLUTION 8   // 8-Bit = 0-255 Werte
#define HW_BACKLIGHT_DEFAULT 100        // Standard-Helligkeit %

// Backlight-Varianten:
// 1. PWM-Steuerung: Pin + PWM-Kanal (Helligkeit 0-100%)
// 2. Digital: Pin + HIGH/LOW (nur An/Aus)
// 3. Kein Backlight: Pins auskommentieren

// Beispiel für Digital-Backlight (ohne PWM):
// #define HW_BACKLIGHT_PIN 21
// #define HW_BACKLIGHT_INVERTED false
// // PWM-Definitionen weglassen für Digital-Modus

// ============================================
// TOUCH CONFIGURATION
// ============================================

// *** SCHRITT 9: TOUCH CONTROLLER ***
// Häufige Touch-Controller:
// - XPT2046 (Resistiv, Single-Touch)
// - GT911   (Kapazitiv, Multi-Touch)
// - FT6236  (Kapazitiv, Multi-Touch)
// - CST816S (Kapazitiv, Single-Touch)
#define HW_TOUCH_CONTROLLER XPT2046

// *** SCHRITT 10: TOUCH SPI BUS ***
// WICHTIG: ESP32 hat HSPI und VSPI
// HSPI = Standard (meist mit Display geteilt)
// VSPI = Separater Bus für Touch
#define HW_TOUCH_SPI_BUS VSPI           // VSPI oder HSPI

// *** SCHRITT 11: TOUCH MULTIPOINT ***
// Anzahl gleichzeitiger Touch-Punkte
#define HW_TOUCH_MULTIPOINT 1           // XPT2046 = 1, GT911 = 5

// *** SCHRITT 12: TOUCH SPI PINS ***
// Bei HSPI: Meist mit Display geteilt
// Bei VSPI: Separate Pins
#define HW_TOUCH_IRQ 36                 // Interrupt Pin
#define HW_TOUCH_MOSI 32                // SPI Data Out
#define HW_TOUCH_MISO 39                // SPI Data In  
#define HW_TOUCH_CLK 25                 // SPI Clock
#define HW_TOUCH_CS 33                  // Chip Select

// Touch Pin-Beispiele:
// ESP32-2432S028R (VSPI): MOSI=32, MISO=39, CLK=25, CS=33, IRQ=36
// ESP32-TZT-24 (HSPI):    MOSI=13, MISO=12, CLK=14, CS=33, IRQ=36
// Kapazitive Touch (I2C): SDA=21, SCL=22, IRQ=36, RST=25

// *** SCHRITT 13: TOUCH KALIBRIERUNG ***
// Diese Werte müssen durch Testing ermittelt werden!
#define HW_TOUCH_MIN_X 200              // Minimaler X-Rohwert
#define HW_TOUCH_MAX_X 3800             // Maximaler X-Rohwert
#define HW_TOUCH_MIN_Y 200              // Minimaler Y-Rohwert
#define HW_TOUCH_MAX_Y 3800             // Maximaler Y-Rohwert
#define HW_TOUCH_THRESHOLD 600          // Touch-Druckschwelle
#define HW_TOUCH_CALIBRATED false       // true = Kalibriert, false = Muss kalibriert werden

// Kalibrierungs-Anleitung:
// 1. Setze HW_TOUCH_CALIBRATED auf false
// 2. Aktiviere RAW_DEBUG in config.h
// 3. Berühre alle 4 Ecken des Displays
// 4. Notiere Min/Max X/Y Werte aus Serial
// 5. Trage Werte hier ein
// 6. Setze HW_TOUCH_CALIBRATED auf true

// *** SCHRITT 14: TOUCH INVERSION ***
// Manchmal sind Touch-Koordinaten gespiegelt
#define HW_TOUCH_INVERT_X false         // X-Achse spiegeln
#define HW_TOUCH_INVERT_Y false         // Y-Achse spiegeln

// *** SCHRITT 15: TOUCH SPI FREQUENZ ***
#define HW_TOUCH_SPI_FREQ 2500000       // 2.5MHz für XPT2046

// Touch Frequenz-Empfehlungen:
// XPT2046: 2.5MHz (Standard)
// GT911:   I2C 400kHz
// FT6236:  I2C 400kHz

// ============================================
// ADDITIONAL HARDWARE
// ============================================

// *** SCHRITT 16: RGB LED (OPTIONAL) ***
// Falls das Board RGB-LEDs hat
#define HW_LED_RED_PIN 4                // Roter LED Pin
#define HW_LED_GREEN_PIN 16             // Grüner LED Pin
#define HW_LED_BLUE_PIN 17              // Blauer LED Pin
#define HW_LED_INVERTED false           // true = Common Cathode, false = Common Anode

// Verschiedene LED-Konfigurationen:
// Einzelne LEDs:     Separate Pins
// WS2812 RGB:        Ein Data-Pin + Anzahl LEDs
// Keine LEDs:        Pins auskommentieren

// *** SCHRITT 17: RS485 KOMMUNIKATION (OPTIONAL) ***
#define HW_RS485_RX_PIN 22              // RS485 RX Pin
#define HW_RS485_TX_PIN 27              // RS485 TX Pin
#define HW_RS485_UART UART2             // UART Nummer
#define HW_RS485_BAUD 57600             // Baudrate

// *** SCHRITT 18: SD CARD (OPTIONAL) ***
// Falls SD-Card-Slot vorhanden
// #define HW_SD_CS_PIN 5               // SD Card Chip Select
// #define HW_SD_SPI_FREQ 25000000     // SD Card SPI Frequenz

// *** SCHRITT 19: AUDIO (OPTIONAL) ***
// Falls Lautsprecher/Buzzer vorhanden
// #define HW_SPEAKER_PIN 26            // Speaker/Buzzer Pin
// #define HW_SPEAKER_CHANNEL 1         // PWM Kanal für Töne

// ============================================
// ORIENTATION MAPPINGS
// ============================================

// *** SCHRITT 20: ORIENTIERUNGS-ZUORDNUNG ***
// Diese Zuordnung hängt vom Display-Controller ab
#define HW_ROTATION_PORTRAIT_NORMAL    0  // 240x320 oder 320x480
#define HW_ROTATION_LANDSCAPE_LEFT     1  // 320x240 oder 480x320
#define HW_ROTATION_PORTRAIT_INVERTED  2  // Wie 0, aber gedreht
#define HW_ROTATION_LANDSCAPE_RIGHT    3  // Wie 1, aber gedreht

// Controller-spezifische Rotation:
// ST7789:  0=Portrait, 1=Landscape, 2=Portrait inv, 3=Landscape inv
// ILI9341: 0=Portrait, 1=Landscape, 2=Portrait inv, 3=Landscape inv
// ILI9488: Ähnlich, aber andere Auflösung

// ============================================
// FEATURE FLAGS
// ============================================

// *** SCHRITT 21: VERFÜGBARE FEATURES ***
#define HW_HAS_BACKLIGHT_CONTROL true   // Backlight steuerbar
#define HW_HAS_PWM_BACKLIGHT true       // PWM oder nur Digital
#define HW_HAS_RGB_LED true             // RGB LEDs vorhanden
#define HW_HAS_RS485 true               // RS485 Interface
#define HW_HAS_TOUCH true               // Touch vorhanden
#define HW_HAS_MULTITOUCH false         // Multi-Touch fähig
#define HW_HAS_SD_CARD false            // SD-Karte vorhanden
#define HW_HAS_SPEAKER false            // Audio-Ausgabe vorhanden
#define HW_HAS_WIFI true                // WiFi verfügbar (Standard ESP32)
#define HW_HAS_BLUETOOTH true           // Bluetooth verfügbar

// Feature-Flags helfen beim:
// - Conditional Compilation
// - Web-Interface Anpassung
// - Diagnostics

// ============================================
// TFT_ESPI USER_SETUP MAPPING
// ============================================

// *** SCHRITT 22: TFT_ESPI KOMPATIBILITÄT ***
// Diese Werte werden für automatische User_Setup.h verwendet
#define TFT_ESPI_DRIVER "ILI9341_DRIVER"
#define TFT_ESPI_WIDTH HW_DISPLAY_WIDTH
#define TFT_ESPI_HEIGHT HW_DISPLAY_HEIGHT
#define TFT_ESPI_MISO HW_DISPLAY_MISO
#define TFT_ESPI_MOSI HW_DISPLAY_MOSI
#define TFT_ESPI_SCLK HW_DISPLAY_SCLK
#define TFT_ESPI_CS HW_DISPLAY_CS
#define TFT_ESPI_DC HW_DISPLAY_DC
#define TFT_ESPI_RST HW_DISPLAY_RST
#define TFT_ESPI_SPI_PORT "HSPI"        // oder "VSPI"
#define TFT_ESPI_SPI_FREQ HW_DISPLAY_SPI_FREQ

// Driver-Namen für verschiedene Controller:
// "ST7789_DRIVER"
// "ILI9341_DRIVER" 
// "ILI9488_DRIVER"
// "ST7262_DRIVER"

// ============================================
// HARDWARE-SPECIFIC INITIALIZATION CODE
// ============================================

// *** SCHRITT 23: HARDWARE-SPEZIFISCHE INITIALISIERUNG ***
#define HW_INIT_CODE() do { \
  /* Hier kommt hardware-spezifischer Code */ \
  Serial.println("Generic Hardware Init"); \
  /* Beispiele: */ \
  /* - Spezielle Register setzen */ \
  /* - GPIO-Modi konfigurieren */ \
  /* - Power Management */ \
} while(0)

// *** SCHRITT 24: TOUCH-SPEZIFISCHE INITIALISIERUNG ***
#define HW_TOUCH_INIT_CODE() do { \
  Serial.println("Generic Touch Init"); \
  /* Touch-spezifische Initialisierung */ \
  /* Beispiele: */ \
  /* - Touch-Controller Reset */ \
  /* - Kalibrierung laden */ \
  /* - Sensitivity einstellen */ \
} while(0)

// ============================================
// BOARD-SPECIFIC COORDINATE MAPPING
// ============================================

// *** SCHRITT 25: KOORDINATEN-TRANSFORMATION ***
// Hardware-spezifisches Touch-Mapping für Landscape
#define HW_TOUCH_MAP_LANDSCAPE(rawX, rawY, mappedX, mappedY) do { \
  /* Standard XPT2046 Landscape Mapping */ \
  mappedX = map(rawY, HW_TOUCH_MIN_Y, HW_TOUCH_MAX_Y, 0, HW_DISPLAY_WIDTH); \
  mappedY = map(rawX, HW_TOUCH_MAX_X, HW_TOUCH_MIN_X, 0, HW_DISPLAY_HEIGHT); \
} while(0)

// Hardware-spezifisches Touch-Mapping für Portrait
#define HW_TOUCH_MAP_PORTRAIT(rawX, rawY, mappedX, mappedY) do { \
  /* Standard XPT2046 Portrait Mapping */ \
  mappedX = map(rawX, HW_TOUCH_MIN_X, HW_TOUCH_MAX_X, 0, HW_DISPLAY_WIDTH); \
  mappedY = map(rawY, HW_TOUCH_MIN_Y, HW_TOUCH_MAX_Y, 0, HW_DISPLAY_HEIGHT); \
} while(0)

// Verschiedene Touch-Mapping-Beispiele:
// XPT2046:     Raw X/Y → Display X/Y mit Rotation
// Kapazitiv:   Oft direkte Zuordnung
// GT911:       Multi-Point Koordinaten

// ============================================
// ADVANCED CONFIGURATION
// ============================================

// *** SCHRITT 26: ERWEITERTE KONFIGURATION ***

// Power Management
// #define HW_POWER_ENABLE_PIN 4        // Board Power Enable
// #define HW_POWER_INVERTED false

// External Flash/PSRAM
// #define HW_HAS_PSRAM true            // PSRAM verfügbar
// #define HW_HAS_EXTERNAL_FLASH true   // Externes Flash

// USB/UART
// #define HW_USB_UART_TX 43            // USB-UART TX
// #define HW_USB_UART_RX 44            // USB-UART RX

// I2C für Sensoren/RTC
// #define HW_I2C_SDA 21                // I2C Data
// #define HW_I2C_SCL 22                // I2C Clock
// #define HW_I2C_FREQ 400000           // I2C Frequenz

// ============================================
// VALIDATION & BOARD DETECTION
// ============================================

// *** SCHRITT 27: HARDWARE-VALIDIERUNG ***
// Board-spezifische Konsistenz-Prüfungen
#if HW_DISPLAY_WIDTH < 240 || HW_DISPLAY_HEIGHT < 240
  #warning "Sehr kleine Display-Auflösung - Touch-Interface eventuell schwer bedienbar"
#endif

#if HW_TOUCH_SPI_BUS == HSPI && HW_DISPLAY_MOSI == HW_TOUCH_MOSI
  #pragma message "Info: Touch und Display teilen sich SPI-Bus (HSPI)"
#endif

#if HW_TOUCH_MULTIPOINT > 1 && HW_TOUCH_CONTROLLER == XPT2046
  #error "XPT2046 unterstützt nur Single-Touch (HW_TOUCH_MULTIPOINT = 1)"
#endif

// *** SCHRITT 28: BOARD-ERKENNUNG (OPTIONAL) ***
// Code zur automatischen Hardware-Erkennung
#define HW_BOARD_DETECTION_CODE() ({ \
  bool detected = false; \
  /* Hardware-Detection Logic */ \
  /* Beispiele: */ \
  /* - GPIO-Pin testen */ \
  /* - I2C Device scannen */ \
  /* - SPI Response prüfen */ \
  detected; \
})

// ============================================
// DOCUMENTATION & EXAMPLES
// ============================================

/*
 * *** KONFIGURATIONSBEISPIELE FÜR HÄUFIGE BOARDS ***
 * 
 * ESP32-2432S028R (2.8" ILI9341):
 * - Display: 320x240, ILI9341
 * - Touch: XPT2046 auf VSPI
 * - Pins: Touch MOSI=32, MISO=39, CLK=25
 * 
 * ESP32-3248S035R (3.5" ILI9488):
 * - Display: 320x480, ILI9488  
 * - Touch: XPT2046 auf VSPI
 * - Backlight: Pin 27
 * 
 * ESP32-4827S043R (4.3" ST7262):
 * - Display: 800x480, ST7262
 * - Touch: GT911 kapazitiv auf I2C
 * - RGB Interface statt SPI
 * 
 * ESP32-8048S070C (7" ST7262):
 * - Display: 800x480, ST7262
 * - Touch: GT911 kapazitiv  
 * - I2S Audio, SD-Card
 * 
 * *** TOUCH-KALIBRIERUNG SCHRITT-FÜR-SCHRITT ***
 * 
 * 1. RAW_DEBUG aktivieren:
 *    #define RAW_DEBUG 1
 * 
 * 2. Touch-Test kompilieren und starten
 * 
 * 3. Alle 4 Display-Ecken berühren:
 *    - Oben Links
 *    - Oben Rechts  
 *    - Unten Rechts
 *    - Unten Links
 * 
 * 4. Min/Max Werte aus Serial Monitor notieren:
 *    Raw X: 300-3700 → MIN_X=300, MAX_X=3700
 *    Raw Y: 400-3800 → MIN_Y=400, MAX_Y=3800
 * 
 * 5. Werte in Hardware-Profile eintragen
 * 
 * 6. Testen und bei Bedarf fine-tunen
 * 
 * *** COMMON ISSUES & TROUBLESHOOTING ***
 * 
 * Problem: Display bleibt schwarz
 * Lösung: - Reset-Pin prüfen (RST)
 *         - SPI-Pins prüfen
 *         - Power/Backlight prüfen
 * 
 * Problem: Touch reagiert nicht
 * Lösung: - IRQ-Pin prüfen
 *         - SPI-Bus (HSPI vs VSPI)
 *         - Touch-Controller Reset
 * 
 * Problem: Touch-Koordinaten falsch
 * Lösung: - Kalibrierung wiederholen
 *         - Inversion-Flags prüfen
 *         - Rotation vs Touch-Mapping
 * 
 * Problem: Farben invertiert
 * Lösung: - HW_COLORS_INVERTED umschalten
 *         - Display-Controller prüfen
 * 
 * Problem: Backlight funktioniert nicht
 * Lösung: - Pin prüfen
 *         - Inversion-Flag prüfen
 *         - PWM vs Digital
 */

#endif // ESP32_GENERIC_H