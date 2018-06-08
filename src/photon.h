#ifndef PHOTON
#define PHOTON

#include "typdef.h"
#include "color.h"
#include "vector3.h"

struct Photon
{
    Photon()    {}
    Color color;
    Vector3 pos, dir;
};

#endif
