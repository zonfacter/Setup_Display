/**
 * hardware_manager.cpp - Hardware Abstraction Layer Implementation
 */

#include "config.h"
#include "TFT_Setup.h"  // VOR TFT_eSPI!
#include <TFT_eSPI.h>
#include "hardware_hal.h"
#include <SPI.h>
#include <XPT2046_Touchscreen.h>

// Globale Hardware Manager Instanz
HardwareManager hardware;

// Hardware-spezifische Instanzen
TFT_eSPI tft = TFT_eSPI();

#if HW_TOUCH_SPI_BUS == VSPI
  SPIClass touchSPI = SPIClass(VSPI);
#else
  SPIClass touchSPI = SPIClass(HSPI);
#endif

XPT2046_Touchscreen touch(HW_TOUCH_CS, HW_TOUCH_IRQ);

HardwareManager::HardwareManager() : initialized(false) {}

bool HardwareManager::begin() {
  Serial.println("Initialisiere Hardware: " HW_PROFILE_NAME);
  
  if (!initDisplay()) {
    Serial.println("Display-Initialisierung fehlgeschlagen");
    return false;
  }
  
  if (!initTouch()) {
    Serial.println("Touch-Initialisierung fehlgeschlagen");
    return false;
  }
  
  #ifdef HW_BACKLIGHT_PIN
    initBacklight();
  #endif
  
  // Hardware-spezifische Initialisierung
  HW_INIT_CODE();
  
  initialized = true;
  printHardwareInfo();
  return true;
}

void HardwareManager::end() {
  initialized = false;
}

bool HardwareManager::initDisplay() {
  // TFT initialisieren
  tft.init();
  tft.setRotation(HW_DEFAULT_ROTATION);

  #ifdef HW_COLORS_INVERTED
    if (HW_COLORS_INVERTED) {
      tft.invertDisplay(true);
    }
  #endif
  
  Serial.printf("Display initialisiert: %dx%d, Rotation: %d\n", 
                HW_DISPLAY_WIDTH, HW_DISPLAY_HEIGHT, HW_DEFAULT_ROTATION);
  return true;
  // Vollbildschirm löschen testen
  tft.fillRect(0, 0, 320, 240, TFT_BLACK);
  delay(100);
  tft.drawRect(0, 0, 320, 240, TFT_WHITE);
}

bool HardwareManager::initTouch() {
  // Touch SPI initialisieren
  touchSPI.begin(HW_TOUCH_CLK, HW_TOUCH_MISO, HW_TOUCH_MOSI, HW_TOUCH_CS);
  
  // Touch initialisieren
  touch.begin(touchSPI);
  
  #ifdef HW_TOUCH_INIT_CODE
    HW_TOUCH_INIT_CODE();
  #endif
  
  Serial.printf("Touch initialisiert: %s auf %s\n", 
                HW_TOUCH_CONTROLLER_STR, 
                (HW_TOUCH_SPI_BUS == VSPI) ? "VSPI" : "HSPI");
  return true;
}

void HardwareManager::initBacklight() {
  #ifdef HW_BACKLIGHT_PIN
    #ifdef HW_BACKLIGHT_PWM_CHANNEL
      // PWM-Backlight
      ledcAttach(HW_BACKLIGHT_PIN, HW_BACKLIGHT_PWM_FREQ, HW_BACKLIGHT_PWM_RESOLUTION);
      setDisplayBrightness(HW_BACKLIGHT_DEFAULT);
      Serial.println("PWM-Backlight initialisiert");
    #else
      // Digital Backlight
      pinMode(HW_BACKLIGHT_PIN, OUTPUT);
      digitalWrite(HW_BACKLIGHT_PIN, HW_BACKLIGHT_INVERTED ? LOW : HIGH);
      Serial.println("Digital-Backlight initialisiert");
    #endif
  #endif
}

void HardwareManager::setDisplayRotation(int rotation) {
  tft.setRotation(rotation);
}

void HardwareManager::setDisplayBrightness(int percent) {
  #ifdef HW_BACKLIGHT_PIN
    percent = constrain(percent, 0, 100);
    
    #ifdef HW_BACKLIGHT_PWM_CHANNEL
      // PWM-Steuerung
      int duty = map(percent, 0, 100, 0, (1 << HW_BACKLIGHT_PWM_RESOLUTION) - 1);
      if (HW_BACKLIGHT_INVERTED) {
        duty = ((1 << HW_BACKLIGHT_PWM_RESOLUTION) - 1) - duty;
      }
      ledcWrite(HW_BACKLIGHT_PIN, duty);
    #else
      // Digital Ein/Aus
      bool state = (percent > 50) ? !HW_BACKLIGHT_INVERTED : HW_BACKLIGHT_INVERTED;
      digitalWrite(HW_BACKLIGHT_PIN, state);
    #endif
  #endif
}

void HardwareManager::invertDisplay(bool invert) {
  tft.invertDisplay(invert);
}

bool HardwareManager::isTouchPressed() {
  return touch.tirqTouched() && touch.touched();
}

void HardwareManager::getTouchPoint(int* x, int* y) {
  if (!isTouchPressed()) {
    *x = -1;
    *y = -1;
    return;
  }
  
  TS_Point p = touch.getPoint();
  int rotation = tft.getRotation();
  
  // Hardware-spezifisches Touch-Mapping
  switch (rotation) {
    case 0: // Portrait
      #ifdef HW_TOUCH_MAP_PORTRAIT
        HW_TOUCH_MAP_PORTRAIT(p.x, p.y, *x, *y);
      #else
        *x = map(p.x, HW_TOUCH_MIN_X, HW_TOUCH_MAX_X, 0, tft.width());
        *y = map(p.y, HW_TOUCH_MIN_Y, HW_TOUCH_MAX_Y, 0, tft.height());
      #endif
      break;
      
    case 1: // Landscape
      #ifdef HW_TOUCH_MAP_LANDSCAPE
        HW_TOUCH_MAP_LANDSCAPE(p.x, p.y, *x, *y);
      #else
        *x = map(p.y, HW_TOUCH_MIN_Y, HW_TOUCH_MAX_Y, 0, tft.width());
        *y = map(p.x, HW_TOUCH_MAX_X, HW_TOUCH_MIN_X, 0, tft.height());
      #endif
      break;
      
    case 2: // Portrait inverted
      *x = map(p.x, HW_TOUCH_MAX_X, HW_TOUCH_MIN_X, 0, tft.width());
      *y = map(p.y, HW_TOUCH_MAX_Y, HW_TOUCH_MIN_Y, 0, tft.height());
      break;
      
    case 3: // Landscape inverted
      *x = map(p.y, HW_TOUCH_MAX_Y, HW_TOUCH_MIN_Y, 0, tft.width());
      *y = map(p.x, HW_TOUCH_MIN_X, HW_TOUCH_MAX_X, 0, tft.height());
      break;
  }
  
  // Touch-Inversion anwenden
  #ifdef HW_TOUCH_INVERT_X
    if (HW_TOUCH_INVERT_X) {
      *x = tft.width() - *x;
    }
  #endif
  
  #ifdef HW_TOUCH_INVERT_Y
    if (HW_TOUCH_INVERT_Y) {
      *y = tft.height() - *y;
    }
  #endif
  
  // Koordinaten begrenzen
  *x = constrain(*x, 0, tft.width() - 1);
  *y = constrain(*y, 0, tft.height() - 1);
}

int HardwareManager::getTouchCount() {
  #ifdef HW_TOUCH_MULTIPOINT
    return HW_TOUCH_MULTIPOINT;
  #else
    return 1;
  #endif
}

void HardwareManager::getTouchPoints(int points[][2], int maxPoints) {
  // Single-Touch Implementation
  if (maxPoints > 0 && isTouchPressed()) {
    getTouchPoint(&points[0][0], &points[0][1]);
    // Weitere Punkte als ungültig markieren
    for (int i = 1; i < maxPoints; i++) {
      points[i][0] = -1;
      points[i][1] = -1;
    }
  } else {
    // Alle Punkte als ungültig markieren
    for (int i = 0; i < maxPoints; i++) {
      points[i][0] = -1;
      points[i][1] = -1;
    }
  }
}

String HardwareManager::getProfileName() {
  return String(HW_PROFILE_NAME);
}

String HardwareManager::getDisplayController() {
  return String(HW_DISPLAY_CONTROLLER_STR);
}

String HardwareManager::getTouchController() {
  return String(HW_TOUCH_CONTROLLER_STR);
}

bool HardwareManager::hasMultiTouch() {
  #ifdef HW_TOUCH_MULTIPOINT
    return HW_TOUCH_MULTIPOINT > 1;
  #else
    return false;
  #endif
}

bool HardwareManager::hasBacklightControl() {
  #ifdef HW_HAS_BACKLIGHT_CONTROL
    return HW_HAS_BACKLIGHT_CONTROL;
  #else
    return false;
  #endif
}

bool HardwareManager::isBacklightInverted() {
  #ifdef HW_BACKLIGHT_INVERTED
    return HW_BACKLIGHT_INVERTED;
  #else
    return false;
  #endif
}

bool HardwareManager::areColorsInverted() {
  #ifdef HW_COLORS_INVERTED
    return HW_COLORS_INVERTED;
  #else
    return false;
  #endif
}

bool HardwareManager::hasPWMBacklight() {
  #ifdef HW_BACKLIGHT_PWM_CHANNEL
    return true;
  #else
    return false;
  #endif
}

void HardwareManager::printHardwareInfo() {
  Serial.println("\n=== HARDWARE INFORMATION ===");
  Serial.printf("Profile: %s\n", HW_PROFILE_NAME);
  Serial.printf("Display: %s (%dx%d)\n", HW_DISPLAY_CONTROLLER_STR, HW_DISPLAY_WIDTH, HW_DISPLAY_HEIGHT);
  Serial.printf("Touch: %s (%s, %d points)\n", HW_TOUCH_CONTROLLER_STR, 
                (HW_TOUCH_SPI_BUS == VSPI) ? "VSPI" : "HSPI", getTouchCount());
  
  #ifdef HW_BACKLIGHT_PIN
    Serial.printf("Backlight: Pin %d (%s, %s)\n", HW_BACKLIGHT_PIN,
                  HW_BACKLIGHT_INVERTED ? "inverted" : "normal",
                  hasPWMBacklight() ? "PWM" : "digital");
  #endif
  
  Serial.printf("Features: ");
  if (hasMultiTouch()) Serial.print("MultiTouch ");
  if (hasBacklightControl()) Serial.print("Backlight ");
  if (areColorsInverted()) Serial.print("ColorInv ");
  Serial.println();
  
  Serial.println("============================\n");
}

bool HardwareManager::validateHardware() {
  bool valid = true;
  
  // Display-Validation
  if (tft.width() != HW_DISPLAY_WIDTH || tft.height() != HW_DISPLAY_HEIGHT) {
    Serial.printf("ERROR: Display size mismatch. Expected: %dx%d, Got: %dx%d\n",
                  HW_DISPLAY_WIDTH, HW_DISPLAY_HEIGHT, tft.width(), tft.height());
    valid = false;
  }
  
  // Touch-Validation
  if (!touch.begin(touchSPI)) {
    Serial.println("ERROR: Touch controller not responding");
    valid = false;
  }
  
  return valid;
}