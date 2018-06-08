#ifndef PHOTONTRACER_H
#define PHOTONTRACER_H

#include "photonmap.h"
#include "photon.h"
#include "intsct.h"
extern const int MAX_PHOTON_DEP;

class Scene;
class Photontracer
{
    public:
        Photontracer(Scene* _scene = NULL) {scene = _scene;}
        Photonmap* Start();
        void PhotonTrace(Photon photon, int dep, Photonmap* photonmap, bool refracted);
        void Diffuse(Intsct* intsct, Photon photon, int dep, Photonmap* photonmap, bool refracted);
        void Reflect(Intsct* intsct, Photon photon, int dep, Photonmap* photonmap, bool refracted);
        void Refract(Intsct* intsct, Photon photon, int dep, Photonmap* photonmap, bool refracted);
        void SetScene(Scene* _scene)    {scene = _scene;}
    private:
        Scene* scene;
};

#endif
