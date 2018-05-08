#include "Ray.h"

Ray::Ray()
{
	originPoint = Vector3d::Zero();
	direction = Vector3d::Zero();
	null = true;
}
Ray::Ray(Vector3d &start, Vector3d &dir)
{
	originPoint = start;
	direction = dir;
	null = false;
}
Ray::~Ray()
{
}

/*******************************************************************************
getRefractionDir() - 	calculates the direction the ray will be going after 
							crossing a sphere boundary
arguments -		references to: 
					- the vector pointing from the intersection point to the eye
					- the point at which the ray intersected something
					- the normal vector at the point of intersection
				the index of refraction of the current medium
				the index of refraction of the next medium
returns -		either a zero vector or the direction of the refracted light ray
*******************************************************************************/
Vector3d Ray::getRefractionDir(Vector3d &QEhat, Vector3d &point, Vector3d &Nhat, double out, double in)
{
	double mu = out / in;
	double MdotN = QEhat.dot(Nhat);		//M == -W
	double radicalSq = mu*mu * (MdotN*MdotN - 1) + 1;
	Vector3d refracted = Vector3d::Zero();

	if (radicalSq >= 0)
	{
		double beta = mu*MdotN - sqrt(radicalSq);
		refracted = (-mu)*QEhat + beta*Nhat;
	}

	return refracted;
}

/*******************************************************************************
getExitRay() - 	creates a Ray in the direction exiting a sphere
arguments -		references to:
					- the direction of the ray bounce
					- the vector pointing from the intersection point to the eye
					- the normal vector at the point of intersection
					- the point at which the ray intersected something
returns -		either the refraction Ray or a null Ray
*******************************************************************************/
Ray Ray::getExitRay(Vector3d &refract, Vector3d &QEhat, Vector3d &Nhat, Vector3d &point, double in)
{
	refract = getRefractionDir(QEhat, point, Nhat, 1, in);
	if (refract(0, 0) != 0 || refract(1, 0) != 0 || refract(2, 0) != 0)
	{
		//				intersect	2 * (QC.T1 = double) * T1
		Vector3d exit = point + 2 * (refract.dot(closestSphere.center - point)*refract);
		Vector3d insideN = closestSphere.center - exit;
		insideN.normalize();
		refract = -refract;
		Vector3d exitRefraction = getRefractionDir(refract, exit, insideN, in, 1);

		return Ray(exit, exitRefraction);
	}
	else
	{
		return Ray();
	}
}

