#include <Arduino.h>
#include <MD_TCS230.h>
#include "LedControl.h"

#define  S0_OUT  2
#define  S1_OUT  3
#define  S2_OUT  4
#define  S3_OUT  5
MD_TCS230 ColorSensor(S2_OUT, S3_OUT, S0_OUT, S1_OUT);

int DIN = 26;
int CLK = 22;
int CS = 24;
int CD = 1;
LedControl ledDisplay = LedControl(DIN, CLK, CS, CD);

void setup() {
    sensorData whiteCalibration;
    whiteCalibration.value[TCS230_RGB_R] = 113290;
    whiteCalibration.value[TCS230_RGB_G] = 99090;
    whiteCalibration.value[TCS230_RGB_B] = 128070;

    sensorData blackCalibration;
    blackCalibration.value[TCS230_RGB_R] = 10580;
    blackCalibration.value[TCS230_RGB_G] = 9420;
    blackCalibration.value[TCS230_RGB_B] = 13500;

    Serial.begin(115200);
    ColorSensor.begin();
    ColorSensor.setDarkCal(&blackCalibration);
    ColorSensor.setWhiteCal(&whiteCalibration);

    ledDisplay.shutdown(0, false);
    ledDisplay.setIntensity(0, 8);
    ledDisplay.clearDisplay(0);
}

void loop() {
    colorData rgb;
    ColorSensor.read();
    while (!ColorSensor.available())
        ;
    ColorSensor.getRGB(&rgb);

    drawValue(rgb);
    delay(1000);

}

void drawValue(colorData rgb) {
  ledDisplay.clearDisplay(0);

  printValue(rgb.value[TCS230_RGB_R], 1);
  printAssignment("R", rgb.value[TCS230_RGB_R]);
  printValue(rgb.value[TCS230_RGB_G], 3);
  printAssignment("G", rgb.value[TCS230_RGB_G]);
  printValue(rgb.value[TCS230_RGB_B], 5);
  printAssignment("B", rgb.value[TCS230_RGB_R]);
}

void printValue(int value, int column) {
  int printValue = value/16;
  for (int i = 0; i <= printValue; i++) {
    ledDisplay.setLed(0, 7 - i, column, true);
    ledDisplay.setLed(0, 7 - i, column - 1, true);
  }
}

void printAssignment(const char* color, int32_t value){
    Serial.print("value[TCS230_RGB_");
    Serial.print(color);
    Serial.print("] = ");
    Serial.print(value);
    Serial.println(";");
}
