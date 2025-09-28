#include <Arduino.h>
#include "DFRobotDFPlayerMini.h"

#include <SoftwareSerial.h>
SoftwareSerial softSerial(/*rx =*/4, /*tx =*/5);
#define FPSerial softSerial


DFRobotDFPlayerMini dfPlayer;
void dfpm_printDetail(uint8_t type, int value);


void dfpm_setup(int volume) {
  FPSerial.begin(9600);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!dfPlayer.begin(FPSerial, /*isACK = */ true, /*doReset = */ true)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true)
      ;
  }
  Serial.println(F("DFPlayer Mini online."));

  delay(200);

  dfPlayer.setTimeOut(1000);  //Set serial communictaion time out 500ms

  //----Set volume----
  Serial.print("Setting volume to: ");
  Serial.println(volume);
  dfPlayer.volume(volume);  //Set volume value (0~30).

  //----Set different EQ----
  // dfPlayer.EQ(DFPLAYER_EQ_NORMAL);
  //  dfPlayer.EQ(DFPLAYER_EQ_POP);
  //  dfPlayer.EQ(DFPLAYER_EQ_ROCK);
  //  dfPlayer.EQ(DFPLAYER_EQ_JAZZ);
  //  dfPlayer.EQ(DFPLAYER_EQ_CLASSIC);
  dfPlayer.EQ(DFPLAYER_EQ_BASS);

  dfPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  dfPlayer.enableDAC();              //Enable On-chip DAC
  dfPlayer.outputSetting(true, 15);  //output setting, enable the output and set the gain to 15
  dfPlayer.disableLoopAll();         //stop loop all mp3 files.
  dfPlayer.disableLoop();            //disable loop.
}

int dfpm_is_playing() {
  return dfPlayer.readState();
}


void dfpm_play_track(int trackNr) {

  dfPlayer.playMp3Folder(trackNr);  //play specific mp3 in SD:/MP3/0004.mp3; File Name(0~65535)

  unsigned long startTime = millis();
  while (millis() - startTime < 100) {  // Listen for up to 100ms
    if (dfPlayer.available()) {
      // We received a response, print it for debugging and then exit the loop.
      dfpm_printDetail(dfPlayer.readType(), dfPlayer.read());
      break;
    }
  }
}

void dfpm_check_status() {
  if (dfPlayer.available()) {
    dfpm_printDetail(dfPlayer.readType(), dfPlayer.read());  //Print the detail message from DFPlayer to handle different errors and states.
  }
}



void dfpm_printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}