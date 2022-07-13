SYSTEM_THREAD(ENABLED);

#include "SdFat.h"
#include "Adafruit_VS1053.h"
#include "Adafruit_LiquidCrystal.h"
#include <blynk.h>
#include <SPI.h>

SdFat SD;
const int MP3_RESET = -1; // VS1053 reset pin (unused!)
const int SD_CS = D2;     // SD Card chip select pin
const int MP3_CS = D3;    // VS1053 chip select pin (output)
const int DREQ = D4;      // VS1053 Data request, ideally an Interrupt pin
const int MP3_DCS = D5;   // VS1053 Data/command select pin (output)

LiquidCrystal lcd(0);

int fileNum = 0;
bool music = false;
unsigned long timerOfMine = millis();

int fileScroll = 0;
unsigned long scrollTime = millis();
int fileScrollDiff = 16;

unsigned long secondLineTime = millis();
bool secondLine = true;
String lineTwo = "Loading...";

unsigned long songTimer;

long fileTimes[7] = {302, 273, 263, 280, 249, 346, 225};

int fileBpm[7] = {176, 42, 63, 120, 88, 88, 69};

String files[7] = {"1.mp3", "2.mp3", "3.mp3", "4.mp3", "5.mp3", "6.mp3", "7.mp3"};

String fileNames[7] = {"Hard Rock Backing Track", "Piano Backing Track", "Acoustic Backing Track", "Funk Backing Track", "R n B Backing Track", "Soft Rock Backing Track", "Orchestral Backing Track"};

Adafruit_VS1053_FilePlayer musicPlayer(MP3_RESET, MP3_CS, MP3_DCS, DREQ, SD_CS);

BLYNK_WRITE(V0)
{
  if (millis() >= timerOfMine + 2000)
  {
    musicPlayer.pausePlaying(music);
    music = !music;
    timerOfMine = millis();
  }
}

BLYNK_WRITE(V2)
{
  if (millis() >= timerOfMine + 2000)
  {
    playMusic();
    timerOfMine = millis();
  }
}

BLYNK_WRITE(V1)
{
  if (millis() >= timerOfMine + 2000)
  {
    fileNum = param.asInt() - 1;
    playMusic();
    timerOfMine = millis();
  }
}

void setup()
{
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);

  Serial.begin(9600);

  pinMode(D7, OUTPUT);

  Blynk.begin("ttCOyuu9x3tXApkPe1m4LMgI5BNNzq5d", IPAddress(167, 172, 234, 162), 8080);

  if (!SD.begin(SD_CS))
  {
    while (1)
      yield(); // don't do anything more
  }
  Serial.println("SD OK!");

  // initialise the music player
  if (!musicPlayer.begin())
  { // initialise the music player
    while (1)
      yield();
  }

  musicPlayer.setVolume(0, 0);

  if (musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT))
  {
    digitalWrite(D7, HIGH);
    musicPlayer.setIsrCallback(blink);
  }

  musicPlayer.startPlayingFile(files[fileNum]);
  musicPlayer.pausePlaying(true);
}

void loop()
{
  Blynk.run();

  if (millis() >= scrollTime + 350)
  {
    lcd.clear();
    lcd.setCursor(fileScrollDiff, 0);
    String toDisect = fileNames[fileNum];
    String toDisplay;
    int nameLength = toDisect.length();

    if (musicPlayer.paused())
    {
      lineTwo = "No Music Playing";
      toDisect = "No Music Playing";
    }
    else
    {
      if (secondLine)
      {
        lineTwo = "BPM: " + String(fileBpm[fileNum]);
      }
      else
      {
        unsigned long playDuration = millis() - songTimer;
        float playSeconds = playDuration / 1000.0 - 0.5; // so it always rounds down
        playDuration = fileTimes[fileNum] - int(playSeconds);
        String extraZero = "";

        if (playDuration < 1)
        {
          playDuration = 0;
        }

        int seconds = playDuration % 60;
        if (int(seconds / 10 - 0.5) == 0)
        {
          extraZero = "0";
        }
        else

          lineTwo = "Time Left: " + String(int((playDuration / 60) - 0.5)) + ":" + extraZero + String(playDuration % 60);
      }
    }

    if (0 < fileScrollDiff)
    {
      unsigned int pos = fileScroll + 16;
      toDisplay = toDisect.substring(0, pos - fileScrollDiff);
      fileScrollDiff--;
    }
    else if (nameLength >= fileScroll)
    {
      unsigned int pos = fileScroll + 16;
      toDisplay = toDisect.substring(fileScroll, pos);
      fileScroll++;
    }
    else
    {
      fileScrollDiff = 16;
      fileScroll = 0;
    }

    lcd.print(toDisplay);
    lcd.setCursor(0, 1);
    lcd.print(lineTwo);
    scrollTime = millis();
  }

  if (millis() >= secondLineTime + 10000)
  {
    secondLineTime = millis();
    secondLine = !secondLine;
  }
}

void playMusic()
{
  if (fileNum == 0)
  {
    musicPlayer.setVolume(5, 5);
  }
  else
  {
    musicPlayer.setVolume(0, 0);
  }
  musicPlayer.pausePlaying(false);
  music = true;
  musicPlayer.startPlayingFile(files[fileNum]);
  songTimer = millis();
}

void blink(void)
{
  digitalWriteFast(D7, !pinReadFast(D7));
}