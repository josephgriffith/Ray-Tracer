#ifndef MODEL_H
#define MODEL_H

#include <vector>
using std::vector;
#include <algorithm>
using std::fill;
#include <Eigen/Dense>
using namespace Eigen;
#include "Material.h"


struct FaceTuple
{
	inline string str() { char buffer[100]; snprintf(buffer, 100, "%d//%d", tuple[0], tuple[2]); string output = buffer; return output; }

	inline const int& operator [] (int i) const { return tuple[i]; }
	inline int& operator [] (int i) { return tuple[i]; }

	int tuple[3];
};
struct Face
{
	FaceTuple tuple[3];				//a single 2//1 (from model file)
	int matIndex = 0;				//index in scene's materials

	inline const FaceTuple& operator [] (int i) const { return tuple[i]; }
	inline FaceTuple& operator [] (int i) { return tuple[i]; }

	inline string str() { char buffer[100]; snprintf(buffer, 100, "f %s %s %s\n", tuple[0].str().c_str(), tuple[1].str().c_str(), tuple[2].str().c_str()); string output = buffer; return output; }
};

class Model
{
	public:
		Model();
		~Model();

		string filename;
		string comments = "";
		double rotation[4];
		double scalar;
		double translation[3] = {0,0,0};

		vector<Vector3d> vertices;			//vert[0-n][0-2] -> x,y,z; x,y,z...	
		vector<Vector3d> vns;			
		vector<Face> faces;

		Matrix<double, 4, Dynamic> points;
		Matrix4d rotate;
		Matrix4d scale;
		Matrix4d translate;

		inline void printVertices() { for (unsigned int i = 0; i < vertices.size(); i++) { cout << "vertices[" << i << "]: " << vertices[i].transpose() << endl; } }
		inline void printVNs()		{ for (unsigned int i = 0; i < vns.size(); i++) { cout << "VNs[" << i << "]: " << vns[i].transpose() << endl; } }
		inline void printFaceFail()	{ for (unsigned int i = 0; i < faces.size(); i++) { if (faces[i].matIndex < 0) cout << "face[" << i << "].matIndex: " << faces[i].matIndex << endl; } }
		inline void printModel() { cout << rotation[0] << " " << rotation[1] << " " << rotation[2] << " " << rotation[3] << ", " << scalar << ", "
										<< translation[0] << " " << translation[1] << " " << translation[2] << ", model:" << filename << "\n"; }

		inline void clearOrientation() { fill(rotation, rotation + 4, 0); scalar = 0; fill(translation, translation + 3, 0); filename = ""; }

	private:
};

#endif