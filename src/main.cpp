#include <Arduino.h>
#include "FastLED.h"
#include "ekatleaf.h"

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN 23
#define CLK_PIN 22
#define LED_TYPE APA102
#define COLOR_ORDER BGR
#define NUM_LEDS 42
CRGB leds[NUM_LEDS];

#define BRIGHTNESS 90

uint8_t gHue = 0;

vector<Leaf> leaves;

Leaf leaf1 = Leaf(&leds[41], &leds[0], &leds[1], "1");
Leaf leaf2 = Leaf(&leds[40], &leds[2], &leds[3], "2");
Leaf leaf3 = Leaf(&leds[38], &leds[39], &leds[4], "3");
Leaf leaf4 = Leaf(&leds[37], &leds[5], &leds[6], "4");
Leaf leaf5 = Leaf(&leds[35], &leds[36], &leds[7], "5");
Leaf leaf6 = Leaf(&leds[34], &leds[8], &leds[9], "6");
Leaf leaf7 = Leaf(&leds[23], &leds[33], &leds[10], "7");
Leaf leaf8 = Leaf(&leds[21], &leds[22], &leds[11], "8");
Leaf leaf9 = Leaf(&leds[12], &leds[13], &leds[20], "9");
Leaf leaf10 = Leaf(&leds[14], &leds[18], &leds[19], "10");
Leaf leaf11 = Leaf(&leds[15], &leds[16], &leds[17], "11");
Leaf leaf12 = Leaf(&leds[32], &leds[31], &leds[24], "12");
Leaf leaf13 = Leaf(&leds[25], &leds[26], &leds[30], "13");
Leaf leaf14 = Leaf(&leds[27], &leds[29], &leds[28], "14");

void initializeConfiguration()
{
    leaf1.connect(&leaf2);
    leaf2.connect(&leaf3);
    leaf3.connect(&leaf4);
    leaf4.connect(&leaf5);
    leaf5.connect(&leaf6);
    leaf6.connect(&leaf7);
    leaf7.connect(&leaf8);
    leaf7.connect((&leaf12)->connect((&leaf13)->connect((&leaf14))));
    leaf8.connect(&leaf9);
    leaf9.connect(&leaf10);
    leaf10.connect(&leaf11);

    leaves.push_back(leaf1);
    leaves.push_back(leaf2);
    leaves.push_back(leaf3);
    leaves.push_back(leaf4);
    leaves.push_back(leaf5);
    leaves.push_back(leaf6);
    leaves.push_back(leaf7);
    leaves.push_back(leaf8);
    leaves.push_back(leaf9);
    leaves.push_back(leaf10);
    leaves.push_back(leaf11);
    leaves.push_back(leaf12);
    leaves.push_back(leaf13);
    leaves.push_back(leaf14);
}

void setup()
{
    delay(3000); // 3 second delay for recovery
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS)
        .setCorrection(0xDDBBFF)
        .setTemperature(WarmFluorescent);

    // set master brightness control
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();

    initializeConfiguration();

    randomSeed(analogRead(0));
}

void loop()
{
    Leaf start = leaves.at(random(leaves.size() - 1));

    CRGB color;
    hsv2rgb_rainbow(CHSV(gHue, 255, 192), color);
    dropInColor(&start, color, 1000);
    gHue = gHue + 10;
}
