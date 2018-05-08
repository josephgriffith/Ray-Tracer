#ifndef SCENE_H
#define SCENE_H

#include "Model.h"
#include "Eye.h"
#include "Light.h"
#include "Sphere.h"
#include <map>
using std::map;

class Scene
{
	public:
		Scene();
		~Scene();
		
		vector<string> driver;				//lines of the driver file
		vector<Model> models;				//
		vector<Sphere> spheres;
		Eye eye;							//the 'camera'
		vector<Material> mats;				//all materials in the scene
		map<string, vector<int>> matMap;	//dictionary with {file.mtl: mats indices}

		int recursion = -44;
		double IoR = -1.5;

		Vector3d ambient;					//ambient light rgb balance, intensity
		vector<Light> lights;				//lights

		inline void printDriver()	{ for (unsigned int i = 0; i < driver.size(); i++)  { cout << "scene.driver[" << i << "]: " << driver[i] << endl;	} }
		inline void printLights()	{ for (unsigned int i = 0; i < lights.size(); i++)	{ cout << "light[" << i << "]: "; lights[i].printLight();		} }
		inline void printSpheres()	{ for (unsigned int i = 0; i < spheres.size(); i++) { cout << "sphere[" << i << "]: "; spheres[i].printSphere();	} }
		inline void printModels()	{ for (unsigned int i = 0; i < models.size(); i++) { cout << "model[" << i << "]: "; models[i].printModel(); } }
		inline void printMaterials(){ for (unsigned int i = 0; i < mats.size(); i++) { cout << "material[" << i << "]: "; mats[i].printMat(); } }

		void printScene();

		inline const Model& operator [] (int i) const { return models[i]; }
		inline Model& operator [] (int i) { return models[i]; }

	private:
};


#endif