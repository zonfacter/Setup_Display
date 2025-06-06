/**
 * hardware_hal.h - Hardware Abstraction Layer
 * 
 * Compile-Time Hardware Profile Selection für ESP32 Touch-Panels
 * 
 * Usage:
 * #define HARDWARE_PROFILE ESP32_2432S028R
 * #include "hardware_hal.h"
 */

#ifndef HARDWARE_HAL_H
#define HARDWARE_HAL_H

#include <Arduino.h>

// ============================================
// HARDWARE PROFILE SELECTION
// ============================================

// Verfügbare Hardware-Profile
#define ESP32_TZT_24      1    // TZT 2,4" ST7789 (original)
#define ESP32_2432S028R   2    // 2,8" ILI9341
#define ESP32_GENERIC     99   // Generic Fallback

// Standard-Profile falls nicht definiert
#ifndef HARDWARE_PROFILE
  #define HARDWARE_PROFILE ESP32_TZT_24
  #warning "HARDWARE_PROFILE nicht definiert - verwende ESP32_TZT_24 als Standard"
#endif

// ============================================
// PROFILE-SPEZIFISCHE INCLUDES
// ============================================

#if HARDWARE_PROFILE == ESP32_TZT_24
  #include "hardware_profiles/esp32_tzt_24.h"
#elif HARDWARE_PROFILE == ESP32_2432S028R
  #include "hardware_profiles/esp32_2432s028r.h"
#elif HARDWARE_PROFILE == ESP32_GENERIC
  #include "hardware_profiles/esp32_generic.h"
#else
  #error "Unbekanntes HARDWARE_PROFILE - unterstützte Profile: ESP32_TZT_24, ESP32_2432S028R, ESP32_GENERIC"
#endif

// ============================================
// HARDWARE ABSTRACTION LAYER API
// ============================================

class HardwareManager {
private:
  bool initialized;
  void initBacklight();  // Private Methode deklariert
  
public:
  HardwareManager();
  
  // Initialisierung
  bool begin();
  void end();
  
  // Display Management
  bool initDisplay();
  void setDisplayRotation(int rotation);
  void setDisplayBrightness(int percent);
  void invertDisplay(bool invert);
  
  // Touch Management  
  bool initTouch();
  bool isTouchPressed();
  void getTouchPoint(int* x, int* y);
  int getTouchCount();  // Multi-Touch Support
  void getTouchPoints(int points[][2], int maxPoints); // Multi-Touch
  
  // Hardware Info
  String getProfileName();
  String getDisplayController();
  String getTouchController();
  bool hasMultiTouch();
  bool hasBacklightControl();
  
  // Feature Flags
  bool isBacklightInverted();
  bool areColorsInverted();
  bool hasPWMBacklight();
  
  // Debug & Diagnostics
  void printHardwareInfo();
  bool validateHardware();
};

// Globale Hardware Manager Instanz
extern HardwareManager hardware;

// ============================================
// UNIFIED HARDWARE INTERFACE MACROS
// ============================================

// Display Macros (für Backward-Kompatibilität)
#define DISPLAY_WIDTH     HW_DISPLAY_WIDTH
#define DISPLAY_HEIGHT    HW_DISPLAY_HEIGHT
#define DISPLAY_ROTATION  HW_DEFAULT_ROTATION

// Touch Macros
#define TOUCH_THRESHOLD   HW_TOUCH_THRESHOLD
#define TOUCH_CALIBRATED  HW_TOUCH_CALIBRATED

// ============================================
// FEATURE DETECTION MACROS
// ============================================

// Multi-Touch Detection
#ifdef HW_TOUCH_MULTIPOINT
  #define HAS_MULTITOUCH 1
  #define MAX_TOUCH_POINTS HW_TOUCH_MULTIPOINT
#else
  #define HAS_MULTITOUCH 0
  #define MAX_TOUCH_POINTS 1
#endif

// PWM Backlight Detection
#ifdef HW_BACKLIGHT_PWM_CHANNEL
  #define HAS_PWM_BACKLIGHT 1
#else
  #define HAS_PWM_BACKLIGHT 0
#endif

// Color Inversion Detection
#ifdef HW_COLORS_INVERTED
  #define COLORS_INVERTED HW_COLORS_INVERTED
#else
  #define COLORS_INVERTED false
#endif

// ============================================
// VALIDATION MACROS
// ============================================

// Pflicht-Definitionen prüfen
#ifndef HW_PROFILE_NAME
  #error "HW_PROFILE_NAME muss im Hardware-Profile definiert sein"
#endif

#ifndef HW_DISPLAY_WIDTH
  #error "HW_DISPLAY_WIDTH muss im Hardware-Profile definiert sein"
#endif

#ifndef HW_DISPLAY_HEIGHT
  #error "HW_DISPLAY_HEIGHT muss im Hardware-Profile definiert sein"
#endif

// ============================================
// BACKWARD COMPATIBILITY LAYER
// ============================================

// Alte config.h Defines für Kompatibilität
#ifndef SCREEN_WIDTH
  #define SCREEN_WIDTH HW_DISPLAY_WIDTH
#endif

#ifndef SCREEN_HEIGHT
  #define SCREEN_HEIGHT HW_DISPLAY_HEIGHT
#endif

#ifndef TFT_BL_PIN
  #ifdef HW_BACKLIGHT_PIN
    #define TFT_BL_PIN HW_BACKLIGHT_PIN
  #endif
#endif

// ============================================
// COMPILE-TIME INFORMATION
// ============================================

// Hardware-Info zur Compile-Zeit ausgeben
#pragma message "Hardware Profile: " HW_PROFILE_NAME
#pragma message "Display: " HW_DISPLAY_CONTROLLER_STR " (" HW_DISPLAY_WIDTH_STR "x" HW_DISPLAY_HEIGHT_STR ")"
#pragma message "Touch: " HW_TOUCH_CONTROLLER_STR

#endif // HARDWARE_HAL_H