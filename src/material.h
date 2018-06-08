#ifndef MATERIAL_H
#define MATERIAL_H

#include "typdef.h"
#include "color.h"
#include "vector3.h"
#include "texture.h"

extern const int POWCOEF;

class Material
{
    public:
        Material()
        {
            texture = NULL;
            incolor = Color(1,1,1);
            crefl = crefc = cdiff = cabso = cspec = 0.0;
            N = 1.0;
        }
        void Read(str var, ssm& finv);

        Color color, absorb, incolor;
        Texture* texture;
        double crefl, crefc, cdiff, cabso, cspec, N;
        double BRDF(const Ray& I, const Ray& N, const Ray& V);
};


#endif
