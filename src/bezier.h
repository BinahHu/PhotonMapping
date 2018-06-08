#ifndef BEZIER_H
#define BEZIER_H

#include "object.h"

class Bezier: public Object
{
    //To be continue...
    public:
        virtual Intsct* Intersect(const Ray& rO, const Ray& rt);
};


#endif
