#include "IObot.h"

IObot::IObot()
{
}
IObot::~IObot()
{
}

/*******************************************************************************
readDriver() - 	calls functions to parse the driver file and store data
arguments -		name of driver file, reference to a Scene object
returns -		true on success, false on failure
*******************************************************************************/
bool IObot::readDriver(const char input[], Scene &scene)
{
	if (!folderPrep(input))
		return false;

	fin.open(input);
	if (!fin.is_open())
	{
		string name = input;
		cerr << "error: IObot::readDriver() - unable to open DRIVER file '" << name << "', may not have permissions" << endl;
		return false;
	}

	if (!parseDriver(scene))
		return false;

	fin.close();
	return true;
}
bool IObot::folderPrep(const char input[])
{
	string infile = input;
	int len = infile.length();
	if (infile.substr(len - 4) != ".txt")
	{
		cerr << "error: IObot::folderPrep() - file must be a .txt file" << endl;
		return false;
	}
	
	path = infile;
	path.resize(len - 4);			//chop the extension off

	path.append("/");

	return true;
}
bool IObot::parseDriver(Scene &scene)
{
	linenum = 0;
	blanks = 0;

	int count = 0;
	nextLine(fin);					//overwrites line
	while (!fin.eof())				//loop through all lines of the file
	{
		string check;
		sstream >> check;

		if (check == "eye" || check == "look" || check == "up" || check == "sphere" || check == "ambient" || check == "light")		//3x double, + other stuff?
		{
			scene.driver.push_back(line);
			if (check == "eye")
			{
					scene.eye.position = getPoint();
			}
			else if (check == "look")
			{
					scene.eye.look = getPoint();
			}
			else if (check == "up")
			{
				scene.eye.up = getPoint();
			}
			else if (check == "sphere")									//move to outer ifs...
			{
				scene.spheres.push_back(getSphere(scene));
			}
			else if (check == "light")
			{
				scene.lights.push_back(getLight());
			}
			else if (check == "ambient")
			{
				scene.ambient = getPoint();
			}
		}
		else if (check == "d" || check == "recursionLevel")				//1x int/double
		{
			scene.driver.push_back(line);

			if (check == "d")
			{
				sstream >> scene.eye.d;
				if (scene.eye.d > 0)
				{
					scene.eye.d = -scene.eye.d;
				}
			}
			else if (check == "recursionLevel")
			{
				sstream >> scene.recursion;
			}
		}
		else if (check == "res" || check == "bounds")					//2x int/double
		{
			scene.driver.push_back(line);
			if (check == "res")
			{
				{
					sstream >> scene.eye.res[0];
					sstream >> scene.eye.res[1];
				}
			}
			else if (check == "bounds")
			{
				sstream >> scene.eye.left;
				sstream >> scene.eye.bottom;
				sstream >> scene.eye.right;
				sstream >> scene.eye.top;
				scene.eye.llCorner[0] = scene.eye.left;
				scene.eye.llCorner[1] = scene.eye.bottom;
				scene.eye.ruCorner[0] = scene.eye.right;
				scene.eye.ruCorner[1] = scene.eye.top;
			}
		}
		else if (check == "model")
		{
			scene.driver.push_back(line);
			getNums(4);
			getNums(1);
			getNums(3);

			sstream >> check;
			int len = check.length();
			if (check.substr(len - 4) != ".obj")
			{
				cerr << "error: IObot::parseDriver() - file must be a .obj file" << endl;
				return false;
			}
			check.resize(len - 4);
			object.filename = check;
			scene.models.push_back(object);
			object.clearOrientation();
		}
		else if (check[0] == '#')
		{
			while (!sstream.eof()) { sstream >> junk; }				//discard line
		}
		else
		{
			linenum--;
			count++;
			while (!sstream.eof()) { sstream >> junk; }
		}
		nextLine(fin);
	}

	if (!scene.eye.setup())
	{
		cerr << "error: IObot::parseDriver() - WUV setup failed" << endl;
		return false;
	}
	
	return true;
}
Vector3d IObot::getPoint()
{
	double x;
	sstream >> x;
	double y;
	sstream >> y;
	double z;
	sstream >> z;
	Vector3d out;
	out << x, y, z;

	return out;
}
Light IObot::getLight()
{
	Vector3d position;
	double w;
	Vector3d rgb;

	position = getPoint();
	sstream >> w;
	rgb = getPoint();

	return Light(position, w, rgb);
}
Sphere IObot::getSphere(Scene &scene)
{
	Vector3d center;
	double r;
	Vector3d amb;
	Vector3d diff;
	Vector3d spec;
	Vector3d att;
	Vector3d opa;
	opa << 1, 1, 1;
	double ior = 1;
	double phong = 16;

	center = getPoint();
	sstream >> r;
	amb = getPoint();
	diff = getPoint();
	spec = getPoint();
	att = getPoint();
	if (!sstream.eof())
		opa = getPoint();
	if (!sstream.eof())
		sstream >> ior;
	if (!sstream.eof())
		sstream >> phong;
	
	return Sphere(center, r, amb, diff, spec, att, opa, ior, phong);
}
void IObot::nextLine(ifstream &file)
{
	getline(file, line);
	linenum++;

	int end = line.size() - 1;
	if (end == -1 || end == 0)
	{
		blanks++;
	}
	else
	{
		while (line[end] == ' ' || line[end] == '\t' || line[end] == '\v' || line[end] == '\f')
		{
			line.resize(end);
		}
	}

	sstream.clear();				//CLEARS ERROR FLAGS, NOT CONTENT!!!!
	sstream.str(string());			//clears the content
	sstream << line;				//does not change line
}
void IObot::getNums(int quantity)
{
	int i = 0;
	while (!sstream.eof() && i < quantity)
	{
		double token;
		sstream >> token;
		if (quantity == 4)
			object.rotation[i] = token;
		else if (quantity == 1)
			object.scalar = token;
		else if (quantity == 3)
			object.translation[i] = token;
		i++;
	}
}

bool IObot::readModels(Scene &scene)
{

	for (unsigned int i = 0; i < scene.models.size(); i++)				//each model file
	{
		linenum = 0;
		line.clear();

		string file = scene.models[i].filename + ".obj";
		fin.open(file);
		if (!fin.is_open())
		{
			cerr << "error: IObot::readModels() - unable to open OBJECT file '" << file << "', may not have permissions" << endl;
			return false;
		}

		sstream = stringstream();
		
		int matI = -1 + i;

		bool comments = true;
		while (!fin.eof())												//each line of model file
		{
			nextLine(fin);

			string token;
			sstream >> token;
			if (token[0] != '#' && comments)		//as soon as the first char is not a #, turn off comments forever
			{
				comments = false;
			}

			if (comments)
			{
				scene.models[i].comments.append(sstream.str());
				scene.models[i].comments.append("\n");
\				sstream = stringstream();
			}
			else if (token == "mtllib")
			{
				sstream >> token;
				int len = token.length();
				if (token.substr(len - 4) != ".mtl")
				{
					cerr << "error: IObot::readModels() - file must be an .mtl file" << endl;
					return false;
				}

				if (!readMaterials(scene, i, token))
				{
					cerr << "error: IObot::readModels() - unable to open MATERIAL file, may not have permissions" << endl;
					return false;
				}
			}
			else if (token == "v" || token == "vn")
			{
				double x;
				double y;
				double z;
				Vector3d vert;

				for (int j = 0; !sstream.eof(); j++)
				{
					switch (j)
					{
						case 0:
							sstream >> x;
							break;
						case 1:
							sstream >> y;
							break;
						case 2:
							sstream >> z;
							break;
						default:
							while (!sstream.eof()) { sstream >> token; }
					}
				}
				vert << x, y, z;
				if (token == "v")
					scene.models[i].vertices.push_back(vert);
				if (token == "vn")
					scene.models[i].vns.push_back(vert);
				sstream = stringstream();
			}
			else if (token == "usemtl")
			{
				matI++;
			}
			else if (token == "f")
			{
				if (matI-int(i) == -1)											//model only has one material, doesnt use 'usemtl'
				{
					matI++;
				}

				Face face;
				face.matIndex = matI;

				for (int j = 0; !sstream.eof(); j++)							//for each tuple #/#/#
				{
					for (int k = 0; k < 3; k++)									//for each face - 3x tuples
					{
						string cornerS;
						sstream >> cornerS;
						string piece;
						stringstream corner(cornerS);
						for (int l = 0; l < 3; l++)								//for each # in one tuple
						{
							getline(corner, piece, '/');
							stringstream num(piece);
							int bit;			
							if (num >> bit)
							{
								if (l == 0)
									face.tuple[k][l] = bit;
								if (l == 2)
									face.tuple[k][l] = bit;
							}
						}
					}
				}
				scene.models[i].faces.push_back(face);
			}
			else
			{
				while (!sstream.eof()) { sstream >> token; }
			}
		}
		fin.close();
	}

	return true;
}

bool IObot::readMaterials(Scene & scene, int i, string &filename)
{
	if (!(scene.matMap.count(filename) > 0))
	{
		ifstream mtl;
		mtl.open(filename);
		if (!mtl.is_open())
		{
			cerr << "error: IObot::readMaterials() - unable to open MATERIAL file '" << filename << "', may not have permissions" << endl;
			return false;
		}

		sstream = stringstream();
		nextLine(mtl);
		linenum--;

		Material mat = Material();
		vector<int> list;
		mat.Ns = -44444;
		string inner;
		sstream >> inner;
		bool first = true;

		while (!mtl.eof())				//loop through all lines of the file
		{
			if (inner == "newmtl")
			{
				mat = Material();
				mat.file = filename;
				inner = "";
				first = false;
			}
			else if (inner == "Ka" || inner == "Kd" || inner == "Ks" || inner == "Ke" || inner == "Kr" || inner == "Ko")		//3x double, + other stuff?
			{
				if (inner == "Ka")									//ambient
				{
					mat.ambient = getPoint();
				}
				else if (inner == "Kd")									//diffuse
				{
					mat.diffuse = getPoint();
				}
				else if (inner == "Ks")									//specular
				{
					mat.specular = getPoint();
				}
				else if (inner == "Ke")									//energy - NOT USED
				{
					mat.energy = getPoint();
				}
				else if (inner == "Kr")									//attenuation
				{
					mat.attenuation = getPoint();
				}
				else if (inner == "Ko")									//opacity
				{
					mat.opacity = getPoint();
				}
			}
			else if (inner == "Ns" || inner == "Ni" || inner == "d" || inner == "illum")			//1x double
			{
				if (inner == "Ns")					//phong
				{
					sstream >> mat.Ns;
				}
				else if (inner == "Ni")				//NOT USED
				{
					sstream >> mat.Ni;
				}
				else if (inner == "d")				//transparency - 'd' : Amount of transparency.Ignore for the current assignment be be aware we will be using semi - transparent objects in later assignments.
				{
					sstream >> mat.d;
				}
				else if (inner == "illum")			//always 2
				{
					sstream >> mat.illum;
				}
			}

			nextLine(mtl);
			linenum--;
			inner = "";
			sstream >> inner;
			if ((!first && inner == "newmtl") || mtl.eof())
			{
				scene.mats.push_back(mat);
				list.push_back(int(scene.mats.size() - 1));
				scene.matMap[filename] = list;
			}
		}
		mtl.close();
	}

	return true;
}

bool IObot::writeOBJ(Scene &scene)
{
	if (!makeDir())
		return false;
	
	for (unsigned int i = 0; i < scene.models.size(); i++)
	{
		stringstream s;
		string num;
		s << setw(2) << setfill('0') << i;
		num = s.str();
		num.append(".obj");

		fout.open(path + scene.models[i].filename + "_mw" + num);
		if (!fout.is_open())
		{
			cerr << "error: IObot::writeOBJ() - unable to open output file '" << path + scene.models[i].filename + "_mw" + num << "', directory may not exist, or may not have permissions" << endl;
			return false;
		}

		if (scene.models[i].comments != "")
		{
			fout << scene.models[i].comments;
		}
		for (unsigned int j = 0; j < scene.models[i].vertices.size(); j++)
		{
			char buffer[100]; 
			snprintf(buffer, 100, "%.7f %.7f %.7f ", scene.models[i].vertices[j](0, 0), scene.models[i].vertices[j](1, 0), scene.models[i].vertices[j](2, 0));
			string output;
			output.append("v ");
			output.append(buffer);
			output.append("\n");

			fout << output;
		}
		for (unsigned int k = 0; k < scene.models[i].faces.size(); k++)
		{
			fout << scene.models[i].faces[k].str();
		}

		fout.close();
	}
	return true;
}

bool IObot::writePPM(Camera & camera, const char out[])
{
	stringstream s;
	string start;
	s << "P3\n" << camera.res[0] << " " << camera.res[1] << " " << 255 << "\n";
	start = s.str();

	fout.open(out);
	if (!fout.is_open())
	{
		cerr << "error: IObot::writePPM() - unable to open output file '" << out << "', directory may not exist, or may not have permissions" << endl;
		return false;
	}

	fout << start;
	for (unsigned int j = 0; j < camera.pic.size(); j++)
	{
		for (unsigned int k = 0; k < camera.pic[j].size(); k++)
		{
			fout << camera.pic[j][k].str();
		}
		fout << "\n";
	}

	fout.close();


	PPMtoPNG(string(out));

	return true;
}

bool IObot::PPMtoPNG(string out)
{
	int len = out.length();
	string command = "convert " + out;
	out.resize(len - 4);			//chop the extension off
	command.append(" " + out + ".png");

	const int dir_err = system(command.c_str());
	if (-1 == dir_err)
	{
		cout << "error: IObot::PPMtoPNG() - could not create file - permissions?";
		return false;
	}

	//removing large .ppm file after .png created
	command = "rm " + out + ".ppm";
	const int dir_err2 = system(command.c_str());
	if (-1 == dir_err2)
	{
		cout << "error: IObot::PPMtoPNG() - could not delete ppm file - permissions?";
		return false;
	}

	return true;
}

bool IObot::makeDir()
{
	string command = "mkdir -p " + path;
	const int dir_err = system(command.c_str());
	if (-1 == dir_err)
	{
		cout << "error: IObot::makeDir() - could not create directory - permissions?";
		return false;
	}

	return true;
}

