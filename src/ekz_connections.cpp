#include "ekz_connections.h"

void setUpConnections(Leaf *leaf, CRGB *led0, CRGB *led1, CRGB *led2, String name)
{
        leaf->leds[0] = led0;
        leaf->leds[1] = led1;
        leaf->leds[2] = led2;
        leaf->name = name;
}