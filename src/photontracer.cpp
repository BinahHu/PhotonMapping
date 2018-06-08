#include "photontracer.h"
#include "intsct.h"
#include "kit.h"
#include <cstdlib>
#include <omp.h>

const int MAX_PHOTON_DEP = 10;
inline double ran() {return double(rand() % RAND_MAX) / RAND_MAX;}

Photonmap* Photontracer::Start()
{
    int maxphotons = 0;
    float maxpower = 0.0;
    Light* tmp = scene->GetLightHead();
    while(tmp)
    {
        maxphotons += tmp->GetMaxPhotons();
        maxpower += tmp->GetColor().Power();
        tmp = tmp->GetNext();
    }
    Photonmap* photonmap = new Photonmap(maxphotons * MAX_PHOTON_DEP, scene);
    float photonpower = float(maxpower) / maxphotons;
    tmp = scene->GetLightHead();
    while(tmp)
    {
#pragma parallel omp for
        for(int i = 0; i < tmp->GetMaxPhotons(); i++)
        {
            Photon photon = tmp->EmitPhoton();
            photon.color *= photonpower;
            PhotonTrace(photon, 1, photonmap, false);
        }
        tmp = tmp->GetNext();
    }
    photonmap->Balance();
    return photonmap;
}

void Photontracer::PhotonTrace(Photon photon, int dep, Photonmap* photonmap, bool refracted)
{
    if(photon.color.IsBlack())return;
    if(dep > MAX_PHOTON_DEP)return;
    Intsct* intsct = scene->GetNearstObj(photon.pos, photon.dir);
    if(!intsct)return;
    photon.pos = intsct->P;
    Material* mat = intsct->GetObj()->GetMaterial();
    if(mat->cdiff > EPS)
        photonmap->Store(photon);

    double boardsize = mat->cabso + mat->crefl + mat->crefc;
    double end = ran() * boardsize;
         if(end < mat->crefl)              Reflect(intsct, photon, dep, photonmap, refracted);
    else if(end < mat->crefl + mat->crefc) Refract(intsct, photon, dep, photonmap, refracted);

    if(intsct)  delete intsct;
}

void Photontracer::Diffuse(Intsct* intsct, Photon photon, int dep, Photonmap* photonmap, bool refracted)
{
    Object* obj = intsct->GetObj();
    photon.dir = intsct->N.Diffuse();
    Color basecolor = obj->GetColor(intsct->P) * obj->GetMaterial()->cdiff;
    float power = photon.color.Power();
    photon.color = photon.color * basecolor;
    PhotonTrace(photon, dep + 1, photonmap, refracted);
}

void Photontracer::Reflect(Intsct* intsct, Photon photon, int dep, Photonmap* photonmap, bool refracted)
{
    Object* obj = intsct->GetObj();
    photon.dir = ReflDir(intsct->I, intsct->N);
    Color basecolor = obj->GetColor(intsct->P) * obj->GetMaterial()->crefl;
    float power = photon.color.Power();
    photon.color = photon.color * basecolor;
    PhotonTrace(photon, dep + 1, photonmap, refracted);
}

void Photontracer::Refract(Intsct* intsct, Photon photon, int dep, Photonmap* photonmap, bool refracted)
{
    Object* obj = intsct->GetObj();
    double n = obj->GetMaterial()->N;
    bool valid;
    n = refracted ? 1.0 / n : n;
    photon.dir = RefrDir(intsct->I, intsct->N, n, valid);
    if(!valid)return;
    Color basecolor = obj->GetMaterial()->incolor * obj->GetMaterial()->crefc;
    if(refracted)
        basecolor = basecolor * (obj->GetMaterial()->absorb * -intsct->dep).Exp();
    float power = photon.color.Power();
    photon.color = photon.color * basecolor;
    PhotonTrace(photon, dep + 1, photonmap, refracted);
}
