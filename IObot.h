#ifndef IOBOT_H
#define IOBOT_H

#include <fstream>
using std::ifstream;
using std::ofstream;
#include <sstream>
using std::stringstream;

#include "Scene.h"
#include "Camera.h"

#include <cstdlib>
#include <iomanip>
using std::setw;
using std::setfill;
using std::fixed;
using std::setprecision;


class IObot
{
	public:
		IObot();
		~IObot();

		string path;
		string junk;

		bool readDriver(const char input[], Scene &scene);		//assumes it will be read first, calls filePrep
		bool parseDriver(Scene &scene);							//populates scene's models with transformation information
		Vector3d getPoint();
		Light getLight();
		Sphere getSphere(Scene &scene);

		bool readModels(Scene &scene);										//loads the .obj files in scene's models, and populates their vertices/faces
		bool readMaterials(Scene &scene, int i, string &file);				//

		bool makeDir();														//attempts to make the output directory
		bool writeOBJ(Scene &scene);										//assumes that folderPrep has been run
		bool writePPM(Camera &camera, const char out[]);					//assumes that folderPrep has been run
		bool PPMtoPNG(string out);											//attempts to convert the .ppm file to a .png

	private:
		ifstream fin;
		stringstream sstream;
		string line;
		int blanks;
		int linenum;

		Model object;

		void nextLine(ifstream &fin);							//loads next driver line in sstream, and removes trailing whitespace
		bool folderPrep(const char input[]);					//fills in file and path variables
		void getNums(int quantity);								//loads object's rotation, scale, translation

		ofstream fout;
};

#endif




