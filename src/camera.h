#ifndef CAMERA_H
#define CAMERA_H

#include "typdef.h"
#include "vector3.h"
#include "bmp.h"
#include "raytracer.h"

class Scene;
class Camera
{
	public:
		Camera(Scene* _scene);
		void Read(str var, ssm& finv);
		void Init();
		Vector3 Emit(double i, double j);
		Bmp* Shot();
		double GetCOEF()	{return coef;}
	private:
		Scene* scene;
		Raytracer* raytracer;
		Vector3 O, N, U, S, R, Dh, Dw;
		int w, h;
		double phyh, phyw, f, coef;

};


#endif
