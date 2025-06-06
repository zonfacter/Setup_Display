/**
 * esp32_tzt_24.h - Hardware Profile für TZT ESP32 2,4" ST7789
 * 
 * Original Hardware-Konfiguration (bestehende Hardware)
 */

#ifndef ESP32_TZT_24_H
#define ESP32_TZT_24_H

// ============================================
// HARDWARE PROFILE INFORMATION
// ============================================

#define HW_PROFILE_NAME "ESP32-TZT-2.4"
#define HW_PROFILE_VERSION "1.0"
#define HW_PROFILE_DESCRIPTION "TZT ESP32 2.4 inch ST7789 Display"

// String-Versionen für Pragma Messages
#define HW_PROFILE_NAME_STR "ESP32-TZT-2.4"
#define HW_DISPLAY_WIDTH_STR "240"
#define HW_DISPLAY_HEIGHT_STR "320"
#define HW_DISPLAY_CONTROLLER_STR "ST7789"
#define HW_TOUCH_CONTROLLER_STR "XPT2046"

// ============================================
// DISPLAY CONFIGURATION
// ============================================

#define HW_DISPLAY_CONTROLLER ST7789
#define HW_DISPLAY_WIDTH 240
#define HW_DISPLAY_HEIGHT 320
#define HW_DEFAULT_ROTATION 2    // Portrait USB unten
#define HW_COLORS_INVERTED false
#define HW_DISPLAY_INVERSION_OFF true

// Display SPI Pins
#define HW_DISPLAY_MISO 12
#define HW_DISPLAY_MOSI 13
#define HW_DISPLAY_SCLK 14
#define HW_DISPLAY_CS 15
#define HW_DISPLAY_DC 2
#define HW_DISPLAY_RST -1  // Mit Arduino Reset verbunden

// Display Features
#define HW_DISPLAY_DMA true
#define HW_DISPLAY_SPI_FREQ 80000000
#define HW_DISPLAY_SPI_READ_FREQ 80000000

// ============================================
// BACKLIGHT CONFIGURATION
// ============================================

#define HW_BACKLIGHT_PIN 27
#define HW_BACKLIGHT_INVERTED false
#define HW_BACKLIGHT_PWM_CHANNEL 0
#define HW_BACKLIGHT_PWM_FREQ 5000
#define HW_BACKLIGHT_PWM_RESOLUTION 8
#define HW_BACKLIGHT_DEFAULT 100

// ============================================
// TOUCH CONFIGURATION
// ============================================

#define HW_TOUCH_CONTROLLER XPT2046
#define HW_TOUCH_SPI_BUS HSPI
#define HW_TOUCH_MULTIPOINT 1  // Single Touch

// Touch SPI Pins
#define HW_TOUCH_IRQ 36
#define HW_TOUCH_MOSI 13  // Shared with Display
#define HW_TOUCH_MISO 12  // Shared with Display
#define HW_TOUCH_CLK 14   // Shared with Display
#define HW_TOUCH_CS 33

// Touch Calibration
#define HW_TOUCH_MIN_X 400
#define HW_TOUCH_MAX_X 3900
#define HW_TOUCH_MIN_Y 400
#define HW_TOUCH_MAX_Y 3900
#define HW_TOUCH_THRESHOLD 600
#define HW_TOUCH_CALIBRATED true
#define HW_TOUCH_INVERT_X true
#define HW_TOUCH_INVERT_Y false

// Touch SPI Frequency
#define HW_TOUCH_SPI_FREQ 2500000

// ============================================
// ADDITIONAL HARDWARE
// ============================================

// RGB LED Pins
#define HW_LED_RED_PIN 4
#define HW_LED_GREEN_PIN 17
#define HW_LED_BLUE_PIN 16
#define HW_LED_INVERTED true  // Common cathode

// RS485 Communication
#define HW_RS485_RX_PIN 22
#define HW_RS485_TX_PIN 21
#define HW_RS485_UART UART2
#define HW_RS485_BAUD 57600

// ============================================
// ORIENTATION MAPPINGS
// ============================================

// Rotation zu Orientierung Mapping
#define HW_ROTATION_PORTRAIT_NORMAL    0  // USB oben
#define HW_ROTATION_LANDSCAPE_LEFT     1  // USB links
#define HW_ROTATION_PORTRAIT_INVERTED  2  // USB unten (Standard)
#define HW_ROTATION_LANDSCAPE_RIGHT    3  // USB rechts

// ============================================
// FEATURE FLAGS
// ============================================

#define HW_HAS_BACKLIGHT_CONTROL true
#define HW_HAS_PWM_BACKLIGHT true
#define HW_HAS_RGB_LED true
#define HW_HAS_RS485 true
#define HW_HAS_TOUCH true
#define HW_HAS_MULTITOUCH false

// ============================================
// TFT_ESPI USER_SETUP MAPPING
// ============================================

// Für automatische User_Setup.h Generierung
#define TFT_ESPI_DRIVER "ST7789_DRIVER"
#define TFT_ESPI_WIDTH HW_DISPLAY_WIDTH
#define TFT_ESPI_HEIGHT HW_DISPLAY_HEIGHT
#define TFT_ESPI_MISO HW_DISPLAY_MISO
#define TFT_ESPI_MOSI HW_DISPLAY_MOSI
#define TFT_ESPI_SCLK HW_DISPLAY_SCLK
#define TFT_ESPI_CS HW_DISPLAY_CS
#define TFT_ESPI_DC HW_DISPLAY_DC
#define TFT_ESPI_RST HW_DISPLAY_RST
#define TFT_ESPI_SPI_PORT "HSPI"
#define TFT_ESPI_SPI_FREQ HW_DISPLAY_SPI_FREQ

// ============================================
// HARDWARE-SPECIFIC INITIALIZATION CODE
// ============================================

// Hardware-spezifische Initialisierung
#define HW_INIT_CODE() do { \
  /* TFT-spezifische Initialisierung */ \
  /* RGB Order für ST7789 */ \
} while(0)

// Hardware-spezifische Touch-Initialisierung
#define HW_TOUCH_INIT_CODE() do { \
  /* XPT2046-spezifische Konfiguration */ \
} while(0)

// ============================================
// VALIDATION
// ============================================

// Hardware-Konsistenz prüfen
#if HW_DISPLAY_WIDTH != 240 || HW_DISPLAY_HEIGHT != 320
  #error "TZT 2.4 Display muss 240x320 sein"
#endif

#if HW_TOUCH_MULTIPOINT != 1
  #error "TZT 2.4 Hardware unterstützt nur Single-Touch"
#endif

#endif // ESP32_TZT_24_H