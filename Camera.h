#ifndef CAMERA_H
#define CAMERA_H

#define NOSPHEREHIT -11111
#define NOBETA -22222
#define NOGAMMA -33333
#define NOTEE -44444

#include "Eye.h"
#include "Scene.h"
#include <limits>
using std::numeric_limits;
#include "Ray.h"
#include <math.h>
using std::pow;
using std::flush;

typedef int color;

struct pixel
{
	color rgb[3] = { 0, 0, 0 };

	inline bool set(color r, color g, color b) { if (setR(r) && setG(g) && setB(b)) { return true; } else { return false; } }

	inline bool checkRange(color num) { if (num >= 0 && num <= 255) { return true; } else { return false; } }
	inline bool setR(color num) { if (checkRange(num)) { rgb[0] = num; return true; } else { cerr << "pixel::setR() - number not in range 0-255" << endl; return false; } }
	inline bool setG(color num) { if (checkRange(num)) { rgb[1] = num; return true; } else { cerr << "pixel::setG() - number not in range 0-255" << endl; return false; } }
	inline bool setB(color num) { if (checkRange(num)) { rgb[2] = num; return true; } else { cerr << "pixel::setB() - number not in range 0-255" << endl; return false; } }

	inline color r() { return rgb[0]; }
	inline color g() { return rgb[1]; }
	inline color b() { return rgb[2]; }

	inline string str() { char buffer[100]; snprintf(buffer, 100, "%3d %3d %3d ", rgb[0], rgb[1], rgb[2]); string output = buffer; return output; }

	inline const int& operator [] (int i) const { return rgb[i]; }
	inline int& operator [] (int i) { return rgb[i]; }
};
struct Row
{
	vector<pixel> element;

	inline unsigned int size() const { return element.size(); }
	inline void push_back(pixel p) { element.push_back(p); }
	inline void reserve(int i) { element.reserve(i); }
	inline void reserve(unsigned int i) { element.reserve(i); }
	inline void clear() { element.clear(); }

	inline const pixel& operator [] (int i) const { return element[i]; }
	inline pixel& operator [] (int i) { return element[i]; }
};
struct Image
{
	vector<Row> matrix;

	inline unsigned int size() const { return matrix.size(); }
	inline void push_back(Row r) { matrix.push_back(r); }
	inline void reserve(int i) { matrix.reserve(i); }
	inline void reserve(unsigned int i) { matrix.reserve(i); }
	inline void clear() { matrix.clear(); }

	inline const Row& operator [] (int i) const { return matrix[i]; }
	inline Row& operator [] (int i) { return matrix[i]; }
};

class Camera
{
	public:
		Camera();
		~Camera();

		Image pic;
		int res[2];

		double beta = NOBETA;
		double gamma = NOGAMMA;
		double t;
		double tmin = numeric_limits<float>::max();
		double tmax = numeric_limits<float>::min();

		void takePicture(Scene &scene);
		void saveReset(vector<Vector3d> &innerT, vector<Ray> &rayRow);
		void fillImage();
		int checkPixel(double num);
		Vector3d getPixelPoint(int i, int j, Eye &eye);
		
		Vector3d pixelColor = Vector3d(0,0,0);
		Vector3d attenuation = Vector3d(1, 1, 1);
		vector<vector<Vector3d>> colors;
		Vector3d rayTrace(Scene & scene, Ray &ray, Vector3d &color, Vector3d &attenuation, int remaining);

		inline const Vector3d getBounce(const Vector3d & normal, const Vector3d & negDirection) {	return (2 * (normal.dot(negDirection))*normal - negDirection);	}
		inline const Vector3d pairwiseProduct(const Vector3d &A, const Vector3d &B)				{	return Vector3d(A(0,0)*B(0,0), A(1,0)*B(1,0), A(2,0)*B(2,0));	}
		inline const double pointDistance(const Vector3d &A, const Vector3d &B)					
				{	return sqrt(	(A(0,0)-B(0,0))*(A(0,0)-B(0,0))	+	(A(1,0)-B(1,0))*(A(1,0)-B(1,0))	+	(A(2,0)-B(2,0))*(A(2,0)-B(2,0))		);	}

		Vector3d pixelpt;
		Vector3d direction;

		Vector3d T;

		Vector3d A;
		Vector3d B;
		Vector3d C;
		Vector3d vx;
		Vector3d vy;
		Vector3d y;

		void checkModels(Scene &scene, Ray &ray, bool light);
		void checkSpheres(Scene &scene, Ray &ray, bool light);

		bool hitSphere(Scene & scene, Ray & ray, int i, double &checkHit);
		
		Vector3d closestIntersection;
		Vector3d junk;
		Ray ray;
		Sphere nullsphere;
		vector<vector<Ray>> rays;				//ray, distance, sphere, and point for every pixel

		pixel getColor(double num, double min, double max);
		int min(int a, int b);
		int max(int a, int b);

		Vector3d cramerMXY(Vector3d &vx, Vector3d &vy, Vector3d &vz, Vector3d &y);

		

	private:

};

#endif







































