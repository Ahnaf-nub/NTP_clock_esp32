#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_BMP085.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#define TFT_CS   12
#define TFT_RST   9 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC   11
#define TFT_MOSI 33  // Data out
#define TFT_SCLK 18  // Clock out
#define TFT_BCK 3 //Backlight
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "bd.pool.ntp.org", 21600);
String arr_days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
String date_time;
String pred;

Adafruit_BMP085 bmp;
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup() {
  Serial.begin(115200);
  Wire.begin(35, 37);
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  }
  pinMode(LED_BUILTIN, OUTPUT);
  tft.initR(INITR_MINI160x80_PLUGIN);  // Init ST7735S mini display
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(3);
  connectToWifi();
  tft.fillScreen(ST77XX_BLACK);
  timeClient.begin();
  timeClient.setTimeOffset(21600); // Set time offset for Bangladesh (UTC+6)
}

void loop() {
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }
  timeClient.update();
  date_time = timeClient.getFormattedDate();
  int index_date = date_time.indexOf("T");
  int day = timeClient.getDay();
  String date = date_time.substring(0, index_date);
  int t = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100;
  Serial.println(pressure);
  if ((pressure <= 1008.0) && (pressure > 998.5)) {
    pred = "Cloudy";
  }
  else if ((pressure <= 998.5) && (pressure > 996.5)) {
    pred = "Rainy";
  }
  else if ((pressure >= 1009.00) && (pressure < 1050)) {
    pred = "Clear sky";
  }
  else if ((pressure >= 990) && (pressure <= 996.4)) {
    pred = "Stormy";
  }
  //time part
  tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 5);
  tft.println(String(timeClient.getFormattedTime()));
  //day part
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setCursor(120, 5);
  tft.println(arr_days[day]);
  //date part
  tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setCursor(20, 32);
  tft.println(date);
  //weather prediction part
  tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 60);
  tft.println(String(pred));
  //temperature part
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setCursor(112, 60);
  tft.println(String(t) + (char)247 + "C");
}


void connectToWifi() {
  tft.setTextSize(2);
  WiFi.begin("", ""); //ssid and pass
  tft.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    tft.print(".");
  }
  tft.fillScreen(ST77XX_BLACK);
  digitalWrite(LED_BUILTIN, LOW);
  tft.println(" ");
  tft.setTextColor(ST77XX_BLUE, ST77XX_BLACK);
  tft.println("WiFi");
  tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);
  tft.println("Connected.");
  delay(2000);
}
