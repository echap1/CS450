#include "../lib/Adafruit_NeoPixel/Adafruit_NeoPixel.h"

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(40, 3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(40, 5, NEO_GRB + NEO_KHZ800);

void setup() {
    strip1.begin();
    strip2.begin();
}

void loop() {
    strip1.clear();
    strip2.clear();

    // The first NeoPixel in a strand is #0, second is 1, all the way up
    // to the count of pixels minus one.
    for(int i=0; i<40; i++) {
        strip1.setPixelColor(i, Adafruit_NeoPixel::Color(0, 150, 0));
        strip2.setPixelColor(i, Adafruit_NeoPixel::Color(0, 150, 0));

        strip1.show();
        strip2.show();

        delay(500);
    }
}
