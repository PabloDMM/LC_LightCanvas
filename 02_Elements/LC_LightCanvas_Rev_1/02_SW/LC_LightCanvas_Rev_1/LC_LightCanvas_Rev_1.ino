/***********************************************************/
// Demo for the Serial MP3 Player Catalex (YX5300 chip)
// Hardware: Serial MP3 Player *1
// Board:  Arduino UNO
// http://www.dx.com/p/uart-control-serial-mp3-music-player-module-for-arduino-avr-arm-pic-blue-silver-342439#.VfHyobPh5z0
//
//
//
//#include "ART_Collage_Melody.h"
//#include "RedMP3.h"
//#include <Adafruit_NeoPixel.h>
#include "AC_LEDStripe_0.h"
#include "AC_LEDStripe_1.h"
#include "AC_LEDStripe_2.h"
#include "AC_LEDStripe_3.h"
#include "AC_LEDStripe_4.h"
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#include <SoftwareSerial.h>

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

const char* ssid = "Pablin";
const char* password = "ayno1992";

AsyncWebServer server(80);

// PIN DEFINITION
#define SEN_0       34 // GPIO34          
#define SEN_1       35 // GPIO35 

#define LED_0       15 // GPIO15
#define LED_1       2  // GPIO2
#define LED_2       4  // GPIO4

#define SW_0        5  // GPIO5
#define SW_1        18 // GPIO18
#define SW_2        23 // GPIO23

#define RXD2        16  // GPIO 16
#define TXD2        17  // GPIO 17

// RELAX COLOR DEFINITION
#define STR_0_R     100
#define STR_0_G     100
#define STR_0_B     100

#define STR_1_R     80
#define STR_1_G     80
#define STR_1_B     80

#define STR_2_R     70
#define STR_2_G     70
#define STR_2_B     70

#define STR_3_R     60
#define STR_3_G     60
#define STR_3_B     60

#define STR_4_R     50
#define STR_4_G     50
#define STR_4_B     50

#define STR_5_R     40
#define STR_5_G     40
#define STR_5_B     40

// SERIAL COMMANDS FOR MP3 PLAYER
#define CMD_NEXT_SONG     0X01  // Play next song.
#define CMD_PREV_SONG     0X02  // Play previous song.
#define CMD_PLAY_W_INDEX  0X03
#define CMD_VOLUME_UP     0X04
#define CMD_VOLUME_DOWN   0X05
#define CMD_SET_VOLUME    0X06

#define CMD_SNG_CYCL_PLAY 0X08  // Single Cycle Play.
#define CMD_SEL_DEV       0X09
#define CMD_SLEEP_MODE    0X0A
#define CMD_WAKE_UP       0X0B
#define CMD_RESET         0X0C
#define CMD_PLAY          0X0D
#define CMD_PAUSE         0X0E
#define CMD_PLAY_FOLDER_FILE 0X0F

#define CMD_STOP_PLAY     0X16
#define CMD_FOLDER_CYCLE  0X17
#define CMD_SHUFFLE_PLAY  0x18 //
#define CMD_SET_SNGL_CYCL 0X19 // Set single cycle.

#define CMD_SET_DAC 0X1A
#define DAC_ON  0X00
#define DAC_OFF 0X01

#define CMD_PLAY_W_VOL    0X22
#define CMD_PLAYING_N     0x4C
#define CMD_QUERY_STATUS      0x42
#define CMD_QUERY_VOLUME      0x43
#define CMD_QUERY_FLDR_TRACKS 0x4e
#define CMD_QUERY_TOT_TRACKS  0x48
#define CMD_QUERY_FLDR_COUNT  0x4f

/************ Opitons **************************/
#define DEV_TF            0X02

// CONSTANT VALUES FOR USE
const bool OFF                  = 0;
const bool ON                   = 1;
const int SW_THR_TACTILE        = 200;
const int SW_THR_SHARP          = 450;

//MP3 mp3(RXD2, TXD2);
bool SWONOFF          = OFF;
bool SystemONOFF      = OFF;
int seqStep           = 0;

int8_t volume = 0x1a;//0~0x1e (30 adjustable level)
int8_t folderName = 0x01;//folder name must/ be 01 02 03 04 ...
int8_t fileName = 0x01; // prefix of file name must be 001xxx 002xxx 003xxx 004xxx ...

// Select storage device to TF card
static uint8_t select_SD_card[] = {0x7e, 0x03, 0X35, 0x01, 0xef}; // 7E 03 35 01 EF
// Play with index: /01/001xxx.mp3
static uint8_t play_first_song[] = {0x7e, 0x04, 0x41, 0x00, 0x01, 0xef}; // 7E 04 41 00 01 EF
// Play with index: /01/002xxx.mp3
static uint8_t play_second_song[] = {0x7e, 0x04, 0x41, 0x00, 0x02, 0xef}; // 7E 04 41 00 02 EF
// Play the song.
static uint8_t mp3Play[] = {0x7e, 0x02, 0x01, 0xef}; // 7E 02 01 EF
// Pause the song.
static uint8_t mp3Pause[] = {0x7e, 0x02, 0x02, 0xef}; // 7E 02 02 EF

// GlobalVariables

const int nSTR = 5;
// Define each of the Led Stripes
AC_LEDStripe_0 LEDSTR_0 = AC_LEDStripe_0(STR_0_R, STR_0_G, STR_0_B);
AC_LEDStripe_1 LEDSTR_1 = AC_LEDStripe_1(STR_1_R, STR_1_G, STR_1_B);
AC_LEDStripe_2 LEDSTR_2 = AC_LEDStripe_2(STR_2_R, STR_2_G, STR_2_B);
AC_LEDStripe_3 LEDSTR_3 = AC_LEDStripe_3(STR_3_R, STR_3_G, STR_3_B);
AC_LEDStripe_4 LEDSTR_4 = AC_LEDStripe_4(STR_4_R, STR_4_G, STR_4_B);
//AC_LEDStripe_5 LEDSTR_5 = AC_LEDStripe_5(STR_5_R, STR_5_G, STR_5_B);

void setup()
{
  Serial.begin(9600);
/*
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

 // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! This is a sample response.");
  });

  AsyncElegantOTA.begin(&server);    // Start AsyncElegantOTA
  server.begin();
  Serial.println("HTTP server started");
  */
  Serial.println("LC_LighCanvas SETUP START");
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  analogReadResolution(10);
  delay(500);
  LED_initialization();
  delay(500);
}


void loop()
{
  int sensorBig = 0;
  int sensorSmall = 0;
  send_command_to_MP3_player(mp3Play, 4);
  for(int i = 0; i < 2030; i++){
    sensorBig = analogRead(SEN_1);
    sensorSmall = analogRead(SEN_0);
    Serial.println(i);
    LEDSTR_0.setFromSensor(sensorBig);
    LEDSTR_1.setFromSensor(sensorSmall);
    LEDSTR_2.setFromSensor(sensorBig);
    LEDSTR_3.setFromSensor(sensorSmall);
    LEDSTR_4.setFromSensor(sensorBig);
    //LEDSTR_5.setFromSensor(sensorSmall);
    //delay(10);
  }
  //send_command_to_MP3_player(mp3Pause, 4);
  //delay(1000);
  /*
  switch (SystemONOFF){
    case ON:
      if(seqStep < nSTEPS){
        sequenceContinue();
        if(readStartSwitch_Sharp()){
          sequenceEnd();
          SystemONOFF = OFF;
        }
      }else{
        sequenceEnd();
        SystemONOFF = OFF;
      }
      delay(330);
      break;
    default: // OFF
      if(readStartSwitch_Sharp()){
        SystemONOFF = ON;
        sequenceStart();
      }
      delay(500);
      break;
  }*/
}

void send_command_to_MP3_player(uint8_t command[], int len){
  Serial.print("\nMP3 Command => ");
  for(int i=0;i<len;i++){ Serial2.write(command[i]); Serial.print(command[i], HEX); }
  delay(1000);
}

void displaySound(){
  
}

bool readStartSwitch_Tactile(){
   /*switch (SWONOFF){
    case ON:
      if(analogRead(A0) < SW_THR_TACTILE){
        SWONOFF = OFF;
        return OFF;
      }
      break;
    default: // OFF
      if(analogRead(A0) > SW_THR_TACTILE){
        SWONOFF = ON;
        return ON;
      }
      break;
  }
  return SWONOFF;*/
  return false;
}

bool readStartSwitch_Sharp(){
  //int avrAnalogRead = ((analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1)+analogRead(A1)) / 10);
  /*int avrAnalogRead = 0;
  Serial.println(avrAnalogRead);
  switch (SWONOFF){
    case ON:
      if(avrAnalogRead < SW_THR_SHARP){
        SWONOFF = OFF;
        return OFF;
      }
      break;
    default: // OFF
      if(avrAnalogRead > SW_THR_SHARP){
        SWONOFF = ON;
        return ON;
      }
      break;
  }
  return SWONOFF;*/
  return false;
}

void LED_initialization(){
  LEDSTR_0.init();
  LEDSTR_1.init();
  LEDSTR_2.init();
  LEDSTR_3.init();
  LEDSTR_4.init();
  //LEDSTR_5.init();
}

void sequenceEnd(){
  /*mp3.pause();
  seqStep = nSTEPS;
  setallLed_noStep(0, 0, 0);*/
}

void sequenceStart(){
  /*mp3.setVolume(volume);
  delay(50);//you should wait for >=50ms between two commands
  mp3.playWithFileName(folderName,fileName);
  seqStep = 0;
  setLed_Melody(seqStep); */ 
}

void sequenceContinue(){
  /*seqStep++;
  setLed_Melody(seqStep);*/   
}

/*
 * @function    void blinkLed()
 * @description Blinks the LED in white for 100ms and returns to the previos color
 * @param       none
 * @return      none
 */
void LED_blinkLedStr(int STR){
    /*setallLed_noStep(255,255,255);
    delay(500);
    setallLed_noStep(0,0,0);
    delay(500);*/
}

/*
 * @function    void setLed_Melody(int mStep)
 * @description Sets all the LEDs in stripe to selected Melody Step
 * @param       int mStep
 * @return      none
 */
void setLed_Melody(int mStep){
    /*for(int i=0; i<LEDn; i++){
      //ledRGB.setPixelColor(i, MED[mStep][i], MED[mStep][i], MED[mStep][i]);
    }
    //ledRGB.show();*/
}
