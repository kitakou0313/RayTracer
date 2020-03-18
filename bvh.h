#ifndef BVHH
#define BVHH

#include "hitable.h"

class bvhNode : public hitable
{
public:
    bvhNode() {}
    bvhNode(hitable **l, int n, float time0, float time1);

    virtual bool hit(const ray &, float tmin, float tmax, hitRecord &rec) const;
    virtual bool boundingBox(float t0, float t1, aabb &box) const;

    hitable *left;
    hitable *right;
    aabb box;
};

bool bvhNode::boundingBox(float t0, float t1, aabb &b) const
{
    b = box;
    return true;
}

bool bvhNode::hit(const ray &r, float tMin, float tMax, hitRecord &rec) const
{
    if (box.hit(r, tMin, tMax))
    {
        hitRecord leftRec, rightRec;
        bool leftHit = left->hit(r, tMin, tMax, leftRec);
        bool rightHit = right->hit(r, tMin, tMax, rightRec);

        if (leftHit && rightHit)
        {
            if (leftRec.t < rightRec.t)
                rec = leftRec;
            else
                rec = rightRec;
            return true;
        }
        else if (leftHit)
        {
            rec = leftRec;
            return true;
        }
        else if (rightHit)
        {
            rec = rightRec;
            return true;
        }
        else
        {
            return false;
        }
    }

    else
        return false;
}

int boxXCompare(const void *a, const void *b)
{
    aabb boxLeft, boxRight;
    hitable *ah = *(hitable **)a;
    hitable *bh = *(hitable **)b;

    if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
        std::cerr << "no bounding box in bvhNode constructor \n";

    if (boxLeft.min().x() - boxRight.min().x() < 0.0)
        return -1;
    else
        return 1;
}

int boxYCompare(const void *a, const void *b)
{
    aabb boxLeft, boxRight;
    hitable *ah = *(hitable **)a;
    hitable *bh = *(hitable **)b;

    if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
        std::cerr << "no bounding box in bvhNode constructor \n";

    if (boxLeft.min().y() - boxRight.min().y() < 0.0)
        return -1;
    else
        return 1;
}

int boxZCompare(const void *a, const void *b)
{
    aabb boxLeft, boxRight;
    hitable *ah = *(hitable **)a;
    hitable *bh = *(hitable **)b;

    if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
        std::cerr << "no bounding box in bvhNode constructor \n";

    if (boxLeft.min().z() - boxRight.min().z() < 0.0)
        return -1;
    else
        return 1;
}

bvhNode ::bvhNode(hitable **l, int n, float time0, float time1)
{
    int axis = int(3 * drand());

    if (axis == 0)
        qsort(l, n, sizeof(hitable *), boxXCompare);
    else if (axis == 1)
        qsort(l, n, sizeof(hitable *), boxYCompare);
    else
        qsort(l, n, sizeof(hitable *), boxZCompare);

    if (n == 1)
        left = right = l[0];
    else if (n == 2)
    {
        left = l[0];
        right = l[1];
    }
    else
    {
        left = new bvhNode(l, n / 2, time0, time1);
        right = new bvhNode(l + n / 2, n - n / 2, time0, time1);
    }

    aabb boxLeft, boxRight;

    if (!left->boundingBox(time0, time1, boxLeft) ||
        !right->boundingBox(time0, time1, boxRight))
    {

        std::cerr << "No Bounding box in bvhNode constructor \n";
    }

    box = surroundingBox(boxLeft, boxRight);
}

#endif