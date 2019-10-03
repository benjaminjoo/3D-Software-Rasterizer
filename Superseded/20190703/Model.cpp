#include "Model.h"
#include <string>
#include <sstream>


Model::Model()
{
	filename = "ace_of_spades.stl";

	x = 0.0;
	y = 0.0;
	z = 0.0;

	scale = 1.0;

	colour = 255;
}


Model::Model(string f, double cx, double cy, double cz)
{
	filename = f;

	x = cx;
	y = cy;
	z = cz;

	scale = 1.0;

	colour = 255;
}


Model::Model(string f, double cx, double cy, double cz, double sc, Uint32 col)
{
	filename = f;

	x = cx;
	y = cy;
	z = cz;

	scale = sc;

	colour = col;
}


Model::~Model()
{
}


int Model::getTotalPoly()
{
	int nPoly = 0, nLine = 0;
	ifstream modelFile(filename);
	if (modelFile.is_open())
	{
		printf("Model file successfully accessed...\n");
		string currentLine;
		while (!modelFile.eof())
		{
			getline(modelFile, currentLine);
			nLine++;
		}
		nPoly = (nLine - 2) / 7;
		modelFile.close();
	}
	else
	{
		printf("Couldn't access model file!\n");
	}
	return nPoly;
}


triangle3dV* Model::getTriangleData()
{
	int totalPoly = this->getTotalPoly();
	triangle3dV* t = new triangle3dV[totalPoly];

	if (t != NULL)
	{
		ifstream modelFile(filename);
		if (modelFile.is_open())
		{
			string currentLine;
			getline(modelFile, currentLine);
			double xCoord, yCoord, zCoord;
			char normal_crap[] = "facet normal";
			char vertex_crap[] = "vertex";
			char space[] = " ";
			for (int i = 0; i < totalPoly; i++)
			{
				getline(modelFile, currentLine);
				getline(modelFile, currentLine);		//"outer loop"
				for (int v = 0; v < 3; v++)
				{
					getline(modelFile, currentLine);
					stringstream(currentLine) >> normal_crap >> xCoord >> yCoord >>  zCoord;
					//printf("%.4f\t%.4f\t%.4f\n", x, y, z);
					if (i < totalPoly)
					{
						switch (v)
						{
						case 0:
							t[i].A.x = x + xCoord * scale;
							t[i].A.y = y + yCoord * scale;
							t[i].A.z = z + zCoord * scale;
							t[i].A.w = 1.0;
							break;
						case 1:
							t[i].B.x = x + xCoord * scale;
							t[i].B.y = y + yCoord * scale;
							t[i].B.z = z + zCoord * scale;
							t[i].B.w = 1.0;
							break;
						case 2:
							t[i].C.x = x + xCoord * scale;
							t[i].C.y = y + yCoord * scale;
							t[i].C.z = z + zCoord * scale;
							t[i].C.w = 1.0;
							break;
						}
					}
				}
				vect3 temp = crossProduct(subVectors(t[i].B, t[i].A), subVectors(t[i].C, t[i].B));
				t[i].An = t[i].Bn = t[i].Cn = t[i].N = unitVector(temp);
				t[i].N.w = 0.0;
				//printf("%.4f\t%.4f\t%.4f\n", t[i].N.x, t[i].N.y, t[i].N.z);
				getline(modelFile, currentLine);		//"endloop"
				getline(modelFile, currentLine);		//"endfacet"
			}

			modelFile.close();
		}
		else
		{
			printf("Couldn't access model file!\n");
		}
	}

	return t;
}


Uint32 Model::getColour()
{
	return colour;
}


unsigned char Model::getRedValue()
{
	return ((unsigned char)(colour >> 16 & 0xFF));
}


unsigned char Model::getGreenValue()
{
	return ((unsigned char)(colour >> 8 & 0xFF));
}


unsigned char Model::getBlueValue()
{
	return ((unsigned char)(colour & 0xFF));
}
