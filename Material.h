#ifndef MATERIAL_H
#define MATERIAL_H

#include <Eigen/Dense>
using namespace Eigen;
#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::ostream;

class Material
{
	public:
		Material();
		~Material();

		string file = "";

		double Ns = 16;									//phong/'Ns':  This is the exponent in the phong model, i.e.what I typically call specular highlights. If it is present please use it.
		double reflectivity = 0;						//reflectivity - 'mirrorness'
		Vector3d ambient;								//ambient (ka) values						
		Vector3d diffuse;								//diffuse (kd) values
		Vector3d specular;								//specular (ks) values
		Vector3d energy;								//energy/emission (ke) values - NOT USED	'Ke' : Emissive constants.These relate to a more complicated 'Physically-based rendering' approach we are not covering.
		Vector3d attenuation = Vector3d(1, 1, 1);		//attenuation (kr) values: 111==mirror, 000==no light returned from bounces off of it
		Vector3d opacity = Vector3d(1, 1, 1);			//opacity (ko) values: 111==opaque, 000==completely transparant
		double IoR = 1;									//index of refraction: 1==air, 1.5==glass...
		double Ni = 0;									//'Ni': Quoting the original documentation 'are the degree n basis functions' - please ignore.
		double d = 0;									//'d': Amount of transparency.Ignore for the current assignment be be aware we will be using semi - transparent objects in later assignments.
		double illum = 2;								//'illum': Allowed values 0, 1 and 2.  2 means use ambient, diffuse and specular illumination. We will never set it to anything other than 2

		inline void printMat() { cout << "mat: " << file << ", Ns: " << Ns << ", ka: " << ambient.transpose() << ", kd: " << diffuse.transpose() 
									<< ", ks: " << specular.transpose() << ", ke: " << energy.transpose() << ", attenuation: " << attenuation.transpose()
									<< ", opacity: " << opacity.transpose() << ", IoR: " << IoR << ", Ni: " << Ni << ", d: " << d << ", illum: " 
									<< illum << ", reflectivity: " << reflectivity << endl; }

	private:

};

#endif




