#include "Scene.h"

Scene::Scene()
{
	models.reserve(100);
	spheres.reserve(10000);
	ambient = Vector3d::Zero();
}
Scene::~Scene()
{
}

/*******************************************************************************
printScene() - 	prints information about the Scene to the console
*******************************************************************************/
void Scene::printScene()
{
	cout << "eye: [" << eye.position.transpose() << "]\nlook: [" << eye.look.transpose() << "]\nup: [" << eye.up.transpose() << "]" << endl;
	cout << "d: " << eye.d << endl;
	cout << "bounds: (" << eye.left << ", " << eye.bottom << "), (" << eye.right << ", " << eye.top << ")" << endl;
	cout << "res: " << eye.res[0] << "x" << eye.res[1] << endl;
	cout << "recursion: " << recursion << endl;
	cout << "ambient: " << ambient.transpose() << endl;
	printLights();
	printSpheres();
}

