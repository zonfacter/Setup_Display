/**
 * hardware_commissioning.ino - Hardware Inbetriebnahme & Test Tool
 * 
 * Eigenständiges Programm zum Testen und Kalibrieren von ESP32 Touch-Displays
 * 
 * USAGE:
 * 1. #define HARDWARE_PROFILE ESP32_DEIN_PROFILE
 * 2. Upload & öffne Serial Monitor
 * 3. Folge den Test-Menü Anweisungen
 * 4. Kalibrierungsdaten notieren
 */

// ============================================
// HARDWARE PROFILE SELECTION
// ============================================


// ============================================
// REQUIRED LIBRARIES
// ============================================

#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include "config.h"
#include "hardware_hal.h"

// ============================================
// EXTERNAL DECLARATIONS
// ============================================

// TFT und Touch Instanzen aus hardware_manager.cpp
extern TFT_eSPI tft;
extern XPT2046_Touchscreen touch;

// ============================================
// TEST CONFIGURATION
// ============================================

#define SERIAL_BAUD 115200
#define TEST_TIMEOUT 30000  // 30s pro Test
#define TOUCH_DEBOUNCE 100  // 100ms Debounce

// Test-Modi
enum TestMode {
  TEST_MENU = 0,
  TEST_DISPLAY = 1,
  TEST_COLORS = 2,
  TEST_BACKLIGHT = 3,
  TEST_TOUCH_SINGLE = 4,
  TEST_TOUCH_MULTI = 5,
  TEST_TOUCH_CALIBRATION = 6,
  TEST_ORIENTATION = 7,
  TEST_STRESS = 8,
  TEST_INFO = 9
};

// Globale Variablen
TestMode currentTest = TEST_MENU;
unsigned long testStartTime = 0;
bool testRunning = false;

// Touch-Kalibrierung
struct TouchCalibration {
  int minX = 9999, maxX = 0;
  int minY = 9999, maxY = 0;
  int samples = 0;
} touchCal;

// ============================================
// SETUP & MAIN LOOP
// ============================================

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(2000);
  
  printHeader();
  
  // Hardware initialisieren
  if (!hardware.begin()) {
    Serial.println("❌ HARDWARE INIT FAILED!");
    while(1) delay(1000);
  }
  
  // DIREKT-TEST: Vollbild-Farben
  Serial.println("Teste Display-Grenzen...");
  tft.fillScreen(TFT_RED);
  delay(1000);
  tft.fillScreen(TFT_GREEN);
  delay(1000);
  tft.fillScreen(TFT_BLUE);
  delay(1000);
  
  // Test: Rahmen um ganzes Display
  tft.fillScreen(TFT_BLACK);
  tft.drawRect(0, 0, 320, 240, TFT_WHITE);
  delay(2000);
  
  Serial.printf("TFT Größe: %dx%d\n", tft.width(), tft.height());
}

void loop() {
  // Serial Kommandos verarbeiten
  if (Serial.available()) {
    char cmd = Serial.read();
    handleSerialCommand(cmd);
  }
  
  // Aktiver Test verarbeiten
  if (testRunning) {
    switch(currentTest) {
      case TEST_DISPLAY: runDisplayTest(); break;
      case TEST_COLORS: runColorTest(); break;
      case TEST_BACKLIGHT: runBacklightTest(); break;
      case TEST_TOUCH_SINGLE: runSingleTouchTest(); break;
      case TEST_TOUCH_MULTI: runMultiTouchTest(); break;
      case TEST_TOUCH_CALIBRATION: runTouchCalibration(); break;
      case TEST_ORIENTATION: runOrientationTest(); break;
      case TEST_STRESS: runStressTest(); break;
      default: testRunning = false; break;
    }
    
    // Timeout prüfen
    if (millis() - testStartTime > TEST_TIMEOUT) {
      Serial.println("\n⏰ Test-Timeout erreicht");
      stopTest();
    }
  }
  
  delay(10);
}

// ============================================
// MENU SYSTEM
// ============================================

void printHeader() {
  Serial.println("\n" + String('=', 60));
  Serial.println("🔧 ESP32 HARDWARE COMMISSIONING TOOL v1.0");
  Serial.println("Hardware Profile: " + hardware.getProfileName());
  Serial.println(String('=', 60));
}

void showMainMenu() {
  Serial.println("\n📋 HAUPT-MENÜ:");
  Serial.println("1 - Display Test (Grundfunktionen)");
  Serial.println("2 - Farb Test (Normal/Invertiert)");
  Serial.println("3 - Backlight Test (PWM/Digital)");
  Serial.println("4 - Single Touch Test");
  Serial.println("5 - Multi Touch Test");
  Serial.println("6 - Touch Kalibrierung");
  Serial.println("7 - Orientierungs Test");
  Serial.println("8 - Stress Test");
  Serial.println("9 - Hardware Info");
  Serial.println("0 - Menü wiederholen");
  Serial.println("\nWähle Test (1-9, 0): ");
}

void handleSerialCommand(char cmd) {
  switch(cmd) {
    case '0': showMainMenu(); break;
    case '1': startTest(TEST_DISPLAY); break;
    case '2': startTest(TEST_COLORS); break;
    case '3': startTest(TEST_BACKLIGHT); break;
    case '4': startTest(TEST_TOUCH_SINGLE); break;
    case '5': startTest(TEST_TOUCH_MULTI); break;
    case '6': startTest(TEST_TOUCH_CALIBRATION); break;
    case '7': startTest(TEST_ORIENTATION); break;
    case '8': startTest(TEST_STRESS); break;
    case '9': printDetailedInfo(); break;
    case 'q': case 'Q': stopTest(); break;
    default: 
      if (testRunning) handleTestCommand(cmd);
      break;
  }
}

// ============================================
// TEST FRAMEWORK
// ============================================

void startTest(TestMode test) {
  currentTest = test;
  testStartTime = millis();
  testRunning = true;
  
  Serial.println("\n🚀 Starte Test: " + getTestName(test));
  Serial.println("Drücke 'q' zum Beenden");
}

void stopTest() {
  testRunning = false;
  Serial.println("\n✋ Test beendet");
  showMainMenu();
}

String getTestName(TestMode test) {
  switch(test) {
    case TEST_DISPLAY: return "Display Grundfunktionen";
    case TEST_COLORS: return "Farb & Inversion Test";
    case TEST_BACKLIGHT: return "Backlight Steuerung";
    case TEST_TOUCH_SINGLE: return "Single Touch";
    case TEST_TOUCH_MULTI: return "Multi Touch";
    case TEST_TOUCH_CALIBRATION: return "Touch Kalibrierung";
    case TEST_ORIENTATION: return "Display Orientierung";
    case TEST_STRESS: return "Stress Test";
    default: return "Unbekannt";
  }
}

// ============================================
// DISPLAY TESTS
// ============================================

void runDisplayTest() {
  static int phase = 0;
  static unsigned long lastUpdate = 0;
  
  if (millis() - lastUpdate < 2000) return;
  lastUpdate = millis();
  
  switch(phase) {
    case 0:
      Serial.println("📺 Phase 1: Vollbild Farben");
      tft.fillScreen(TFT_RED);
      break;
    case 1:
      tft.fillScreen(TFT_GREEN);
      break;
    case 2:
      tft.fillScreen(TFT_BLUE);
      break;
    case 3:
      Serial.println("📺 Phase 2: Geometrische Formen");
      drawGeometryTest();
      break;
    case 4:
      Serial.println("📺 Phase 3: Text Rendering");
      drawTextTest();
      break;
    case 5:
      Serial.println("📺 Phase 4: Farbverläufe");
      drawGradientTest();
      break;
    default:
      Serial.println("✅ Display Test abgeschlossen");
      stopTest();
      return;
  }
  phase++;
}

void drawGeometryTest() {
  tft.fillScreen(TFT_BLACK);
  
  // Rechtecke
  tft.fillRect(10, 10, 50, 30, TFT_RED);
  tft.drawRect(70, 10, 50, 30, TFT_GREEN);
  
  // Kreise
  tft.fillCircle(50, 80, 20, TFT_BLUE);
  tft.drawCircle(120, 80, 20, TFT_YELLOW);
  
  // Linien
  for(int i = 0; i < 10; i++) {
    tft.drawLine(10 + i*3, 120, 10 + i*3, 150, TFT_WHITE);
  }
}

void drawTextTest() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  
  // Verschiedene Schriftgrößen
  tft.drawString("Text Size 1", 10, 10, 1);
  tft.drawString("Text Size 2", 10, 30, 2);
  tft.drawString("Size 4", 10, 60, 4);
  
  // Farben
  tft.setTextColor(TFT_RED);
  tft.drawString("Red Text", 10, 100, 2);
  tft.setTextColor(TFT_GREEN);
  tft.drawString("Green Text", 10, 120, 2);
}

void drawGradientTest() {
  for(int x = 0; x < tft.width(); x++) {
    uint16_t color = tft.color565(x * 255 / tft.width(), 0, 0);
    tft.drawFastVLine(x, 0, tft.height()/3, color);
  }
  
  for(int y = 0; y < tft.height()/3; y++) {
    uint16_t color = tft.color565(0, y * 255 / (tft.height()/3), 0);
    tft.drawFastHLine(0, tft.height()/3 + y, tft.width(), color);
  }
}

// ============================================
// COLOR TESTS
// ============================================

void runColorTest() {
  static int phase = 0;
  static unsigned long lastUpdate = 0;
  static bool inverted = false;
  
  if (millis() - lastUpdate < 3000) return;
  lastUpdate = millis();
  
  switch(phase) {
    case 0:
      Serial.println("🎨 Normale Farben");
      hardware.invertDisplay(false);
      drawColorPattern();
      break;
    case 1:
      Serial.println("🎨 Invertierte Farben");
      hardware.invertDisplay(true);
      drawColorPattern();
      inverted = true;
      break;
    case 2:
      Serial.println("🎨 RGB Komponenten Test");
      drawRGBComponentTest();
      break;
    default:
      if (inverted) {
        hardware.invertDisplay(false);
        Serial.println("↩️ Farb-Inversion zurückgesetzt");
      }
      Serial.println("✅ Farb Test abgeschlossen");
      stopTest();
      return;
  }
  phase++;
}

void drawColorPattern() {
  int w = tft.width() / 8;
  int h = tft.height() / 8;
  
  uint16_t colors[] = {
    TFT_BLACK, TFT_WHITE, TFT_RED, TFT_GREEN, 
    TFT_BLUE, TFT_YELLOW, TFT_MAGENTA, TFT_CYAN
  };
  
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      tft.fillRect(i*w, j*h, w, h, colors[(i+j) % 8]);
    }
  }
}

void drawRGBComponentTest() {
  int w = tft.width() / 3;
  
  // Rot
  for(int x = 0; x < w; x++) {
    uint16_t color = tft.color565(x * 255 / w, 0, 0);
    tft.drawFastVLine(x, 0, tft.height(), color);
  }
  
  // Grün
  for(int x = 0; x < w; x++) {
    uint16_t color = tft.color565(0, x * 255 / w, 0);
    tft.drawFastVLine(w + x, 0, tft.height(), color);
  }
  
  // Blau
  for(int x = 0; x < w; x++) {
    uint16_t color = tft.color565(0, 0, x * 255 / w);
    tft.drawFastVLine(2*w + x, 0, tft.height(), color);
  }
}

// ============================================
// BACKLIGHT TESTS
// ============================================

void runBacklightTest() {
  static int brightness = 0;
  static bool increasing = true;
  static unsigned long lastUpdate = 0;
  
  if (millis() - lastUpdate < 200) return;
  lastUpdate = millis();
  
  hardware.setDisplayBrightness(brightness);
  
  // Info anzeigen
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Backlight Test", 10, 10, 2);
  tft.drawString("Helligkeit: " + String(brightness) + "%", 10, 40, 2);
  
  if (hardware.hasPWMBacklight()) {
    tft.setTextColor(TFT_GREEN);
    tft.drawString("PWM Backlight", 10, 70, 1);
  } else {
    tft.setTextColor(TFT_YELLOW);
    tft.drawString("Digital Backlight", 10, 70, 1);
  }
  
  Serial.println("💡 Helligkeit: " + String(brightness) + "%");
  
  // Helligkeit ändern
  if (increasing) {
    brightness += 10;
    if (brightness >= 100) {
      brightness = 100;
      increasing = false;
    }
  } else {
    brightness -= 10;
    if (brightness <= 0) {
      brightness = 0;
      increasing = true;
    }
  }
}

// ============================================
// TOUCH TESTS
// ============================================

void runSingleTouchTest() {
  static unsigned long lastTouch = 0;
  
  // Touch-Status prüfen
  if (hardware.isTouchPressed()) {
    if (millis() - lastTouch > TOUCH_DEBOUNCE) {
      int x, y;
      hardware.getTouchPoint(&x, &y);
      
      if (x >= 0 && y >= 0) {
        // Touch visualisieren
        tft.fillCircle(x, y, 10, TFT_RED);
        tft.drawCircle(x, y, 15, TFT_WHITE);
        
        Serial.printf("👆 Touch: X=%d, Y=%d\n", x, y);
        lastTouch = millis();
      }
    }
  }
  
  // Info-Text
  static unsigned long lastInfo = 0;
  if (millis() - lastInfo > 5000) {
    tft.fillRect(0, 0, tft.width(), 40, TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("Single Touch Test", 10, 10, 2);
    tft.drawString("Berühre das Display", 10, 25, 1);
    lastInfo = millis();
  }
}

void runMultiTouchTest() {
  if (!hardware.hasMultiTouch()) {
    Serial.println("❌ Hardware unterstützt kein Multi-Touch");
    stopTest();
    return;
  }
  
  // Multi-Touch Implementation
  int points[5][2];  // Max 5 Touch-Punkte
  hardware.getTouchPoints(points, 5);
  
  // Alle aktiven Punkte anzeigen
  for(int i = 0; i < 5; i++) {
    if (points[i][0] >= 0 && points[i][1] >= 0) {
      uint16_t color = (i == 0) ? TFT_RED : ((i == 1) ? TFT_GREEN : TFT_BLUE);
      tft.fillCircle(points[i][0], points[i][1], 8, color);
      Serial.printf("👆 Touch %d: X=%d, Y=%d\n", i+1, points[i][0], points[i][1]);
    }
  }
}

void runTouchCalibration() {
  static int phase = 0;
  static bool waitingForTouch = true;
  static unsigned long phaseStart = 0;
  
  if (phase == 0) {
    // Reset Kalibrierung
    touchCal = {9999, 0, 9999, 0, 0};
    phase = 1;
    phaseStart = millis();
    Serial.println("🎯 Touch Kalibrierung gestartet");
    Serial.println("Berühre alle 4 Ecken + Mitte für 30 Sekunden");
  }
  
  // Touch sammeln
  if (hardware.isTouchPressed()) {
    // Raw Touch-Daten lesen
    TS_Point p = touch.getPoint();
    
    // Min/Max aktualisieren
    touchCal.minX = min(touchCal.minX, (int)p.x);
    touchCal.maxX = max(touchCal.maxX, (int)p.x);
    touchCal.minY = min(touchCal.minY, (int)p.y);
    touchCal.maxY = max(touchCal.maxY, (int)p.y);
    touchCal.samples++;
    
    // Visuelles Feedback
    int x, y;
    hardware.getTouchPoint(&x, &y);
    tft.fillCircle(x, y, 5, TFT_GREEN);
    
    Serial.printf("Raw: X=%d, Y=%d | Min/Max: X=%d-%d, Y=%d-%d\n", 
                  p.x, p.y, touchCal.minX, touchCal.maxX, touchCal.minY, touchCal.maxY);
  }
  
  // Info anzeigen
  static unsigned long lastInfo = 0;
  if (millis() - lastInfo > 1000) {
    tft.fillRect(0, 0, tft.width(), 60, TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("Touch Kalibrierung", 10, 10, 2);
    tft.drawString("Samples: " + String(touchCal.samples), 10, 30, 1);
    tft.drawString("Zeit: " + String((millis() - phaseStart)/1000) + "s", 10, 45, 1);
    lastInfo = millis();
  }
  
  // Timeout oder genug Samples
  if (millis() - phaseStart > 30000 || touchCal.samples > 100) {
    printCalibrationResults();
    stopTest();
  }
}

void printCalibrationResults() {
  Serial.println("\n" + String('=', 50));
  Serial.println("🎯 TOUCH KALIBRIERUNG ERGEBNISSE");
  Serial.println(String('=', 50));
  Serial.printf("Samples gesammelt: %d\n", touchCal.samples);
  Serial.printf("X-Range: %d - %d\n", touchCal.minX, touchCal.maxX);
  Serial.printf("Y-Range: %d - %d\n", touchCal.minY, touchCal.maxY);
  Serial.println("\n📋 HARDWARE PROFILE WERTE:");
  Serial.printf("#define HW_TOUCH_MIN_X %d\n", touchCal.minX);
  Serial.printf("#define HW_TOUCH_MAX_X %d\n", touchCal.maxX);
  Serial.printf("#define HW_TOUCH_MIN_Y %d\n", touchCal.minY);
  Serial.printf("#define HW_TOUCH_MAX_Y %d\n", touchCal.maxY);
  Serial.println(String('=', 50));
}

// ============================================
// ORIENTATION TEST
// ============================================

void runOrientationTest() {
  static int rotation = 0;
  static unsigned long lastRotation = 0;
  
  if (millis() - lastRotation < 10000) return;
  lastRotation = millis();
  
  hardware.setDisplayRotation(rotation);
  
  // DEBUG
  Serial.printf("Rotation %d: TFT=%dx%d, HW=%dx%d\n", 
                rotation, tft.width(), tft.height(), 
                HW_DISPLAY_WIDTH, HW_DISPLAY_HEIGHT);
  
  // Test-Pattern für jede Orientierung - ERST schwarzer Hintergrund
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  
  // Orientierung anzeigen
  tft.drawString("Rotation: " + String(rotation), 10, 10, 2);
  tft.drawString("Size: " + String(tft.width()) + "x" + String(tft.height()), 10, 30, 1);
  
  // Feste Marker-Größe
  int markerSize = 20;
  
  // Ecken markieren - NACH fillScreen!
  tft.fillRect(0, 0, markerSize, markerSize, TFT_RED);                           // Oben Links
  tft.fillRect(tft.width()-markerSize, 0, markerSize, markerSize, TFT_GREEN);           // Oben Rechts
  tft.fillRect(tft.width()-markerSize, tft.height()-markerSize, markerSize, markerSize, TFT_BLUE); // Unten Rechts
  tft.fillRect(0, tft.height()-markerSize, markerSize, markerSize, TFT_YELLOW);         // Unten Links
  
  // Debug: Koordinaten anzeigen
  Serial.printf("Marker Coords: TR(%d,0) BR(%d,%d) BL(0,%d)\n", 
                tft.width()-markerSize, tft.width()-markerSize, tft.height()-markerSize, tft.height()-markerSize);
  
  Serial.printf("📱 Rotation %d: %dx%d\n", rotation, tft.width(), tft.height());
  
  rotation = (rotation + 1) % 4;
  if (rotation == 0) {
    Serial.println("✅ Orientierungs Test abgeschlossen");
    stopTest();
  }
}

// ============================================
// STRESS TEST
// ============================================

void runStressTest() {
  static unsigned long operations = 0;
  static unsigned long lastUpdate = 0;
  
  // Zufällige Display-Operationen
  switch(random(4)) {
    case 0: // Zufällige Pixel
      for(int i = 0; i < 100; i++) {
        tft.drawPixel(random(tft.width()), random(tft.height()), random(0xFFFF));
      }
      break;
    case 1: // Zufällige Linien
      tft.drawLine(random(tft.width()), random(tft.height()), 
                   random(tft.width()), random(tft.height()), random(0xFFFF));
      break;
    case 2: // Zufällige Rechtecke
      tft.fillRect(random(tft.width()-50), random(tft.height()-50), 
                   random(50), random(50), random(0xFFFF));
      break;
    case 3: // Zufällige Kreise
      tft.fillCircle(random(tft.width()), random(tft.height()), 
                     random(20), random(0xFFFF));
      break;
  }
  
  operations++;
  
  // Statistiken anzeigen
  if (millis() - lastUpdate > 1000) {
    Serial.printf("⚡ Stress Test: %lu Operationen, FPS: %lu\n", 
                  operations, operations * 1000 / (millis() - testStartTime));
    lastUpdate = millis();
  }
}

// ============================================
// HARDWARE INFO
// ============================================

void printDetailedInfo() {
  Serial.println("\n" + String('=', 60));
  Serial.println("📊 DETAILLIERTE HARDWARE INFORMATIONEN");
  Serial.println(String('=', 60));
  
  // Hardware Manager Info
  hardware.printHardwareInfo();
  
  // Profile-spezifische Details
  Serial.printf("Display Resolution: %dx%d\n", HW_DISPLAY_WIDTH, HW_DISPLAY_HEIGHT);
  Serial.printf("Display Controller: %s\n", HW_DISPLAY_CONTROLLER_STR);
  Serial.printf("Touch Controller: %s\n", HW_TOUCH_CONTROLLER_STR);
  Serial.printf("Touch SPI Bus: %s\n", (HW_TOUCH_SPI_BUS == VSPI) ? "VSPI" : "HSPI");
  
  #ifdef HW_BACKLIGHT_PIN
  Serial.printf("Backlight Pin: %d (%s)\n", HW_BACKLIGHT_PIN,
                hardware.hasPWMBacklight() ? "PWM" : "Digital");
  #endif
  
  // Feature Matrix
  Serial.println("\n🔧 FEATURE MATRIX:");
  Serial.printf("Multi-Touch: %s\n", hardware.hasMultiTouch() ? "✅" : "❌");
  Serial.printf("PWM Backlight: %s\n", hardware.hasPWMBacklight() ? "✅" : "❌");
  Serial.printf("Color Inversion: %s\n", hardware.areColorsInverted() ? "✅" : "❌");
  Serial.printf("Backlight Inversion: %s\n", hardware.isBacklightInverted() ? "✅" : "❌");
  
  // Touch Kalibrierung Status
  #ifdef HW_TOUCH_CALIBRATED
  Serial.printf("Touch Kalibriert: %s\n", HW_TOUCH_CALIBRATED ? "✅" : "❌");
  Serial.printf("Touch Range: X=%d-%d, Y=%d-%d\n", 
                HW_TOUCH_MIN_X, HW_TOUCH_MAX_X, HW_TOUCH_MIN_Y, HW_TOUCH_MAX_Y);
  #endif
  
  // System Info
  Serial.println("\n💾 SYSTEM INFO:");
  Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
  Serial.printf("Flash Size: %d bytes\n", ESP.getFlashChipSize());
  Serial.printf("CPU Frequency: %d MHz\n", ESP.getCpuFreqMHz());
  
  Serial.println(String('=', 60));
}

void handleTestCommand(char cmd) {
  // Test-spezifische Kommandos können hier verarbeitet werden
  switch(currentTest) {
    case TEST_TOUCH_CALIBRATION:
      if (cmd == 'r') {
        Serial.println("🔄 Kalibrierung zurückgesetzt");
        touchCal = {9999, 0, 9999, 0, 0};
      }
      break;
    default:
      break;
  }
}