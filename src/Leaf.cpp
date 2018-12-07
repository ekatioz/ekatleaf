#include "Leaf.h"

Leaf::Leaf(CRGB *led0, CRGB *led1, CRGB *led2, String name)
{
    this->leds[0] = led0;
    this->leds[1] = led1;
    this->leds[2] = led2;
    this->name = name;
}

void Leaf::setColor(CRGB color)
{
    for (int i = 0; i < 3; i++)
    {
        *leds[i] = color;
    }
}

Leaf *Leaf::connect(Leaf *leaf)
{
    LeafConnection con(this, leaf);
    connections.push_back(con);
    leaf->connections.push_back(con);
    return this;
}

vector<Leaf *> Leaf::next()
{
    vector<Leaf *> nexts;
    for (unsigned i = 0; i < connections.size(); i++)
    {
        nexts.push_back(connections.at(i).getConnected(this));
    }
    return nexts;
}

vector<Leaf *> Leaf::next(Leaf *origin)
{
    vector<Leaf *> nexts;
    for (unsigned i = 0; i < connections.size(); i++)
    {
        if (!connections.at(i).contains(origin))
        {
            nexts.push_back(connections.at(i).getConnected(this));
        }
    }
    return nexts;
}

boolean Leaf::hasNext(Leaf *last)
{
    return connections.size() > 1 || !connections.at(0).contains(last);
}

boolean Leaf::hasNext()
{
    return connections.size() > 0;
}

LeafConnection::LeafConnection(Leaf *a, Leaf *b) : one(a), other(b) {}

boolean LeafConnection::contains(Leaf *leaf)
{
    return one == leaf || other == leaf;
}

Leaf *LeafConnection::getConnected(Leaf *connected)
{
    if (connected->name.equals(one->name))
    {
        return other;
    }
    else if (connected->name.equals(other->name))
    {
        return one;
    }
    else
    {
        return one;
    }
}