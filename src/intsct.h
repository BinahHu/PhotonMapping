#ifndef INTSCT_H
#define INTSCT_H

#include "vector3.h"

class Object;
class Light;

class Intsct
{
	public:
		Vector3 P, N, I;
		double dep;
		Object* obj;
		Intsct()						{obj = NULL;}
		void SetObj(Object* _obj)		{obj = _obj;}
		Object* GetObj()				{return obj;}
};

class LightIntsct
{
	public:
		Vector3 P, N, I;
		double dep;
		Light* lig;
		LightIntsct()					{lig = NULL;}
		void SetLight(Light* _light)	{lig = _light;}
		Light* GetLight()				{return lig;}
};

#endif
