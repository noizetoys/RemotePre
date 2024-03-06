// Data Display


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

struct MicPreData {
  int gainLevel;
  bool phaseState;
  bool phantomPowerState;
  bool inputImpedanceState;
  bool padState;
  bool highPassFilterState;
};

const int gainValues[] = {25, 29, 33, 36, 39, 42, 45, 48, 51, 54, 57, 60};

class DataDisplay {
public:
  DataDisplay(Adafruit_SSD1306& _display, MicPreData _data);
  void updateDisplay();
  void displayDeviceID(int id);

private:
  Adafruit_SSD1306& display;
  MicPreData data;

  void resetText();
  void displayPhantom();
  void displayPolarity();
  void displayInputZ();
  void displayPad();
  void displayHPF();
  void displayGain();
  void displayAllItems();
  void displayMute();
};

DataDisplay::DataDisplay(Adafruit_SSD1306& _display, MicPreData _data) {
  display = _display;
  data = _data;

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  displayAllItems();
  delay(2000);
}

void DataDisplay::updateDisplay() {
  if (!data.phaseState) {
    displayMute();
  } else {
    displayAllItems();
  }
}

void DataDisplay::displayDeviceID(int id) {
  display.clearDisplay();
  display.invertDisplay(false);
  display.setTextColor(WHITE);
  display.setTextSize(5);
  display.setFont(NULL);

  if (id < 10) {
    display.setCursor(36, 23);
  } else {
    display.setCursor(20, 23);
  }

  display.print("#");
  display.println(id);
  display.display();
  delay(2000);
}

void DataDisplay::resetText() {
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setFont(NULL);
}

void DataDisplay::displayPhantom() {
  resetText();
  display.setCursor(1, 1);
  if (data.phantomPowerState) {
    display.println("48v");
  }
}

void DataDisplay::displayPolarity() {
  resetText();
  if (data.phaseState) {
    display.drawCircle(63, 8, 6, WHITE);
    display.drawCircle(63, 8, 7, WHITE);
    display.drawLine(54, 15, 70, 0, WHITE);
    display.drawLine(55, 15, 71, 0, WHITE);
    display.drawLine(56, 15, 72, 0, WHITE);
  }
}

void DataDisplay::displayInputZ() {
  resetText();
  display.setCursor(92, 1);
  display.println(data.inputImpedanceState ? "HiZ" : "LoZ");
}

void DataDisplay::displayPad() {
  resetText();
  if (data.padState) {
    display.setCursor(92, 26);
    display.println("-20");
  }
}

void DataDisplay::displayHPF() {
  resetText();
  if (data.highPassFilterState) {
    display.drawLine(92, 64, 100, 52, WHITE);
    display.drawFastHLine(100, 52, 27, WHITE);
  }
}

void DataDisplay::displayGain() {
  display.setTextSize(3);
  display.setCursor(1, 35);
  display.println("+");
  display.setTextSize(5);
  display.setCursor(20, 28);
  display.println(gainValues[data.gainLevel]);
}

void DataDisplay::displayAllItems() {
  display.clearDisplay();
  display.invertDisplay(false);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setFont(NULL);

  displayPhantom();
  displayPolarity();
  displayInputZ();
  displayPad();
  displayHPF();
  displayGain();

  display.display();
}

void DataDisplay::displayMute() {
  display.clearDisplay();
  display.invertDisplay(true);
  display.drawRect(0, 0, 128, 16, WHITE);
  display.setTextColor(WHITE);
  display.setTextSize(5);
  display.setFont(NULL);
  display.setCursor(7, 23);
  display.println("MUTE");
  display.display();
  delay(2000);
}