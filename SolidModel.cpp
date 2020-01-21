#include "SolidModel.h"


SolidModel::SolidModel()
{
}


SolidModel::SolidModel(float px, float py, float pz, Uint32 c, int t)
{
	scale = { 1.0, 1.0, 1.0, 1.0 };
	position = { px, py, pz, 1.0 };
	rotation = { 0.0, 0.0, 0.0, 1.0 };

	colour = c;
	texture = t;
	txU = 1.0;

	castsShadows = true;
}


SolidModel::SolidModel(float sx, float sy, float sz, float px, float py, float pz, float rx, float ry, float rz, Uint32 c, int t)
{
	scale = { sx, sy, sz, 1.0 };
	position = { px, py, pz, 1.0 };
	rotation = { rx, ry, rz, 1.0 };

	colour = c;
	texture = t;
	txU = 1.0;

	castsShadows = true;
}


SolidModel::~SolidModel()
{

}


int	SolidModel::getTotalVert()
{
	return 0;
}


int	SolidModel::getTotalPoly()
{
	return polyContainer.size();
}


void SolidModel::getTriangleData(triangle3dV* P)
{
	nPoly = polyContainer.size();

	for (auto i = 0; i < nPoly; i++)
	{
		P[i] = polyContainer[i];
	}

	Projector->transformMesh(nPoly, P, scale.x, scale.y, scale.z,
										position.x, position.y, position.z,
										rotation.x, rotation.y, rotation.z);
}


void SolidModel::readModelFile(const std::string& fileName)
{
	
	std::ifstream importFile(fileName, std::ios_base::binary);

	if (importFile.is_open())
	{
		std::cout << "Model file succesfully opened..." << std::endl;

		unsigned int solidN;

		importFile.read((char*)& solidN, sizeof(unsigned int));

		for (unsigned int j = 0; j < solidN; j++)
		{
			int nCurrent;

			importFile.read((char*)& nCurrent, sizeof(int));

			for (int i = 0; i < nCurrent; i++)
			{
				triangle3dV temp;

				importFile.read((char*)& temp, sizeof(triangle3dV));

				polyContainer.push_back(temp);
			}
		}
		polyContainer.shrink_to_fit();

		nPoly = getTotalPoly();
		mesh = new triangle3dV[nPoly];
		getTriangleData(mesh);
	}
	else
	{
		std::cout << "Could not read model file " << fileName << std::endl;
	}
}


void SolidModel::logPolyContainerSize()
{
	std::cout << "PolyContainer size: " << polyContainer.size() << std::endl;
}