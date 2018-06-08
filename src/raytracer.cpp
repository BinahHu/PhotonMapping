#include "raytracer.h"
#include "intsct.h"
#include "scene.h"
#include "kit.h"

const int MAX_RAYTRACING_DEP = 20;

Raytracer::Raytracer(Scene* _scene)
{
    scene = _scene;
}


Color Raytracer::Diffusion(Intsct* intsct)
{
    Object* obj = intsct->GetObj();
    Color c1 = scene->GivePhongColor(intsct);
    Color c2 = scene->GetPhotonmap()->GetColor(intsct);
    return c1 + c2 ;
}
Color Raytracer::Reflection(Intsct* intsct, const Ray& V, int dep, bool refracted)
{
    Object* obj = intsct->GetObj();
    Ray rt = ReflDir(V, intsct->N);
    Color basecolor = obj->GetColor(intsct->P) * obj->GetMaterial()->crefl;
    return Raytrace(intsct->P, rt, dep + 1, refracted) * basecolor;
}
Color Raytracer::Refraction(Intsct* intsct, const Ray& V, int dep, bool refracted)
{
    Object* obj = intsct->GetObj();
    double n = obj->GetMaterial()->N;
    bool valid;
    n = refracted ? 1.0 / n : n;
    Ray rt = RefrDir(V, intsct->N, n, valid);
    if(!valid)return Color();
    Color basecolor = obj->GetMaterial()->incolor * obj->GetMaterial()->crefc;
    if(refracted)
        basecolor = basecolor * (obj->GetMaterial()->absorb * -intsct->dep).Exp();
    Color Rcolor = Raytrace(intsct->P, rt, dep + 1, !refracted);
    return basecolor * Rcolor;

}

Color Raytracer::Raytrace(const Ray& rO, const Ray& rt, int dep, bool refracted)
{
    if(dep > MAX_RAYTRACING_DEP)return Color();
    Color ret;
    Intsct* intsct = scene->GetNearstObj(rO, rt);
    LightIntsct* lightintsct = scene->GetNearstLight(rO, rt);
    if(lightintsct)
    {
        if(!intsct || lightintsct->dep < intsct->dep + EPS)
        {
            Color lcolor = lightintsct->GetLight()->GetColor();
            ret += lcolor / lcolor.GetMax();
            delete lightintsct;
            if(intsct)  delete intsct;
            return ret;
        }
        delete lightintsct;
    }
    if(intsct)
    {
        Object* nearestobj = intsct->GetObj();
        if (nearestobj->GetMaterial()->cdiff > EPS) ret += Diffusion(intsct);
		if (nearestobj->GetMaterial()->crefl > EPS) ret += Reflection(intsct, rt, dep, refracted);
		if (nearestobj->GetMaterial()->crefc > EPS)   ret += Refraction(intsct, rt, dep, refracted);
        delete intsct;
    }
    if(dep == 1)ret = ret.Regular();
    return ret;
}
