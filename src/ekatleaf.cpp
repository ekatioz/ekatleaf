#include "ekatleaf.h"

vector<Leaf *> currentLeaves;
vector<Leaf *> nextLeaves;
vector<Leaf *> prevLeaves;
vector<Leaf *> nxts;

void printVector(vector<Leaf *> v, String name)
{
    /* Serial.print("Printing vector ");
    Serial.print(name);
    Serial.print(" with size of ");
    Serial.println(v.size());
    for (auto leaf : v)
    {
        Serial.print("Leaf: ");
        Serial.println(leaf->name);
    }
    Serial.println("- - - - - - - - - - - - - - - -"); */
}

void filterPrev(vector<Leaf *> unfiltered)
{
    nxts.clear();
    for (auto ulf : unfiltered)
    {
        bool found = false;
        for (auto lf : prevLeaves)
        {
            if (lf->name.equals(ulf->name))
            {
                found = true;
            }
        }

        if (!found)
        {
            nxts.push_back(ulf);
        }
    }
}
void dropInColor(Leaf *start, CRGB color, int delayBetweenLeaves)
{
    nextLeaves.clear();
    prevLeaves.clear();
    currentLeaves.push_back(start);
    while (currentLeaves.size() != 0)
    {
        for (auto &&lf : currentLeaves)
        {
            lf->setColor(color);
            prevLeaves.push_back(lf);
            filterPrev(lf->next());
            for (unsigned i = 0; i < nxts.size(); i++)
            {
                nextLeaves.push_back(nxts.at(i));
            }
        }
        currentLeaves.clear();

        for (unsigned i = 0; i < nextLeaves.size(); i++)
        {
            currentLeaves.push_back(nextLeaves.at(i));
        }
        nextLeaves.clear();

        FastLED.show();
        delay(delayBetweenLeaves);
    }
}