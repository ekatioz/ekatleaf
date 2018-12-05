#include <Arduino.h>
#include "FastLED.h"
#include "ekz_connections.h"

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

CRGB colors[] = {CRGB::Red, CRGB::Magenta, CRGB::Blue, CRGB::Cyan, CRGB::Green, CRGB::Yellow};
int colorIdx = 0;

Leaf leaf1;
Leaf leaf2;
Leaf leaf3;
Leaf leaf4;
Leaf leaf5;
Leaf leaf6;
Leaf leaf7;
Leaf leaf8;
Leaf leaf9;
Leaf leaf10;
Leaf leaf11;
Leaf leaf12;
Leaf leaf13;
Leaf leaf14;

vector<Leaf> leaves;

vector<Leaf *> currentLeaves;
vector<Leaf *> nextLeaves;
vector<Leaf *> prevLeaves;
vector<Leaf *> nxts;

void setup()
{
    Serial.begin(9600); // open the serial port at 9600 bps:
    delay(3000);        // 3 second delay for recovery
    Serial.println("Start!");
    // tell FastLED about the LED strip configuration
    //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS)
        .setCorrection(0xDDBBFF)
        .setTemperature(WarmFluorescent);

    // set master brightness control
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();

    setUpConnections(&leaf1, &leds[41], &leds[0], &leds[1], "1");
    setUpConnections(&leaf2, &leds[40], &leds[2], &leds[3], "2");
    setUpConnections(&leaf3, &leds[38], &leds[39], &leds[4], "3");
    setUpConnections(&leaf4, &leds[37], &leds[5], &leds[6], "4");
    setUpConnections(&leaf5, &leds[35], &leds[36], &leds[7], "5");
    setUpConnections(&leaf6, &leds[34], &leds[8], &leds[9], "6");
    setUpConnections(&leaf7, &leds[23], &leds[33], &leds[10], "7");
    setUpConnections(&leaf8, &leds[21], &leds[22], &leds[11], "8");
    setUpConnections(&leaf9, &leds[12], &leds[13], &leds[20], "9");
    setUpConnections(&leaf10, &leds[14], &leds[18], &leds[19], "10");
    setUpConnections(&leaf11, &leds[15], &leds[16], &leds[17], "11");
    setUpConnections(&leaf12, &leds[32], &leds[31], &leds[24], "12");
    setUpConnections(&leaf13, &leds[25], &leds[26], &leds[30], "13");
    setUpConnections(&leaf14, &leds[27], &leds[29], &leds[28], "14");

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
    randomSeed(analogRead(0));
}

void printVector(vector<Leaf *> v, String name)
{
    Serial.print("Printing vector ");
    Serial.print(name);
    Serial.print(" with size of ");
    Serial.println(v.size());
    for (auto leaf : v)
    {
        Serial.print("Leaf: ");
        Serial.println(leaf->name);
    }
    Serial.println("- - - - - - - - - - - - - - - -");
}

void filterPrev(vector<Leaf *> unfiltered)
{
    nxts.clear();
    /*  printVector(prevLeaves,"prevLeaves");
  printVector(unfiltered,"unfiltered"); */
    for (auto ulf : unfiltered)
    {
        bool found = false;
        for (auto lf : prevLeaves)
        {
            /*   Serial.print("Comparing pref ");
      Serial.print(lf->name);
      Serial.print(" and unfiltered ");
      Serial.println(ulf->name); */

            if (lf->name.equals(ulf->name))
            {
                found = true;
                /*   Serial.println("FOUND!"); */
            }
        }

        if (!found)
        {
            /*       Serial.println("NEW!");
      Serial.print("pushing ");
      Serial.println(ulf->name); */
            nxts.push_back(ulf);
        }
    }
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextColor()
{
    // add one to the current pattern number, and wrap around at the end
    colorIdx = (colorIdx + 1) % ARRAY_SIZE(colors);
}

void loop()
{
    nextLeaves.clear();
    prevLeaves.clear();
    Leaf start = leaves.at(random(leaves.size()-1));
    currentLeaves.push_back(&start);
    while (currentLeaves.size() != 0)
    {
       /*  Serial.print("going throu ");
        Serial.print(currentLeaves.size());
        Serial.println(" currentLeaves"); */
        for (auto &&lf : currentLeaves)
        {
           /*  Serial.print("current Leaf is ");
            Serial.println(lf->name); */
            lf->setColor(colors[colorIdx]);
            prevLeaves.push_back(lf);
            filterPrev(lf->next());
            /* Serial.print(nxts.size());
      Serial.println(" upcoming leaves");
 */
            for (unsigned i = 0; i < nxts.size(); i++)
            {
                nextLeaves.push_back(nxts.at(i));
                /* Serial.print("next ");
                Serial.println(nextLeaves.back()->name); */
            }
        }
        currentLeaves.clear();

        for (unsigned i = 0; i < nextLeaves.size(); i++)
        {
            currentLeaves.push_back(nextLeaves.at(i));
        }
        nextLeaves.clear();

        FastLED.show();
        delay(1000);
    }
    nextColor();
}
