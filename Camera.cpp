#include "Camera.h"

Camera::Camera()
{
	pic.reserve(2048);
}
Camera::~Camera()
{
}

/*******************************************************************************
takePicture() - 	goes through the process of 'taking a picture' by simulating
						a ray of light shooting from the eye/camera through each 
						pixel of the image, and any resulting bounces or refractions
						of that ray
					stores the accumulated light and color that each pixel should have
arguments -		a reference to a Scene 
*******************************************************************************/
void Camera::takePicture(Scene &scene)
{
	junk << NOTEE, NOTEE, NOTEE;
	res[0] = scene.eye.res[0];
	res[1] = scene.eye.res[1];

	double percentcheck = 0.0;
	int numPixels = scene.eye.res[0]*scene.eye.res[1];
	int progress = 0;

	for (int i = 0; i < scene.eye.res[1]; i++)					//swapped width/height here and in getPixelPoint() to fix the non-square resolution problems
	{
		vector<Vector3d> innerT;
		vector<Ray> rayRow;
		vector<Vector3d> colorRow;
		for (int j = 0; j < scene.eye.res[0]; j++)				//image pixels
		{
			pixelpt = getPixelPoint(i, j, scene.eye);
			direction = (pixelpt - scene.eye.position) / (pixelpt - scene.eye.position).norm();
			ray = Ray(pixelpt, direction);

			rayTrace(scene, ray, pixelColor, attenuation, scene.recursion);
			colorRow.push_back(pixelColor);

			saveReset(innerT, rayRow);

			if (double(progress) / numPixels >= percentcheck)
			{	//		...---===^^^		progress bar indicators
				if (percentcheck < .25)	cout << "." << flush;	else if (percentcheck < .5)	cout << "-" << flush;	else if (percentcheck < .75)	cout << "=" << flush;	else	cout << "^" << flush;
				percentcheck += .01;
			}
			progress++;
		}
		rays.push_back(rayRow);
		colors.push_back(colorRow);
	}
	fillImage();												//populates the picture with final color values

	cout << "\npic size: " << pic[0].size() << "x" << pic.size() << endl;
}

void Camera::saveReset(vector<Vector3d> &innerT, vector<Ray> &rayRow)
{
	rayRow.push_back(ray);
	ray.closestDist = NOTEE;
	pixelColor << 0, 0, 0;
	attenuation << 1, 1, 1;
}

void Camera::fillImage()
{
	for (unsigned int i = 0; i < colors.size(); i++)
	{
		Row row;
		for (unsigned int j = 0; j < colors[i].size(); j++)
		{
			pixel pix;
			pix.set(checkPixel(colors[i][j](0,0)), checkPixel(colors[i][j](1,0)), checkPixel(colors[i][j](2,0)));
			row.push_back(pix);
		}
		pic.push_back(row);
	}
}

int Camera::checkPixel(double num)
{
	int out = num * 255;
	return max(0, min(out, 255));
}

Vector3d Camera::getPixelPoint(int j, int i, Eye &eye)
{
	//Adapted from the cs410lec07n01 jupyter/sagemath notebooks
	double px = (i/(eye.res[0]-1.0)) * (eye.right-eye.left)+eye.left;
	double py = (j/(eye.res[1]-1.0)) * (eye.top-eye.bottom)+eye.bottom;
	//		start at eye	move d in dir W		move px in U dir	move py in V dir
	return (eye.position + (eye.d*eye.W)	+ (px*eye.U)		+ (py*eye.V));
}

Vector3d Camera::rayTrace(Scene & scene, Ray &ray, Vector3d &colorAccumulator, Vector3d &passedAttenuation, int remaining)
{
	checkSpheres(scene, ray, false);
	checkModels(scene, ray, false);

	if (ray.hit != Ray::status::miss)
	{
		Vector3d ka;
		Vector3d kd;
		Vector3d ks;
		Vector3d kr;
		Vector3d ko;
		Vector3d Nhat;
		Vector3d QEhat;
		int PHONG;
		double ior = 1;

		if (ray.hit == Ray::status::sphere)
		{
			ka = ray.closestSphere.mat.ambient;
			Nhat = ray.closestPoint - ray.closestSphere.center;			//Q-O, from sphere center to intersection, normalized
			kd = ray.closestSphere.mat.diffuse;
			ks = ray.closestSphere.mat.specular;
			kr = ray.closestSphere.mat.attenuation;
			ko = ray.closestSphere.mat.opacity;
			PHONG = ray.closestSphere.mat.Ns;
			ior = ray.closestSphere.mat.IoR;
		}
		else if (ray.hit == Ray::status::model)
		{
			ka = scene.mats[	ray.faceMatIndex	].ambient;
			kd = scene.mats[	ray.faceMatIndex	].diffuse;
			ks = scene.mats[	ray.faceMatIndex	].specular;
			kr = scene.mats[	ray.faceMatIndex	].attenuation;
			ko = scene.mats[	ray.faceMatIndex	].opacity;
			PHONG = scene.mats[	ray.faceMatIndex	].Ns;
			Nhat = ray.modelNhat;									//(A-B)X(C-A), normalized
		}
		Nhat.normalize();
		QEhat = -ray.direction;										//E-Q, from intersection to ray origin, normalized
		QEhat.normalize();
		
		Vector3d pixelColor;
		pixelColor = pairwiseProduct(scene.ambient, ka);			//ambient

		for (unsigned int i = 0; i < scene.lights.size(); i++)
		{
			Vector3d QLhat = scene.lights[i].point - ray.closestPoint;		//L-Q, from intersection to light, normalized
			
			Ray lightTest = Ray(ray.closestPoint, QLhat);
			lightTest.direction.normalize();
			double lightDist = QLhat.dot(lightTest.direction);

			checkSpheres(scene, lightTest, false);
			checkModels(scene, lightTest, false);

			if (lightTest.closestDist == NOTEE || lightTest.closestDist > lightDist)
			{
				QLhat.normalize();

				Vector3d light = scene.lights[i].rgb;
				Vector3d Phat = getBounce(Nhat, QLhat);		//P, from intersection in direction of bounce
				Phat.normalize();

				if (!(Nhat.dot(QEhat) > 0))
					Nhat = -Nhat;
				if (QLhat.dot(Nhat) > 0)					//light source behind intersection point surface
					pixelColor += pairwiseProduct(light, kd) * Nhat.dot(QLhat);									//diffuse
				if (QEhat.dot(Phat) > 0)																					//fixes specular halo bug
					pixelColor += pairwiseProduct(light, ks) * (pow(Phat.dot(QEhat), PHONG));					//specular
			}
		}
		colorAccumulator += pairwiseProduct(pairwiseProduct(passedAttenuation, pixelColor), ko);

		if (remaining > 0)
		{
			Vector3d bounceDir = getBounce(Nhat, QEhat);
			Ray newRay = Ray(ray.closestPoint, bounceDir);
			Vector3d newAttenuation = pairwiseProduct(passedAttenuation, kr);
			Vector3d recColor = Vector3d::Zero();
			
			if (newAttenuation(0, 0) != 0 || newAttenuation(1, 0) != 0 || newAttenuation(2, 0) != 0)
			{
				rayTrace(scene, newRay, recColor, newAttenuation, remaining - 1);
				colorAccumulator += pairwiseProduct(pairwiseProduct(newAttenuation, ko), recColor);
			}

			if (ko(0, 0) < 1 || ko(1, 0) < 1 || ko(2, 0) < 1)		//if not opaque
			{
				bounceDir = Vector3d::Zero();						//inside direction
				newRay.originPoint = Vector3d::Zero();
				recColor = Vector3d::Zero();
				newRay = ray.getExitRay(bounceDir, QEhat, Nhat, ray.closestPoint, ior);		//inverts bounceDir

				if (!newRay.null)
				{
					rayTrace(scene, newRay, recColor, newAttenuation, remaining - 1);
					for (int i = 0; i < 3; i++)
						{	colorAccumulator(i, 0) += newAttenuation(i, 0) * (1 - ko(i, 0)) * recColor(i, 0);	}
				}
			}
		}
	}
	else
		{ }

	return colorAccumulator;
}

void Camera::checkModels(Scene &scene, Ray &ray, bool light)
{
	double localmax = numeric_limits<float>::min();

	for (unsigned int i = 0; i < scene.models.size(); i++)						//for each model
	{
		Vector3d checkHit;
		checkHit = Vector3d::Zero();
		for (unsigned int j = 0; j < scene.models[i].faces.size(); j++)			//for each face
		{
			double a = scene.models[i].faces[j][0][0];
			double b = scene.models[i].faces[j][1][0];
			double c = scene.models[i].faces[j][2][0];

			A = scene.models[i].vertices[a-1];
			B = scene.models[i].vertices[b-1];
			C = scene.models[i].vertices[c-1];

			vx = A - B;
			vy = A - C;
			y = A - ray.originPoint;

			//sets beta, gamma, and t
			checkHit = cramerMXY(vx, vy, ray.direction, y);
						
			if (!light)
			{
				t = checkHit(2, 0);
				//}


				if (t >= 0 && t > 0.000001 && beta >= 0 && gamma >= 0 && (beta + gamma) <= 1.000001)
				{
					//compare any previous t values to current value and keep the smaller one
					if (checkHit(2, 0) <= ray.closestDist || ray.closestDist == NOTEE)			//new closest intersection
					{
						tmin = (tmin > t) ? t : tmin;		//(if) ? true : false
						localmax = t;
						ray.hit = Ray::status::model;

						ray.closestPoint = (ray.direction*t) + ray.originPoint;
						ray.closestDist = checkHit[2];
						ray.closestModel = scene.models[i];
						ray.faceMatIndex = scene.models[i].faces[j].matIndex;
						ray.modelNhat = ((A - B).cross(C - A));
						ray.modelNhat.normalize();
						ray.closestSphere = nullsphere;
					}
				}
			}
		}
		if (!light)
			tmax = (tmax < localmax) ? localmax : tmax;
	}
}

void Camera::checkSpheres(Scene &scene, Ray &ray, bool light)
{
	double checkHit = NOSPHEREHIT;			//t value - distance to intersection
	ray.closestDist = NOTEE;
	ray.closestSphere = nullsphere;

	for (unsigned int i = 0; i < scene.spheres.size(); i++)
	{
		if (hitSphere(scene, ray, i, checkHit))			//writes checkHit
		{
			if ((checkHit < ray.closestDist && checkHit > 0.000001) || (ray.closestDist == NOTEE && checkHit > 0.000001))
			{
				//beta and gamma are never set when spheres are being checked
				tmin = (tmin > checkHit) ? checkHit : tmin;		//(if) ? true : false
				tmax = (tmax < checkHit) ? checkHit : tmax;

				ray.hit = Ray::status::sphere;
				ray.closestPoint = (checkHit*ray.direction) + ray.originPoint;			//Q: point of intersection
				ray.closestSphere = scene.spheres[i];
				ray.closestDist = checkHit;
			}
		}
	}
}

bool Camera::hitSphere(Scene &scene, Ray &ray, int i, double &checkHit)
{
	double v;
	double cSquared;
	double dSquared;

	//trig: a^2 + b^2 = r^2 
	//ray-sphere intersection 2D slice: 2 triangles share their 'b' side - 
	//a1, r1 belong to the smaller circle/ray's triangle, and a2, r2 belong to the larger eye/ray/center's triangle
	//circle/ray triangle -     sides: a1=d, b, r1=r - 2 named vertices: O = circle center/'top' of b, Q = ray-sphere intersection
	//eye/ray/center triangle - sides: a2=v, b, r2=c - 2 named vertices: O = circle center/'top' of b, E = eye (or a given pixel point)
	//(vector) T = O-E, Uhat = unit vector in direction of the ray, v = Uhat dot T, c^2 = T dot T, d^2 = r^2 - (c^2-v^2)
	//if d^2 <= 0, there was an intersection and the distance from eye/pixel to sphere surface is t = v-d
	T = scene.spheres[i].center - ray.originPoint;										//T = O-E
	v = ray.direction.dot(T);															//v = Uhat.T
	cSquared = T.dot(T);																//c^2 = T.T
	dSquared = scene.spheres[i].radius*scene.spheres[i].radius - (cSquared - v*v);		//d^2 = r^2 - (c^2-v^2)

	bool yes = dSquared > 0;
	if (yes)
		checkHit = v - sqrt(dSquared);							//t = v-d

	return yes;
}

Vector3d Camera::cramerMXY(Vector3d &vx, Vector3d &vy, Vector3d &vz, Vector3d &y)
{
	Matrix3d M;
	Vector3d x;

	M << vx, vy, vz;
	double d = M.determinant();
	if (d == 0)
	{
		x << 0, 0, 0;
		beta = NOBETA;
		gamma = NOGAMMA;
		return x;
	}

	M << y, vy, vz;
	double dx = M.determinant();
	M << vx, y, vz;
	double dy = M.determinant();
	M << vx, vy, y;
	double dz = M.determinant();
	beta = dx/d;
	gamma = dy/d;
	x << beta, gamma, dz/d;

	return x;
}

//taken from https://stackoverflow.com/questions/20792445/calculate-rgb-value-for-a-range-of-values-to-create-heat-map
pixel Camera::getColor(double num, double mini, double maxi)
{
	double ratio = 2*(num-mini)/(maxi-mini);
	int r = max(0, 255 * (1-ratio));					//SWITCHED R AND B TO GET COLOR WORKING
	int b = max(0, 255 * (ratio-1));
	int g = 255 - b - r;
	
	pixel px;
	if (num > 0)
		px.set(r, g, b);
	else 
		{ ; }
	return px;
}

int Camera::min(int a, int b)
{
	return (a>b) ? b : a;
}
int Camera::max(int a, int b)
{
	return (a<b) ? b : a;
}


