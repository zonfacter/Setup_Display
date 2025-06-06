/**
 * esp32_2432s028r.h - Hardware Profile für ESP32-2432S028R 2,8"
 * 
 * 2,8" ILI9341 Display mit XPT2046 Touch (VSPI)
 */

#ifndef ESP32_2432S028R_H
#define ESP32_2432S028R_H

// ============================================
// HARDWARE PROFILE INFORMATION
// ============================================

#define HW_PROFILE_NAME "ESP32-2432S028R"
#define HW_PROFILE_VERSION "1.0"
#define HW_PROFILE_DESCRIPTION "ESP32-2432S028R 2.8 inch ILI9341 Display"

// String-Versionen für Pragma Messages
#define HW_PROFILE_NAME_STR "ESP32-2432S028R"
#define HW_DISPLAY_WIDTH_STR "320"
#define HW_DISPLAY_HEIGHT_STR "240"
#define HW_DISPLAY_CONTROLLER_STR "ILI9341"
#define HW_TOUCH_CONTROLLER_STR "XPT2046"

// ============================================
// DISPLAY CONFIGURATION
// ============================================

#define HW_DISPLAY_CONTROLLER ILI9341
#define HW_DISPLAY_WIDTH 320
#define HW_DISPLAY_HEIGHT 240
#define HW_DEFAULT_ROTATION 0   // Landscape (Standard für 2,8")
#define HW_COLORS_INVERTED false
#define HW_DISPLAY_INVERSION_OFF false

// Display SPI Pins (Standard ESP32-2432S028R)
#define HW_DISPLAY_MISO 12
#define HW_DISPLAY_MOSI 13
#define HW_DISPLAY_SCLK 14
#define HW_DISPLAY_CS 15
#define HW_DISPLAY_DC 2
#define HW_DISPLAY_RST -1

// Display Features
#define HW_DISPLAY_DMA true
#define HW_DISPLAY_SPI_FREQ 40000000
#define HW_DISPLAY_SPI_READ_FREQ 20000000

// ============================================
// BACKLIGHT CONFIGURATION
// ============================================

#define HW_BACKLIGHT_PIN 21
#define HW_BACKLIGHT_INVERTED false
#define HW_BACKLIGHT_PWM_CHANNEL 0
#define HW_BACKLIGHT_PWM_FREQ 5000
#define HW_BACKLIGHT_PWM_RESOLUTION 8
#define HW_BACKLIGHT_DEFAULT 100

// ============================================
// TOUCH CONFIGURATION
// ============================================

#define HW_TOUCH_CONTROLLER XPT2046
#define HW_TOUCH_SPI_BUS VSPI  // *** WICHTIG: VSPI statt HSPI ***
#define HW_TOUCH_MULTIPOINT 1  // Single Touch

// Touch SPI Pins (ESP32-2432S028R spezifisch)
#define HW_TOUCH_IRQ 36
#define HW_TOUCH_MOSI 32  // *** ANDERS als TZT Display ***
#define HW_TOUCH_MISO 39  // *** ANDERS als TZT Display ***
#define HW_TOUCH_CLK 25   // *** ANDERS als TZT Display ***
#define HW_TOUCH_CS 33

// Touch Calibration (muss eventuell angepasst werden)
#define HW_TOUCH_MIN_X 320
#define HW_TOUCH_MAX_X 3773
#define HW_TOUCH_MIN_Y 376
#define HW_TOUCH_MAX_Y 3743
#define HW_TOUCH_THRESHOLD 600
#define HW_TOUCH_CALIBRATED true  // Jetzt kalibriert!
#define HW_TOUCH_INVERT_X false
#define HW_TOUCH_INVERT_Y false

// Touch SPI Frequency
#define HW_TOUCH_SPI_FREQ 2500000

// ============================================
// ADDITIONAL HARDWARE
// ============================================

// RGB LED Pins (falls vorhanden - Board-abhängig)
#define HW_LED_RED_PIN 4
#define HW_LED_GREEN_PIN 16
#define HW_LED_BLUE_PIN 17
#define HW_LED_INVERTED false

// RS485 Communication (gleiche Pins wie TZT)
#define HW_RS485_RX_PIN 22
#define HW_RS485_TX_PIN 27  // *** Eventuell anders ***
#define HW_RS485_UART UART2
#define HW_RS485_BAUD 57600

// ============================================
// ORIENTATION MAPPINGS
// ============================================

// Rotation zu Orientierung Mapping (ILI9341-spezifisch)
#define HW_ROTATION_PORTRAIT_NORMAL    0  // 240x320
#define HW_ROTATION_LANDSCAPE_LEFT     1  // 320x240 (Standard)
#define HW_ROTATION_PORTRAIT_INVERTED  2  // 240x320
#define HW_ROTATION_LANDSCAPE_RIGHT    3  // 320x240

// ============================================
// FEATURE FLAGS
// ============================================

#define HW_HAS_BACKLIGHT_CONTROL true
#define HW_HAS_PWM_BACKLIGHT true
#define HW_HAS_RGB_LED true
#define HW_HAS_RS485 true
#define HW_HAS_TOUCH true
#define HW_HAS_MULTITOUCH false
#define HW_HAS_SD_CARD false    // Abhängig vom Board
#define HW_HAS_SPEAKER false    // Abhängig vom Board

// ============================================
// TFT_ESPI USER_SETUP MAPPING
// ============================================

#define TFT_ESPI_DRIVER "ILI9341_DRIVER"
#define TFT_ESPI_WIDTH HW_DISPLAY_WIDTH
#define TFT_ESPI_HEIGHT HW_DISPLAY_HEIGHT
#define TFT_ESPI_MISO HW_DISPLAY_MISO
#define TFT_ESPI_MOSI HW_DISPLAY_MOSI
#define TFT_ESPI_SCLK HW_DISPLAY_SCLK
#define TFT_ESPI_CS HW_DISPLAY_CS
#define TFT_ESPI_DC HW_DISPLAY_DC
#define TFT_ESPI_RST HW_DISPLAY_RST
#define TFT_ESPI_SPI_PORT "HSPI"  // Display weiterhin HSPI
#define TFT_ESPI_SPI_FREQ HW_DISPLAY_SPI_FREQ

// ============================================
// HARDWARE-SPECIFIC INITIALIZATION CODE
// ============================================

// Hardware-spezifische Initialisierung
#define HW_INIT_CODE() do { \
  /* ILI9341-spezifische Initialisierung */ \
  /* Eventuell andere RGB-Order */ \
} while(0)

// Hardware-spezifische Touch-Initialisierung
#define HW_TOUCH_INIT_CODE() do { \
  /* XPT2046 mit VSPI initialisieren */ \
  Serial.println("Initialisiere Touch mit VSPI Bus"); \
} while(0)

// ============================================
// BOARD-SPECIFIC MACROS
// ============================================

// Touch-Koordinaten-Mapping für ILI9341 Landscape
#define HW_TOUCH_MAP_LANDSCAPE(rawX, rawY, mappedX, mappedY) do { \
  mappedX = map(rawY, HW_TOUCH_MIN_Y, HW_TOUCH_MAX_Y, 0, HW_DISPLAY_WIDTH); \
  mappedY = map(rawX, HW_TOUCH_MAX_X, HW_TOUCH_MIN_X, 0, HW_DISPLAY_HEIGHT); \
} while(0)

// Touch-Koordinaten-Mapping für ILI9341 Portrait
#define HW_TOUCH_MAP_PORTRAIT(rawX, rawY, mappedX, mappedY) do { \
  mappedX = map(rawX, HW_TOUCH_MIN_X, HW_TOUCH_MAX_X, 0, HW_DISPLAY_WIDTH); \
  mappedY = map(rawY, HW_TOUCH_MIN_Y, HW_TOUCH_MAX_Y, 0, HW_DISPLAY_HEIGHT); \
} while(0)

#define HW_INIT_CODE() do { \
  tft.writecommand(0x2A); /* Column Address Set */ \
  tft.writedata(0x00); tft.writedata(0x00); \
  tft.writedata(0x01); tft.writedata(0x3F); /* 0-319 */ \
  tft.writecommand(0x2B); /* Row Address Set */ \
  tft.writedata(0x00); tft.writedata(0x00); \
  tft.writedata(0x00); tft.writedata(0xEF); /* 0-239 */ \
} while(0)
	
// ============================================
// VALIDATION
// ============================================

// Hardware-Konsistenz prüfen
#if (HW_DISPLAY_WIDTH != 320 || HW_DISPLAY_HEIGHT != 240) && (HW_DISPLAY_WIDTH != 240 || HW_DISPLAY_HEIGHT != 320)
  #error "ESP32-2432S028R Display muss 320x240 oder 240x320 sein"
#endif

#if HW_TOUCH_SPI_BUS != VSPI
  #error "ESP32-2432S028R Touch muss VSPI Bus verwenden"
#endif

#if HW_TOUCH_MULTIPOINT != 1
  #error "ESP32-2432S028R Hardware unterstützt nur Single-Touch"
#endif

// ============================================
// BOARD DETECTION CODE
// ============================================

// Funktion zur Laufzeit-Erkennung des Boards (optional)
#define HW_BOARD_DETECTION_CODE() ({ \
  bool detected = false; \
  /* Hier könnte Pin-Testing oder andere Erkennung stehen */ \
  detected; \
})

#endif // ESP32_2432S028R_H