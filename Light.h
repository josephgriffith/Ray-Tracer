#ifndef LIGHT_H
#define LIGHT_H

#include <Eigen/Dense>
using namespace Eigen;
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::ostream;

class Light
{
	public:
		Light();
		Light(Vector3d p, double w, Vector3d rbg);
		~Light();

		Vector3d point;									//coordinates of light source
		double w;										//0 means light is 'infinitely' far away 
		Vector3d rgb;									//color values of the light source


		inline void printLight() { cout << "location: " << point.transpose() << ", w: " << w << ", rgb: " << rgb.transpose() << endl; }

	private:

};

#endif




