#ifndef RAY_H
#define RAY_H

#include <Eigen/Dense>
using namespace Eigen;
#include "Sphere.h"
#include "Model.h"

class Ray
{
	public:
		Ray();
		Ray(Vector3d &start, Vector3d &dir);
		~Ray();

		Vector3d originPoint;
		Vector3d direction;

		double closestDist = -44444;
		Vector3d closestPoint = Vector3d(-4444, -4444, -4444);
		Sphere closestSphere = Sphere();
		Model closestModel = Model();
		int faceMatIndex = 0;
		Vector3d modelNhat;

		enum status { miss, sphere, model };
		status hit = miss;

		bool null = false;

		Vector3d getRefractionDir(Vector3d &QEhat, Vector3d &point, Vector3d &Nhat, double outIOR, double inIOR);
		Ray getExitRay(Vector3d &refract, Vector3d &QEhat, Vector3d &Nhat, Vector3d &point, double inIOR);




	private:

};

#endif




