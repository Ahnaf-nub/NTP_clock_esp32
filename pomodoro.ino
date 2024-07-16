#include <TFT_eSPI.h>
#include <SPI.h>


// TFT display setup
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

// Timer variables
int workTime = 25 * 60;  // 25 minutes
int breakTime = 5 * 60;  // 5 minutes
int remainingTime = workTime;
bool isWorking = true;
bool isPaused = true;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize TFT display
  tft.init();
  tft.setRotation(1);  // Set display rotation
  tft.fillScreen(TFT_BLACK);  // Clear screen with black color
  tft.setTextColor(TFT_WHITE);  // Set text color to white
  tft.setTextSize(2);  // Set text size

  // Initialize touch sensor
  touchAttachInterrupt(T3, touchCallback, 40);

  // Initial display update
  updateDisplay();
}

void loop() {
  if (!isPaused) {
    remainingTime--;
    if (remainingTime <= 0) {
      isWorking = !isWorking;
      remainingTime = isWorking ? workTime : breakTime;
    }
    updateDisplay();
    delay(1000);  // Wait for 1 second
  }
}

void touchCallback() {
  isPaused = !isPaused;
  if (!isPaused && remainingTime == 0) {
    isWorking = true;
    remainingTime = workTime;
  }
  updateDisplay();
}

void updateDisplay() {
  tft.fillScreen(TFT_BLACK);  // Clear screen

  if (isPaused) {
    tft.setCursor(10, 10);
    tft.println("Pomodoro Timer");
    tft.setCursor(10, 40);
    tft.println(isWorking ? "Work Session" : "Break Session");
    tft.setCursor(10, 70);
    tft.println("Paused");
  } else {
    int minutes = remainingTime / 60;
    int seconds = remainingTime % 60;

    tft.setCursor(10, 10);
    tft.println("Pomodoro Timer");
    tft.setCursor(10, 40);
    tft.println(isWorking ? "Work Session" : "Break Session");
    tft.setCursor(10, 70);
    tft.printf("%02d:%02d", minutes, seconds);
  }
}

