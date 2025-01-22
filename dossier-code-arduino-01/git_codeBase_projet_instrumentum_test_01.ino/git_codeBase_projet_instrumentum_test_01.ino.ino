#include <M5Atom.h>

CRGB pixel;  // int bob = 10;
unsigned long monChronoMessage; //commence automatiquement à 0

#define MA_BROCHE_LIGHT 0
/*
#define MA_BROCHE_ANGLE 32 //const int MA_BROCHE_ANGLE = 32;

#define PIN_NEO_PIXEL  4   // Arduino pin that connects to NeoPixel
#define NUM_PIXELS     32 // The number of LEDs (pixels) on NeoPixel

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);
*/
#include <M5_PbHub.h>
M5_PbHub myPbHub;

#include <VL53L0X.h>
VL53L0X myTOF;

#include <MicroOscSlip.h>
MicroOscSlip<128> monOsc(&Serial);

void setup() {
  // put your setup code here, to run once:
  M5.begin(false, false, false);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(&pixel, 1);  // Ajouter le pixel du M5Atom à FastLED
  Serial.begin(115200);  //demare communication avec ordi

  //NeoPixel.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  unsigned long chronoDepart = millis();
  while (millis() - chronoDepart < 5000) {
    pixel = CRGB(1,255,251);
    FastLED.show();
    delay(100);
    
    pixel = CRGB(0,0,0);
    FastLED.show();
    delay(100);
  }

  Wire.begin();
  myPbHub.begin();

  myTOF.init();
  myTOF.setTimeout(500);
  myTOF.startContinuous();
}
/*
void maReceptionMessageOsc(MicroOscMessage& oscMessage) {
  if (oscMessage.checkOscAddress("/rgb")) {
    int r = oscMessage.nextAsInt();
    int g = oscMessage.nextAsInt();
    int b = oscMessage.nextAsInt();
    pixel = CRGB(r,g,b);
    FastLED.show();
  }
}
*/
void loop() {
  // put your main code here, to run repeatedly:
  M5.update();
  //monOsc.onOscMessageReceived(maReceptionMessageOsc);

  if (millis() - monChronoMessage >= 25) {
    monChronoMessage = millis();
    // à chaque 50ms
    bool maLectureBouton = M5.Btn.isPressed();

    int maLectureLumiere = myPbHub.analogRead(MA_BROCHE_LIGHT);

    uint16_t valueTof = myTOF.readRangeContinuousMillimeters();

    monOsc.sendInt("/tof", valueTof);

    monOsc.sendInt("/btn", maLectureBouton);
    monOsc.sendInt("/lumiere", maLectureLumiere);
    //Serial.println(maLectureBouton);
    /*
    int maLectureAngle = analogRead(MA_BROCHE_ANGLE);

    for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {           // for each pixel
    NeoPixel.setPixelColor(pixel, NeoPixel.Color(0, 255, 0));  // it only takes effect if pixels.show() is called
    NeoPixel.show();                                           // send the updated pixel colors to the NeoPixel hardware.

    delay(DELAY_INTERVAL);  // pause between each pixel
    /*
    Serial.print(maLectureBouton);
    Serial.print(" ");
    Serial.println(maLectureAngle); //fin ligne
    */
    //monOsc.sendInt("/angle", maLectureAngle);
    //monOsc.sendInt("/bouton", maLectureBouton);
  }
}
