#ifndef HITABLELISTH
#define HITABLELISTH

#include "hitable.h"

class hitableList : public hitable
{
public:
    hitableList() {}
    hitableList(hitable **l, int n)
    {
        list = l;
        listSize = n;
    }
    virtual bool hit(const ray &r, float tMin, float tMax, hitRecord &rec) const;
    hitable **list;
    int listSize;
};

bool hitableList::hit(const ray &r, float tMin, float tMax, hitRecord &rec) const
{
    hitRecord tempRec;
    bool hitAnything = false;

    double closestSoFar = tMax;
    for (int i = 0; i < listSize; i++)
    {
        if (list[i]->hit(r, tMin, closestSoFar, tempRec))
        {
            hitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }
    return hitAnything;
}

bool hitableList::boundingBox(float t0, float t1, aabb &box) const
{
    if (listSize < 1)
        return false;

    aabb tempBox;
    bool firstTrue = list[0]->boundingBox(t0, t1, tempBox);

    if (!firstTrue)
        return false;
    else
        box = tempBox;
    for (int i = 1; i < listSize; i++)
    {
        if (list[i]->boundingBox(t0, t1, tempBox))
        {
            box = surroundingBox(box, tempBox);
        }
        else
        {
            return false;
        }
    }
    return true;
}

#endif