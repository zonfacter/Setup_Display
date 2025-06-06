#ifndef TFT_SETUP_H
#define TFT_SETUP_H

#define USER_SETUP_LOADED
#define DISABLE_ALL_LIBRARY_WARNINGS

// Verhindere Laden der Standard User_Setup.h
#undef USER_SETUP_INFO
#define USER_SETUP_INFO "Hardware_HAL_Setup"

// Hardware Profile laden
#include "config.h"
#include "hardware_hal.h"

// Dynamische TFT Konfiguration basierend auf Hardware Profile
#if HW_DISPLAY_CONTROLLER == ILI9341
  #define ILI9341_DRIVER
  #if HARDWARE_PROFILE == ESP32_2432S028R
    #define TFT_RGB_ORDER TFT_RGB
    #define TFT_INVERSION_OFF
  #else
    #define TFT_RGB_ORDER TFT_RGB
    #define TFT_INVERSION_OFF
  #endif
#elif HW_DISPLAY_CONTROLLER == ST7789
  #define ST7789_DRIVER
  #define TFT_RGB_ORDER TFT_RGB
  #define TFT_INVERSION_OFF
#endif

#define TFT_WIDTH  240  // Fest statt HW_DISPLAY_WIDTH
#define TFT_HEIGHT 320  // Fest statt HW_DISPLAY_HEIGHT
#define TFT_MISO   HW_DISPLAY_MISO
#define TFT_MOSI   HW_DISPLAY_MOSI
#define TFT_SCLK   HW_DISPLAY_SCLK
#define TFT_CS     HW_DISPLAY_CS
#define TFT_DC     HW_DISPLAY_DC
#define TFT_RST    HW_DISPLAY_RST
#define SPI_FREQUENCY HW_DISPLAY_SPI_FREQ

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4

#endif