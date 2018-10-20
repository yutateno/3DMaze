#include "Scene.h"

bool Scene::endflag;
eScene Scene::scene;

Scene::Scene()
{
	start = false;
	end = false;
	count = 0;
}

bool Scene::GetEndFlag()
{
	return Scene::endflag;
}

eScene Scene::GetScene()
{
	return Scene::scene;
}

void Scene::SetScene(eScene scene)
{
	Scene::scene = scene;
}

int Scene::GetPlayModel()
{
	return playmodel;
}

bool Scene::GetCameraType()
{
	return cameratype;
}