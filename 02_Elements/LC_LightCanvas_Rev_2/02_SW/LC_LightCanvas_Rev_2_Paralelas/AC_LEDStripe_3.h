/*
 * @author Pablo dMM (Pablodmm.isp@gmail.com)
 * @brief  Part of the Henar#4 Project. https://github.com/Autofabricantes/Henar-4 .This code controls 8 padNote, 1 padInstrument and 1 padOctave
 * @date   2021_10_31
 */
#ifndef AC_LEDStripe_3_H
#define AC_LEDStripe_3_H

#include "Arduino.h"
//#include <Adafruit_NeoPixel.h>
#include <FastLED.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

/*#define P_LED     33           
#define Q_LED     112

#define P_LED_1     25           
#define Q_LED_1     190

#define P_LED_2     26          
#define Q_LED_2     280
*/
#define P_LED     27           
#define Q_LED     216

/*#define P_LED_4     14           
#define Q_LED_4     500

#define P_LED_5     12           
#define Q_LED_5     500*/

class AC_LEDStripe_3
{
  public:
    AC_LEDStripe_3(int iniR, int iniG, int iniB);
    void init();
    void setFromSensor(int sensorRead);
  private:
    int _ledStripeId;
    int _pinLed;
  	int _qLed;
  	int _iniR;
  	int _iniG;
  	int _iniB;
  	CRGB leds[Q_LED];
    // Gamma corrector table for WS2812B.
    // Obtained from: https://learn.adafruit.com/led-tricks-gamma-correction/the-quick-fix
    uint8_t gamma8[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
};

#endif
