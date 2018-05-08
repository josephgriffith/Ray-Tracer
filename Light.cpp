#include "Light.h"

Light::Light()
{
}
Light::Light(Vector3d P, double W, Vector3d RGB)
{
	point = P;
	w = W;
	rgb = RGB;
}
Light::~Light()
{
}

