#include "scene.h"
#include <ctime>
#include <cstdlib>

int main()
{
	srand((unsigned int)time(0));
	Scene* scene = new Scene();
	scene->SetInput("../input/scene_plane.txt");
	scene->SetOutput("../output/picture.png");
	scene->GenerateScene();
	scene->Rendering();
	delete scene;
	return 0;
}
