/*
Joseph Griffith
2017/12/08
CS 410-001
A5 - Ray tracer features now include: 
		reading in driver files that define the "camera" and object coordinates of a scene
		rendering spheres and polygonal model files, each with their own settings for 
			reflecting light intesity and color in ambient, diffuse, and specular ways
		casting shadows on, and reflecting other objects within a defined number of recursions
		refracting light as it passes through non-opaque spheres and attenuating the light level 
*/

#include <iostream>
#include <fstream>
#include <string>

#include "IObot.h"
#include "Calculon.h"
#include "Camera.h"

#include <map>
using std::map;
/*******************************************************************************
main() - 	creates and uses objects needed to render given scene
arguments -		primarily: driver filename, output filename
				optionally different combinations of extra arguments that override 
				driver file settings as follows:
					<recursion level> 
					<width> <height>
					<width> <height> <index of refraction (set all spheres)>
					<width> <height> <eye x> <eye y> <eye z> 
					<eye x> <eye y> <eye z> <look x> <look y> <look z>
					<width> <height> <eye x> <eye y> <eye z> <look x> <look y> <look z>
returns -		-1 on an error, 0 on success
*******************************************************************************/
int main(int argc, char* argv[])
{
	if (!(argc == 3 || argc == 4 || argc == 5 || argc == 6 || argc == 8 || argc == 9 || argc == 11))
	{
		cerr << "error: main() - exactly two arguments required: <driver filename>, <.ppm filename>" << endl;
		return -1;
	}

	IObot parsenator;
	Scene scene;

	if (!parsenator.readDriver(argv[1], scene))
	{
		cerr << "error: main() - driver file failed to open" << endl;
		return -1;
	}

	//optional arguments added to make it easy to change some things without editing the driver files
	if (argc == 4)																		//recursion depth
	{
		scene.recursion = atoi(argv[3]);
	}
	else if (argc == 5)																	//width, height
	{
		scene.eye.res[0] = atoi(argv[3]);
		scene.eye.res[1] = atoi(argv[4]);
	}
	else if (argc == 6)																	//position
	{
		scene.eye.res[0] = atoi(argv[3]);
		scene.eye.res[1] = atoi(argv[4]);
		scene.IoR = atof(argv[5]);
		for (unsigned int i = 0; i < scene.spheres.size(); i++)							//+ IoR
		{ 
			scene.spheres[i].mat.IoR = scene.IoR;
		}
	}
	else if (argc == 8)																	//width, height, position
	{
		scene.eye.res[0] = atoi(argv[3]);
		scene.eye.res[1] = atoi(argv[4]);
		scene.eye.position << atoi(argv[5]), atoi(argv[6]), atoi(argv[7]);
	}
	else if (argc == 9)																	//position, look
	{
		scene.eye.position << atoi(argv[3]), atoi(argv[4]), atoi(argv[5]);
		scene.eye.look << atoi(argv[6]), atoi(argv[7]), atoi(argv[8]);
	}
	else if (argc == 11)																//width, height, position, look
	{
		scene.eye.res[0] = atoi(argv[3]);
		scene.eye.res[1] = atoi(argv[4]);
		scene.eye.position << atoi(argv[5]), atoi(argv[6]), atoi(argv[7]);
		scene.eye.look << atoi(argv[8]), atoi(argv[9]), atoi(argv[10]);
	}

	if (!parsenator.readModels(scene))
	{
		cerr << "error: main() - a model file failed to open" << endl;
		return -1;
	}

	scene.printScene();
	scene.printModels();
	scene.printMaterials();
	for (auto &i : scene.matMap)
	{
		cout << i.first << ": <";
		for (unsigned int j = 0; j < i.second.size(); j++)
		{
			cout << i.second[j] << ", ";
		}
		cout << ">" << endl;
	}

	Calculon calc;
	calc.applyTransformation(scene);
	calc.writeVertices(scene);

	Camera cam;
	cam.takePicture(scene);

	if (!parsenator.writePPM(cam, argv[2]))
	{
		cerr << "error: main() - file or folder failed to open" << endl;
		return -1;
	}

	return 0;
}

