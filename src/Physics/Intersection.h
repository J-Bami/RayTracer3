#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Surfaces/Surface.h"

/*
Class used to find the first intersection between a ray all surfaces in a (sub) set of surfaces
*/

template<typename ftype>
struct Intersection
{
    ftype distance;                     // the minimum distance of the nearest surface ray points to.
    ftype upper_bound;
    ftype lower_bound;
    const Surface<ftype>* closest;      // the closest surface that intersects with the ray.
    // we are assuming that only surfaces can intersect with rays& interaction doesn't modify surfaces

    Intersection() :
        closest(nullptr),
        distance(INFINITY),
        lower_bound(Surface<ftype>::tolerance),
        upper_bound(INFINITY)
    {}

    void update(const Surface<ftype>* surface, const ftype dist)
    {
        if ((dist < upper_bound) && (dist > lower_bound))
        {
            closest = surface;
            distance = dist;
            upper_bound = dist * Surface<ftype>::rtolerance;
        }
    }
};

#endif