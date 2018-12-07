#include "FastLED.h"
#include <ArduinoSTL.h>
#include <vector>

using std::vector;

class LeafConnection;

class Leaf
{
    vector<LeafConnection> connections;

  private:

  public:
    int iterations = 0;
    CRGB *leds[3];
    String name;

    Leaf(CRGB *led0, CRGB *led1, CRGB *led2, String name);
    void setColor(CRGB color);
    Leaf *connect(Leaf *leaf);
    vector<Leaf *> next();
    vector<Leaf *> next(Leaf *origin);
    boolean hasNext(Leaf *origin);
    boolean hasNext();
};

class LeafConnection
{
    Leaf *one;
    Leaf *other;

  public:
    LeafConnection(Leaf *one, Leaf *other);
    boolean contains(Leaf *leaf);
    Leaf *getConnected(Leaf *connected);
};