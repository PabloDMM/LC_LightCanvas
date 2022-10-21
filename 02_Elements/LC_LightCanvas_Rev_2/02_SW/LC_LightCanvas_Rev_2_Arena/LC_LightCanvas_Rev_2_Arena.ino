

#include <Arduino.h>
//#include <SoftwareSerial.h>
//#include "AltSoftSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <AceButton.h>
#include "esp_adc_cal.h"
#include "AC_LEDStripe_0.h"
#include "AC_LEDStripe_1.h"
#include "AC_LEDStripe_2.h"
#include "AC_LEDStripe_3.h"
#include "AC_LEDStripe_4.h"
#include "AC_LEDStripe_5.h"

using namespace ace_button;

// ADC Filtering
#define AN_Pot1     36
#define FILTER_LEN  5

// PIN DEFINITION
#define SW_00_P       5
#define SW_01_P       18
#define LED_00_P      19
#define LED_01_P      23
#define POT_VOL_P     4
#define POT_SEN_P     2
#define SEN_SPK_P     36
#define SEN_0_P       34
#define SEN_1_P       35
#define LED_STR_0_P   33
#define LED_STR_1_P   25
#define LED_STR_2_P   26
#define LED_STR_3_P   27
#define LED_STR_4_P   32
#define LED_STR_5_P   13
#define MP3_RX_P      16
#define MP3_TX_P      17

// RELAX COLOR DEFINITION
#define STR_0_R     40
#define STR_0_G     40
#define STR_0_B     40

#define STR_1_R     40
#define STR_1_G     40
#define STR_1_B     40

#define STR_2_R     50
#define STR_2_G     50
#define STR_2_B     50

#define STR_3_R     60
#define STR_3_G     60
#define STR_3_B     60

#define STR_4_R     70
#define STR_4_G     70
#define STR_4_B     70

#define STR_5_R     80
#define STR_5_G     80
#define STR_5_B     80

HardwareSerial mySoftwareSerial(1);
DFRobotDFPlayerMini myDFPlayer;

AceButton playStopSwitch(SW_00_P);
// Forward reference to prevent Arduino compiler becoming confused.
void setPlayStopSw(AceButton*, uint8_t, uint8_t);

bool nowPlaying = false;
bool loopForPlaying = false;

// ADC Filtering
uint32_t AN_Pot1_Buffer[FILTER_LEN] = {0};
int AN_Pot1_i = 0;
int AN_Pot1_Raw = 0;
int AN_Pot1_Filtered = 0;
int generalSensibility = 10;
int oldVolume = 0;
const int nSTR = 5;
// Define each of the Led Stripes
AC_LEDStripe_0 LEDSTR_0 = AC_LEDStripe_0(STR_0_R, STR_0_G, STR_0_B);
AC_LEDStripe_1 LEDSTR_1 = AC_LEDStripe_1(STR_1_R, STR_1_G, STR_1_B);
AC_LEDStripe_2 LEDSTR_2 = AC_LEDStripe_2(STR_2_R, STR_2_G, STR_2_B);
AC_LEDStripe_3 LEDSTR_3 = AC_LEDStripe_3(STR_3_R, STR_3_G, STR_3_B);
AC_LEDStripe_4 LEDSTR_4 = AC_LEDStripe_4(STR_4_R, STR_4_G, STR_4_B);
AC_LEDStripe_5 LEDSTR_5 = AC_LEDStripe_5(STR_4_R, STR_5_G, STR_5_B);
void setup()
{
  // Setting the objects
  Serial.begin(115200);
  mySoftwareSerial.begin(9600, SERIAL_8N1, 16, 17);  // speed, type, RX, TX
  // Setup the SW
  pinMode(SW_00_P, INPUT_PULLUP);
  pinMode(SW_01_P, INPUT_PULLUP);
  pinMode(SEN_SPK_P, INPUT);
  playStopSwitch.setEventHandler(setPlayStopSw);
  // Setup the LEDS
  pinMode(LED_00_P, OUTPUT);
  pinMode(LED_01_P, OUTPUT);
  digitalWrite(LED_00_P, LOW);
  digitalWrite(LED_01_P, LOW);
  myDFPlayer_Setup(); // Setup the DFPlayer

  analogReadResolution(10);
  delay(500);
  LED_initialization();
  delay(500);

  //generalSensibility = setSensibilityThroughPotSen();
  //loopForPlaying = setLoopSw();
  
}

void loop()
{
  loopForPlaying = setLoopSw();
  oldVolume = setVolumeThroughPotVol();
  generalSensibility = setSensibilityThroughPotSen();
  playStopSwitch.check();
  
  // ADC Filtering
  AN_Pot1_Raw = analogRead(AN_Pot1);
  int valueForStripe = 0;
  if((AN_Pot1_Raw - 550) < 0){
    valueForStripe = 0;
  }else{
     valueForStripe = ((AN_Pot1_Raw - 550) * generalSensibility); // Print Filtered Output
  }
  
  LEDSTR_0.setFromSensor(valueForStripe);
  LEDSTR_1.setFromSensor(valueForStripe);
  LEDSTR_2.setFromSensor(valueForStripe);
  LEDSTR_3.setFromSensor(valueForStripe);
  LEDSTR_4.setFromSensor(valueForStripe);
  LEDSTR_5.setFromSensor(valueForStripe);
  //delay(10);
}

/**
 * @brief Setup of the DFPlayer
 */
void myDFPlayer_Setup(){
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  delay(2000);
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    
    Serial.println(myDFPlayer.readType(),HEX);
  }
  myDFPlayer.setTimeOut(1000); //Set serial communictaion time out 500ms
  //----Set volume----
  myDFPlayer.volume(5);  //Set volume value (0~30).
  myDFPlayer.volumeUp(); //Volume Up
  myDFPlayer.volumeDown(); //Volume Down
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
}

/**
 * @brief Sets a new volumen through the Pot Vol value measuring 0-4096 to 0-30
 * @returns New volume value 0-30
 */
int setVolumeThroughPotVol(){
  int newVolume = map(analogRead(POT_VOL_P),0,1024,0,30);
  if(oldVolume != newVolume){
    myDFPlayer.volume(newVolume);
    
  }
  return newVolume;
}

/**
 * @brief Sets a new sensibility coefficient the Pot Sen value measuring 0-4096 to 0-100
 * @returns New volume value 0-100
 */
int setSensibilityThroughPotSen(){
  int sensibilityParam = map(analogRead(POT_SEN_P),0,1024,0,10);
  return sensibilityParam;
}

/**
 * @brief Controls the Stop & Play through playStopSwitch
 * @param Unused
 * @param Unused
 */
void setPlayStopSw(AceButton* /* button */, uint8_t eventType, uint8_t /* buttonState */) {
  switch (eventType) {
    case AceButton::kEventPressed:
      if(!nowPlaying){
        if(!loopForPlaying){
          myDFPlayer.play(1);  //Play the first mp3
        }else{
          myDFPlayer.loop(1);  //Play the first mp3
        }
        nowPlaying = true;
        digitalWrite(LED_00_P, HIGH);
      }else{
        myDFPlayer.stop();
        nowPlaying = false;
        digitalWrite(LED_00_P, LOW);
      }
      break;
    case AceButton::kEventReleased:
      //digitalWrite(LED_PIN, LED_OFF);
      break;
  }
}

/**
 * @brief Setsthe LOOP Option for the Panel
 */
boolean setLoopSw(){
  if(!digitalRead(SW_01_P)){
    digitalWrite(LED_01_P, HIGH);
    return true;
  }else{
    digitalWrite(LED_01_P, LOW);
    return false;
  }
}

/**
 * @brief ADC softener
 * @param ADC_Raw Raw measure from the ADC
 */
uint32_t readADC_Avg(int ADC_Raw){
  int i = 0;
  uint32_t Sum = 0;
  
  AN_Pot1_Buffer[AN_Pot1_i++] = ADC_Raw;
  if(AN_Pot1_i == FILTER_LEN)
  {
    AN_Pot1_i = 0;
  }
  for(i=0; i<FILTER_LEN; i++)
  {
    Sum += AN_Pot1_Buffer[i];
  }
  return (Sum/FILTER_LEN);
}

/**
 * @brief Initializes the LED Stripes
 */
void LED_initialization(){
  LEDSTR_0.init();
  LEDSTR_1.init();
  LEDSTR_2.init();
  LEDSTR_3.init();
  LEDSTR_4.init();
  LEDSTR_5.init();
}
