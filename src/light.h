#ifndef LIGHT_H
#define LIGHT_H
#include "typdef.h"
#include "vector3.h"
#include "intsct.h"
#include "camera.h"
#include "color.h"
#include <vector>
#include "photon.h"

extern const double SampleArea;

class Light
{
    protected:
        Light* next;
        Color color;
        int max_photon;
        int emited_photon;
    public:
        std::vector<Vector3> v;
        virtual LightIntsct* Intersect(const Ray& r, const Ray& rt) = 0;
        virtual Photon EmitPhoton() = 0;
        virtual void Read(str var, ssm& finv);
        virtual void Init()                         {}
        Light()                                     {next = NULL;emited_photon = 0;}
        Light* GetNext()                            {return next;}
        void SetNext(Light* _next)                  {next = _next;}
        Color GetColor()                            {return color;}
        void SetMaxPhotons(int num)                 {max_photon = num;}
        int GetMaxPhotons()                         {return max_photon;}
        bool CanEmit()                              {return emited_photon < max_photon;}
};

class PointLight: public Light
{
    public:
        virtual LightIntsct* Intersect(const Ray& r, const Ray& rt);
        virtual Photon EmitPhoton();
        void Read(str var, ssm& finv);
        void Init();
    private:
        Vector3 O;
};

class LineLight: public Light
{
    //To be continue...
    public:
        virtual LightIntsct* Intersect(const Ray& r, const Ray& rt);
        virtual Photon EmitPhoton();
};

class AreaLight: public Light
{
    public:
        virtual LightIntsct* Intersect(const Ray& r, const Ray& rt);
        virtual Photon EmitPhoton();
        void Read(str var, ssm& finv);
        void Init();
    private:
        Vector3 N, O, Dx, Dy;
        double R;
};

#endif
