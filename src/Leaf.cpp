#include "Leaf.h"

void Leaf::setColor(CRGB color)
{
    for (int i = 0; i < 3; i++)
    {
        *leds[i] = color;
    }
}

void Leaf::fadeTo(CRGB color)
{
    for (int i = 0; i < 3; i++)
    {
        *leds[i] = blend(*leds[i], color, 255 / 40);
    }
}

void Leaf::fadeToBlack()
{
    for (int i = 0; i < 3; i++)
    {
        leds[i]->fadeToBlackBy(40);
    }
}

Leaf *Leaf::connect(Leaf *leaf)
{
    LeafConnection con(this, leaf);
    connections.push_back(con);
    leaf->connections.push_back(con);

    Serial.print("connected\t");
    Serial.print(name);
    Serial.print("\tto\t");
    Serial.print(leaf->name);
    Serial.println();
    return this;
}

vector<Leaf *> Leaf::next()
{
    vector<Leaf *> nexts;
    for (unsigned i = 0; i < connections.size(); i++)
    {
        nexts.push_back(connections.at(i).getConnected(this));
    }
    Serial.print("Deliver ");
    Serial.print(nexts.size());
    Serial.println(" next Leaves");
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
    Serial.print("getConnected ");
    Serial.println(connected->name);
    if (connected->name.equals(one->name))
    {
        Serial.print("returning ");
        Serial.println(other->name);
        return other;
    }
    else if (connected->name.equals(other->name))
    {
        Serial.print("returning ");
        Serial.println(one->name);
        return one;
    }
    else
    {
        return one;
    }
}