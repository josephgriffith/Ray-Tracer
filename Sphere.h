#ifndef SPHERE_H
#define SPHERE_H

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::ostream;
#include "Material.h"

class Sphere
{
	public:
		Sphere();
		Sphere(Vector3d c, double r, Vector3d a, Vector3d d, Vector3d s, Vector3d at, Vector3d op, double ior, double phong);
		~Sphere();

		Vector3d center = Vector3d(0,0,0);			//center xyz of sphere
		double radius;								//radius of sphere
		Material mat;								//material of sphere

		bool null = false;

		inline void printSphere() { cout << "center: " << center.transpose() << ", radius: " << radius << ", "; mat.printMat(); }


	private:

};

#endif




