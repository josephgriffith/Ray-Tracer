#ifndef EYE_H
#define EYE_H

#include <Eigen/Dense>
using namespace Eigen;
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::ostream;

class Eye
{
	public:
		Eye();
		~Eye();
		
		Vector3d position = Vector3d(-4444, -4444, -4444);			//coordinates of the eye																world coords
		Vector3d look = Vector3d(-4444, -4444, -4444);				//coordinates of where the eye will look												world coords
		Vector3d up;												//what will be the y-axis of the image -- NOT NECESSARILY y-axis of the eye!!!			eye coords

		double d = 0;				//distance from eye to image/near clipping plane
		
		//image/pixel plane bounds
		double llCorner[2];			//left lower (x,y) corner of the image pixels											eye coords
		double ruCorner[2];			//right upper (x,y) corner of the image pixels											eye coords

		double left;
		double bottom;
		double right;
		double top;

		int res[2];					//resolution of image (x,y)

		Vector3d W;				//-Z axis
		Vector3d U;				//X axis
		Vector3d V;				//Y axis
		//TWUE WUV~

		bool setup();

	private:
};


#endif