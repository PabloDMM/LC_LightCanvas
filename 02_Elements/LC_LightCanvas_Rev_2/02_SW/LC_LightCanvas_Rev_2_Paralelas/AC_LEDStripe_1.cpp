/*
 * @author Pablo dMM (Pablodmm.isp@gmail.com)
 * @brief  Part of the Henar#4 Project. https://github.com/Autofabricantes/Henar-4 .
 * @date   2021_10_31
 */
#include "Arduino.h"
#include "AC_LEDStripe_1.h"
//#include <Adafruit_NeoPixel.h>
#include <FastLED.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

/**
 * @brief AC_LEDStripe_1 Constructor.
 * @param pinLed  Pin of the LED Stripe
 */
AC_LEDStripe_1::AC_LEDStripe_1(int iniR, int iniG, int iniB)
{
  // Pin Configuration
  _iniR = iniR;
  _iniG = iniG;
  _iniB = iniB;
  _qLed = Q_LED;
  _pinLed = P_LED;
  //CRGB leds = CRGB[_qLed];
}

/**
 * @brief Initializes the LED Stripe and sets it to OFF
 */
void AC_LEDStripe_1::init()
{
  FastLED.addLeds<WS2812, P_LED, GRB>(leds, Q_LED);
  /*for (int i = _qLed/2; i >= 0; i--) 
  {
     leds[i] = CRGB (_iniR,_iniG,_iniB);
     leds[_qLed-i] = CRGB (_iniR,_iniG,_iniB );
     //delay(40);
    FastLED.show();
  }*/
}

/**
 * @brief Sets the whole Stripe to the selected color
 * @param colorCode Color to be set
 */
void AC_LEDStripe_1::setFromSensor(int sensorRead){
	  //sensorRead=analogRead(A0);
	  //sensorRead = map(analogRead(34], 0, 255, 0, 4096);
	  sensorRead=sensorRead*2;
	 // Serial.println(sensorRead);
	 // delay(50);
	  if((sensorRead>=450)&&(sensorRead<=550))
	  {
		leds[(_qLed/2)-1]=CRGB (gamma8[0], gamma8[0], gamma8[255]);
		leds[_qLed/2]=CRGB (gamma8[0], gamma8[0], gamma8[255]);
	  }
	  else if((sensorRead>=400)&&(sensorRead<=450))
	  {
		leds[(_qLed/2)-1]=CRGB (gamma8[153], gamma8[153], gamma8[0]);
		leds[_qLed/2]=CRGB (gamma8[153], gamma8[153], gamma8[0]);
	  }
	  else if((sensorRead>=350)&&(sensorRead<=400))
	   {
		 leds[(_qLed/2)-1]=CRGB (gamma8[200], gamma8[50], gamma8[200]);
		leds[_qLed/2]=CRGB (gamma8[200], gamma8[50], gamma8[200]);
	   }
	   else if((sensorRead>=300)&&(sensorRead<=350))
	  {
		leds[(_qLed/2)-1]=CRGB (gamma8[10], gamma8[25], gamma8[217]);
		leds[_qLed/2]=CRGB (gamma8[10], gamma8[25], gamma8[217]);
	  }

		else if((sensorRead>=276)&&(sensorRead<=300))
	   {
		 leds[(_qLed/2)-1]=CRGB (gamma8[50], gamma8[50], gamma8[150]);
		leds[_qLed/2]=CRGB (gamma8[50], gamma8[50], gamma8[150]);
	   }
	   else if((sensorRead>=250)&&(sensorRead<=275))
	   {
		 leds[(_qLed/2)-1]=CRGB (gamma8[230], gamma8[0], gamma8[10]);
		leds[_qLed/2]=CRGB (gamma8[230], gamma8[0], gamma8[10]);
	   }
	  else if((sensorRead>=235)&&(sensorRead<=250))
	   {
		 leds[(_qLed/2)-1]=CRGB (gamma8[0], gamma8[160], gamma8[0]);
		leds[_qLed/2]=CRGB (gamma8[0], gamma8[160], gamma8[0]);
	   }
	   else if((sensorRead>=200)&&(sensorRead<=235))
	   {
		 leds[(_qLed/2)-1]=CRGB (gamma8[1], gamma8[0], gamma8[1]);
		leds[_qLed/2]=CRGB (gamma8[1], gamma8[0], gamma8[1]);
	   }
	  else
	  {
		 /*leds[(_qLed/2)-1] = CRGB ( r,sensorRead-50,b);
		 leds[_qLed/2] = CRGB ( r,sensorRead-50,b);*/
		 leds[(_qLed/2)-1] = CRGB ( gamma8[_iniR],gamma8[_iniG],gamma8[_iniB]);
		 leds[_qLed/2] = CRGB ( gamma8[_iniR],gamma8[_iniG],gamma8[_iniB]);
	  }
		for (int i = 0; i <= ((_qLed/2)-2); i++) 
	  {
		 leds[i] = leds[i+1];
		 leds[_qLed-1-i] = leds[(_qLed)-i-2];
		 
	  }
	 FastLED.show();
}
