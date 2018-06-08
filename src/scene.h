#ifndef SCENE_H
#define SCENE_H
#include "typdef.h"
#include "object.h"
#include "light.h"
#include "camera.h"
#include "color.h"
#include "photonmap.h"
#include "photontracer.h"


class Scene
{
    public:
		Scene();
        ~Scene();
        void AddObj(Object*);
        void AddLight(Light*);
        Object* GetObjHead()			{return objhead;}
        Light*  GetLightHead()			{return lighthead;}
		void SetInput(str _input)		{input = _input;}
		str GetInput()					{return input;}
		void SetOutput(str _output)		{output = _output;}
		str GetOutput()					{return output;}
		void SetCamera(Camera* _camera)	{camera = _camera;}
		Camera*	GetCamera()				{return camera;}
        void CameraRead(str var, ssm& finv) {camera->Read(var, finv);}
        void CameraInit()               {camera->Init();}
        void SetBackground(str var, ssm& finv);
        Color GetBackground()           {return background;}
        Photonmap* GetPhotonmap()       {return photonmap;}

        Intsct* GetNearstObj(const Ray& rO,const Ray& rt);
        LightIntsct* GetNearstLight(const Ray& rO, const Ray& rt);
        Color GivePhongColor(Intsct* intsct);
		void GenerateScene();
		void Rendering();
    private:
        Object* objhead;
        Light* lighthead;
        Camera* camera;
        Color background;
		Photonmap* photonmap;
        Photontracer* photontracer;
		str input, output;
};

#endif
