#ifndef CALCULON_H
#define CALCULON_H

#include "Scene.h"
#include <math.h>
using std::sqrt;
using std::pow;
using std::rand;

class Calculon
{
	public:
		Calculon();
		~Calculon();

		bool applyTransformation(Scene &scene);

		void fillPoints(Model &model);

		void transformModel(Model &model);

		Vector4d makeW(Model &model);
		Vector4d makeU(Vector4d &w);
		Vector4d makeV(Vector4d &w, Vector4d &u);
		Matrix4d makeRz(Model &model);
		Matrix4d makeR(Model &model, Vector4d &u, Vector4d &v, Vector4d &w);
		bool rotateModel(Model &model);

		void scaleModel(Model &model);
		void translateModel(Model &model);

		void writeVertices(Scene &scene);

	private:
};


#endif