#ifndef BASIC_H
#define BASIC_H

#include "object.h"


class Triangle: public Object
{
    //To be continue...
    public:
        virtual Intsct* Intersect(const Ray& rO, const Ray& rt);
};

class Rectangle: public Object
{
    public:
        virtual void Read(str var, ssm& finv);
        virtual Intsct* Intersect(const Ray& rO, const Ray& rt);
        virtual Color GetColor(const Vector3& P);
        virtual void Init();
    private:
        Vector3 N, O, Dx, Dy;
        double R;
};

class Sphere: public Object
{
    public:
        virtual void Read(str var, ssm& finv);
        virtual Intsct* Intersect(const Ray& rO, const Ray& rt);
        virtual Color GetColor(const Vector3& P);
    private:
        Vector3 O;
        double R;
};


#endif
