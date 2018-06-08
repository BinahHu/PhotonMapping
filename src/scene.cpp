#include "scene.h"
#include "kit.h"
#include <vector>


Scene::Scene()
{
	objhead = NULL;
	lighthead = NULL;
	camera = NULL;
	photonmap = NULL;
	photontracer = NULL;
	input = "";
	output = "";
}

Scene::~Scene()
{
	if(camera)			delete camera;
	if(photonmap)		delete photonmap;
	if(photontracer)	delete photontracer;
}


void Scene::AddObj(Object* obj)
{
	obj->SetNext(objhead);
	objhead = obj;
}

void Scene::AddLight(Light* light)
{
	light->SetNext(lighthead);
	lighthead = light;
}

void Scene::GenerateScene()
{
	std::cout << "Start Create Scene" << std::endl;
	camera = new Camera(this);
	LoadFile(this,input);
	photontracer = new Photontracer(this);
	photonmap = photontracer->Start();
	std::cout << "Scene Create Over" << std::endl;
}

void Scene::Rendering()
{
	std::cout << "Start Rendering" << std::endl;
	Bmp* ans = camera->Shot();
	ans->Output(output);
	std::cout << "Rendering Over" << std::endl;
}

void Scene::SetBackground(str var, ssm& finv)
{
		if(var == "color")	finv >> background;
}

Intsct* Scene::GetNearstObj(const Ray& rO,const  Ray& rt)
{
	Intsct* ret = NULL;
	Object* tmp = objhead;
	while(tmp)
	{
		Intsct* tmpint = tmp->Intersect(rO, rt);
		if (tmpint && (ret == NULL || tmpint->dep < ret->dep))
		{
			if(ret)	delete ret;
			ret = tmpint;
		}
		tmp = tmp->GetNext();
	}

	return ret;
}
LightIntsct* Scene::GetNearstLight(const Ray& rO,const Ray& rt)
{
	LightIntsct* ret = NULL;
	Light* tmp = lighthead;
	while(tmp)
	{
		LightIntsct* tmpint = tmp->Intersect(rO, rt);
		if (tmpint && (ret == NULL || tmpint->dep < ret->dep))
		{
			if(ret)	delete ret;
			ret = tmpint;
		}
		tmp = tmp->GetNext();
	}

	return ret;
}

Color Scene::GivePhongColor(Intsct* intsct)
{
	Light* tmp = lighthead;
	Intsct* tmpint;
	Color color;

	while(tmp)
	{
		Color lcolor;
		for(int i = 0; i < tmp->v.size(); i++)
		{
			double dist = (tmp->v[i] - intsct->P).Norm();
			tmpint = GetNearstObj(intsct->P, tmp->v[i] - intsct->P);
			if(tmpint && tmpint->dep < dist - EPS)delete tmpint;
			else
			{
				if(tmpint)	delete tmpint;
				Object* obj = intsct->GetObj();
				lcolor += tmp->GetColor() *  obj->GetMaterial()->BRDF(intsct->P - tmp->v[i], intsct->N, -intsct->I) * obj->GetColor(intsct->P);
			}
		}
		lcolor /= tmp->GetColor().Power();
		if(tmp->v.size() > 0)
			color += lcolor / (tmp->v.size() );
		tmp = tmp->GetNext();
	}
	return color;
}
