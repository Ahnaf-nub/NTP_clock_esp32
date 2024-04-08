#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUDP.h>
#include "miku.h"
#include "7seg20.h"
#include "ghost.h"
#include "Orbitron_Medium_16.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite arrowSprite = TFT_eSprite(&tft);
TFT_eSprite background = TFT_eSprite(&tft);
int x = 0;
#define grey 0x65DB

const char* ssid     = "Mahir";
const char* password = "Ahnaf767";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 5;
int frame = 0;
String arr_days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  tft.init();
  tft.setRotation(3);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  background.createSprite(160, 80);
  arrowSprite.createSprite(32, 32);
  arrowSprite.setSwapBytes(true);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(3, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, 80);
  tft.fillScreen(TFT_BLACK);
  clk();
}

String formattedDate;
String dayStamp;
String timeStamp;
String time2 = "";
String date2 = "";
String year2 = "";

void clk() {
  connectToWifi();
  digitalWrite(LED_BUILTIN, LOW);
  timeClient.begin();
  timeClient.setTimeOffset(21600);
  while (1) {
    unsigned long currentTime = millis();
    while (!timeClient.update()) {
      timeClient.forceUpdate();
    }
    int day = timeClient.getDay();
    formattedDate = timeClient.getFormattedDate();
    int splitT = formattedDate.indexOf("T");
    dayStamp = formattedDate.substring(0, splitT);
    timeStamp = formattedDate.substring(splitT + 1, formattedDate.length() - 1);
    tft.setCursor(0, 15);
    tft.setFreeFont(&DSEG7_Classic_Bold_30);
    if (timeStamp.substring(0, 5) != time2) {
      time2 = timeStamp.substring(0, 5);
      tft.drawString(time2, 0, 45);
    }

    tft.setTextColor(grey, TFT_BLACK);
    tft.setFreeFont(&Orbitron_Medium_16);
    if (date2 != dayStamp.substring(5, 10)) {
      date2 = dayStamp.substring(5, 10);
      tft.drawString(date2, 0, 22);
      tft.setTextColor(0xD340, TFT_BLACK);
      tft.setFreeFont(&Orbitron_Medium_16);
      tft.drawString(arr_days[day], 0, 1);
      tft.setTextFont(1);
      tft.setTextColor(TFT_CYAN, TFT_BLACK);
    }

    tft.setTextFont(1);

    if (year2 != dayStamp.substring(0, 4)) {
      year2 = dayStamp.substring(0, 4);
      tft.drawString(year2, 66, 26, 1);
    }
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setFreeFont(&Orbitron_Medium_16);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.pushImage(115, 10, 40, 48, miku[frame]);
    frame++;
    if (frame == 20)
      frame = 0;
    delay(80);
  }
}

void connectToWifi() {
  WiFi.begin(ssid, password);
  tft.setTextFont(2);
  while (WiFi.status() != WL_CONNECTED) {
    background.fillSprite(TFT_BLACK);
    arrowSprite.pushImage(0, 0, 32, 32, ghost);
    arrowSprite.pushToSprite(&background, x, 20, TFT_BLACK);
    background.pushSprite(0, 0);
    tft.setFreeFont(&Orbitron_Medium_16);
    tft.drawString("Connecting...", 0, 55);
    x++;
    if (x > 180)
      x = -20;
    delay(50);
  }
  // Print local IP address and start web server
  tft.fillScreen(TFT_BLACK);
  tft.println("");
  tft.setFreeFont(&Orbitron_Medium_16);
  tft.drawString("WiFi connected.", 0, 30);
  delay(2000);
  digitalWrite(LED_BUILTIN, HIGH);
  tft.fillScreen(TFT_BLACK);
}

void loop() {
}
