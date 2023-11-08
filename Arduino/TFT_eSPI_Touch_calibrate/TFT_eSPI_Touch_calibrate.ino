/*
  Sketch to generate the setup() calibration values, these are reported to the Serial Monitor.

  The sketch has been tested on the ESP8266 and screen with XPT2046 driver.
*/

#include <SPI.h>
#include <TFT_eSPI.h>      // Hardware-specific library

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

//------------------------------------------------------------------------------------------

void setup() {
  // Use serial port
  Serial.begin(115200);

  // Initialise the TFT screen
  tft.init();

  // Set the rotation to the orientation you wish to use in your project before calibration
  // (the touch coordinates returned then correspond to that rotation only)
  //tft.setRotation(1);

  // Calibrate the touch screen and retrieve the scaling factors
  //touch_calibrate();

/*
  // Replace above line with the code sent to Serial Monitor
  // once calibration is complete, e.g.:
  // Vertikal   bzw. Rotation = 0 bedeutet Touch-Connector und TFT-Pins unten,  TP + SD oben
  // Horizontal bzw. Rotation = 1 bedeutet Touch-Connector und TFT-Pins rechts, TP + SD links
  uint16_t calData[5] = { 330, 3500, 230, 3580, 4 };  // Rotation 0 Ecke links oben geht nicht
  uint16_t calData[5] = { 230, 3580, 330, 3500, 7 };  // Rotation 1 selbe Ecke, aus Sicht der Schrift links unten
  tft.setTouch(calData);
*/
  
  tft.setRotation(0);
  uint16_t calData[5] = { 330, 3500, 230, 3580, 4 };  // xmin, xmax, ymin, ymax, ?
  //tft.setRotation(1);
  //uint16_t calData[5] = { 230, 3580, 330, 3500, 7 };
  tft.setTouch(calData);

  // Clear the screen
  tft.fillScreen(TFT_BLACK);
  tft.drawCentreString("Touch screen test!",tft.width()/2, tft.height()/2, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
}

//------------------------------------------------------------------------------------------

void loop(void) {
  uint16_t x = 0, y = 0; // To store the touch coordinates

  // Pressed will be set true is there is a valid touch on the screen
  bool pressed = tft.getTouch(&x, &y);

  // Draw a white spot at the detected coordinates
  if (pressed) {
    tft.fillCircle(x, y, 1, TFT_YELLOW);
    Serial.print("x,y = ");
    Serial.print(x);
    Serial.print(",");
    Serial.println(y);
     
    tft.setCursor( 25, 25);
    tft.print("x= ");
    tft.print(x);
    tft.print(" y= ");
    tft.print(y);
    tft.println("    ");
  }
}

//------------------------------------------------------------------------------------------

// Code to run a screen calibration, not needed when calibration values set in setup()
void touch_calibrate()
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // Calibrate
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 0);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.println("Touch corners as indicated");

  tft.setTextFont(1);
  tft.println();

  tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

  Serial.println(); Serial.println();
  Serial.println("// Use this calibration code in setup():");
  Serial.print("  uint16_t calData[5] = ");
  Serial.print("{ ");

  for (uint8_t i = 0; i < 5; i++)
  {
    Serial.print(calData[i]);
    if (i < 4) Serial.print(", ");
  }

  Serial.println(" };");
  Serial.print("  tft.setTouch(calData);");
  Serial.println(); Serial.println();

  tft.fillScreen(TFT_BLACK);
  
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Calibration complete!");
  tft.println("Calibration code sent to Serial port.");

  delay(2000);
}

