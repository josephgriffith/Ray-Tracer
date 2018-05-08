#include "Sphere.h"

Sphere::Sphere()
{
	null = true;
}
Sphere::Sphere(Vector3d c, double r, Vector3d a, Vector3d d, Vector3d s, Vector3d at, Vector3d op, double ior, double phong)
{
	center = c;
	radius = r;
	mat.ambient = a;
	mat.diffuse = d;
	mat.specular = s;
	mat.attenuation = at;
	mat.opacity = op;
	mat.IoR = ior;
	mat.Ns = phong;
}
Sphere::~Sphere()
{
}


