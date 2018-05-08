#include "Calculon.h"

Calculon::Calculon()
{
}
Calculon::~Calculon()
{
}

/*******************************************************************************
applyTransformation() -	calls functions to apply transformations to loaded model file
arguments -		reference to a Scene object containing models to be 
					rotated, scaled, and moved into place
returns -		true
*******************************************************************************/
bool Calculon::applyTransformation(Scene & scene)
{
	for (unsigned int i = 0; i < scene.models.size(); i++)
	{
		fillPoints(scene.models[i]);

		transformModel(scene.models[i]);
	}
	return true;
}

void Calculon::fillPoints(Model &model)
{
	unsigned int size = model.vertices.size();
	model.points.conservativeResize(4, size);

	for (unsigned int k = 0; k < size; k++)
	{
		model.points.col(k) << model.vertices[k](0,0), model.vertices[k](1,0), model.vertices[k](2,0), 1;
	}
}
void Calculon::transformModel(Model & model)
{
	//in order
	rotateModel(model);
	scaleModel(model);
	translateModel(model);
}
bool Calculon::rotateModel(Model &model)
{
	//normalize Axis of Rotation: aor == w
	Vector4d aor = makeW(model);
	Vector4d u = makeU(aor);
	Vector4d v = makeV(aor, u);
	Matrix4d R = makeR(model, u, v, aor);		//copies into model

	model.points = R*model.points;

	return true;
}

Vector4d Calculon::makeW(Model &model)
{
	Vector4d aor;
	aor << model.rotation[0], model.rotation[1], model.rotation[2], 0.0;

	aor.normalize();

	return aor;
}

Vector4d Calculon::makeU(Vector4d &aor)
{
	//pick random, nonparallel-to-AoR axis
		//pick smallest aor value, set to 1 and renormalize to get: m
	Vector4d u;
	u.setZero();

	bool zero = u.isZero();

	srand(time(NULL));

	Vector4d m;		// = aor;
	while (zero)
	{
		m(0, 0) = rand()%1000;
		m(1, 0) = rand()%1000;
		m(2, 0) = rand()%1000;
		m(3,0) = 0;

		//create u = aor cross m, m no longer needed
		u = aor.cross3(m);

		zero = u.isZero();
	}

	return u;
}

Vector4d Calculon::makeV(Vector4d &aor, Vector4d &u)
{
	//pick axis v _|_ to aor and u - either v = aorXu or v = uXaor
	Vector4d v = aor.cross3(u);

	return v;
}

Matrix4d Calculon::makeRz(Model & model)
{
	Matrix4d Rz;
	//Rz - rotate by theta around aor
	double pi = 3.14159265358979323846264338327950288419716939937510582;
	double cosine = cos(model.rotation[3] * (pi / 180));
	double sine = sin(model.rotation[3] * (pi / 180));

	Rz <<  cosine,	-sine,	0,	0,
			 sine, cosine,	0,	0,
				0,		0,	1,	0,
				0,		0,	0,	1;

	return Rz;
}

Matrix4d Calculon::makeR(Model &model, Vector4d &u, Vector4d &v, Vector4d &aor)
{
	//construct matrix Raor:		u		0
	//								v		0
	//								aor		0
	//							0	0	0	1
	Matrix4d Raor;

	u.normalize();
	v.normalize();
	Raor <<   u(0, 0),	 u(1, 0),	u(2, 0),	0,
			  v(0, 0),	 v(1, 0),	v(2, 0),	0,
			aor(0, 0), aor(1, 0), aor(2, 0),	0,
				    0,		   0,		  0,	1;

	Matrix4d Rz = makeRz(model);
	//Rt - return to original frame of reference
	Matrix4d Rt;
	Rt = Raor.inverse();

	model.rotate = Rt*Rz*Raor;

	return model.rotate;
}

void Calculon::scaleModel(Model &model)
{
	model.scale << model.scalar,		0,				0,			0,
						0,			model.scalar,		0,			0,
						0,				0,			model.scalar,	0,
						0,				0,				0,			1;

	model.points = model.scale*model.points;
}

void Calculon::translateModel(Model &model)
{
	model.translate <<	1,		0,		0,	model.translation[0],
						0,		1,		0,	model.translation[1],
						0,		0,		1,	model.translation[2],
						0,		0,		0,		1;

	model.points = model.translate*model.points;
}

void Calculon::writeVertices(Scene & scene)
{
	for (unsigned int i = 0; i < scene.models.size(); i++)
	{
		for (unsigned int j = 0; j < scene.models[i].vertices.size(); j++)
		{
			scene.models[i].vertices[j] << (scene.models[i].points(0, j)), (scene.models[i].points(1, j)), (scene.models[i].points(2, j));
		}
	}
}
