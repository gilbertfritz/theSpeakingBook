///              MP3 PLAYER PROJECT
/// http://educ8s.tv/arduino-mp3-player/
//////////////////////////////////////////


#include "SoftwareSerial.h"
SoftwareSerial mySerial(5, 2);
# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]

# define ACTIVATED LOW
# define PAGE_UNDEFINED -1

# define DELAY_LOOP_MS 500;

# define THRESHOLD_SENSORVALUE 500
# define THRESHOLD_PAUSE_ON_UNDEFINED_LOOPS = 4;
# define THRESHOLD_PLAY_ON_DEFINED_LOOPS = 4;

int sensorPin[4] = {A0, A2, A4, A6};
const int sensorPinsSize = sizeof(sensorPin) / sizeof(int);
int sensorValue[sensorPinsSize];

boolean firstPlay = true;
boolean isPlaying = false;

int undefinedPageCount = 0;
int definedPageCount = 0;

void setup () {

  Serial.begin(9600);
  Serial.println("hello");

  for (int i = 0; i < sensorPinsSize; i++) {
    pinMode(sensorPin[i], INPUT);
    sensorValue[i] = 0;
  }

  mySerial.begin (9600);
  delay(1000);

  initPlayer();

}



void loop () {

  int maxValue = 0;
  int actualPage = PAGE_UNDEFINED;
  for (int i = 0; i < sensorPinsSize; i++) {
    sensorValue[i] = analogRead(sensorPin[i]);
    if ( sensorValue[i] > maxValue) {
      maxValue = sensorValue[i];
      actualPage = i;
    }
  }
  //ON PAGE UNDEFINED
  if ( maxValue < THRESHOLD_SENSORVALUE) {
    actualPage = PAGE_UNDEFINED;
    
  }
  //ON PAGE DEFINED
  else {

  }

  Serial.println(actualPage);
  delay(500);

  /*
    if (digitalRead(buttonPause) == ACTIVATED)
    {
      if(isPlaying)
      {
        pause();
        isPlaying = false;
      }else
      {
        isPlaying = true;
        play();
      }
    }


    if (digitalRead(buttonNext) == ACTIVATED)
    {
      if(isPlaying)
      {
        playNext();
      }
    }

     if (digitalRead(buttonPrevious) == ACTIVATED)
    {
      if(isPlaying)
      {
        playPrevious();
      }
    }
  */
}

void playFirst()
{
  Serial.println("playfirst");
  execute_CMD(0x3F, 0, 0);
  delay(500);
  setVolume(20);
  delay(500);
  execute_CMD(0x11, 0, 1);
  delay(500);
}

void initPlayer() {
  Serial.println("init Player");
  execute_CMD(0x3F, 0, 0);
  delay(500);
  setVolume(20);
  delay(500);
}

void setTrackNr( int nr) {
  Serial.println("play track nr " + nr);
  execute_CMD(0x12, nr);
  delay(500);
}

void pause()
{
  Serial.println("pause");
  isPlaying = false;
  execute_CMD(0x0E, 0, 0);
  delay(500);
}

void play()
{
  if ( firstPlay) {
    firstPlay = false;
    Serial.println("play first");
    execute_CMD(0x11, 0, 1);
  } else {
    Serial.println("play");
    execute_CMD(0x0D, 0, 1);
  }

  isPlaying = true;
  delay(500);
}

void playNext()
{
  Serial.println("next pressed");
  execute_CMD(0x01, 0, 1);
  delay(500);
}

void playPrevious()
{
  Serial.println("previous pressed");
  execute_CMD(0x02, 0, 1);
  delay(500);
}

void setVolume(int volume)
{
  execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
  delay(2000);
}

void execute_CMD(byte CMD, byte Par1, byte Par2)
// Excecute the command and parameters
{
  // Calculate the checksum (2 bytes)
  word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
  // Build the command line
  byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
                            Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte
                          };
  //Send the command line to the module
  for (byte k = 0; k < 10; k++)
  {
    mySerial.write( Command_line[k]);
  }
}
