#include "OpenGLAdapter.h"
#include "Definitions.h"


OpenGLAdapter::OpenGLAdapter(Shapes* B, Shapes* A)
{
	Solids = B;
	Actors = A;

	solidN = Solids->getNEntities();
	actorN = Actors->getNEntities();

	solidPolyCount = new int[Solids->getNEntities()];
	Solids->getPolyCountEntities(solidPolyCount);
	actorPolyCount = new int[Actors->getNEntities()];
	Actors->getPolyCountEntities(actorPolyCount);

	solidMesh = new triangle3dV * [solidN];
	for (unsigned int i = 0; i < solidN; i++)
	{
		solidMesh[i] = new triangle3dV[solidPolyCount[i]];
	}
	Solids->getPolyData_(solidN, solidMesh);

	actorMesh = new triangle3dV * [actorN];
	for (unsigned int i = 0; i < actorN; i++)
	{
		actorMesh[i] = new triangle3dV[actorPolyCount[i]];
	}
	Actors->getPolyData_(actorN, actorMesh);

	triangleMesh	= nullptr;
	Entities		= nullptr;
}


unsigned int OpenGLAdapter::getNVert(model M, unsigned int n)
{
	unsigned int nVert = 0;
	switch (M)
	{
	case solid:
		{
			Entities = Solids;
			nVert = Entities->getPolyCount(n) * 3;
		}
	break;
	case actor:
		{
			Entities = Actors;
			nVert = Entities->getPolyCount(n) * 3;
		}
	break;
	}
	return nVert;
}


void OpenGLAdapter::getVertices(model M, unsigned int n, Vertex* V)
{
	switch (M)
	{
	case solid:
		{
			Entities = Solids;
			triangleMesh = solidMesh;
		}
		break;
	case actor:
		{
			Entities = Actors;
			triangleMesh = actorMesh;
		}
		break;
	default:
		Entities = nullptr;
		break;
	}

	if (Entities != nullptr)
	{
		int nPoly = Entities->getPolyCount(n);
		int nVert = nPoly * 3;
		int vCount = 0;
		for (int i = 0; i < nPoly; i++)
		{
			glm::vec3 tempPosition, tempNormal;
			glm::vec2 tempTxtCoord;

			tempPosition.x = (float)triangleMesh[n][i].A.x;
			tempPosition.y = (float)triangleMesh[n][i].A.y;
			tempPosition.z = (float)triangleMesh[n][i].A.z;

			tempTxtCoord.s = (float)triangleMesh[n][i].At.u;
			tempTxtCoord.t = (float)triangleMesh[n][i].At.v;

			tempNormal.x = (float)triangleMesh[n][i].An.x;
			tempNormal.y = (float)triangleMesh[n][i].An.y;
			tempNormal.z = (float)triangleMesh[n][i].An.z;

			V[vCount].setPosition(tempPosition);
			V[vCount].setTxtCoord(tempTxtCoord);
			V[vCount].setNormal(tempNormal);

			vCount++;

			tempPosition.x = (float)triangleMesh[n][i].B.x;
			tempPosition.y = (float)triangleMesh[n][i].B.y;
			tempPosition.z = (float)triangleMesh[n][i].B.z;

			tempTxtCoord.s = (float)triangleMesh[n][i].Bt.u;
			tempTxtCoord.t = (float)triangleMesh[n][i].Bt.v;

			tempNormal.x = (float)triangleMesh[n][i].Bn.x;
			tempNormal.y = (float)triangleMesh[n][i].Bn.y;
			tempNormal.z = (float)triangleMesh[n][i].Bn.z;

			V[vCount].setPosition(tempPosition);
			V[vCount].setTxtCoord(tempTxtCoord);
			V[vCount].setNormal(tempNormal);

			vCount++;

			tempPosition.x = (float)triangleMesh[n][i].C.x;
			tempPosition.y = (float)triangleMesh[n][i].C.y;
			tempPosition.z = (float)triangleMesh[n][i].C.z;

			tempTxtCoord.s = (float)triangleMesh[n][i].Ct.u;
			tempTxtCoord.t = (float)triangleMesh[n][i].Ct.v;

			tempNormal.x = (float)triangleMesh[n][i].Cn.x;
			tempNormal.y = (float)triangleMesh[n][i].Cn.y;
			tempNormal.z = (float)triangleMesh[n][i].Cn.z;

			V[vCount].setPosition(tempPosition);
			V[vCount].setTxtCoord(tempTxtCoord);
			V[vCount].setNormal(tempNormal);

			vCount++;
		}
	}
}


OpenGLAdapter::~OpenGLAdapter()
{
	solidN = Solids->getNEntities();
	actorN = Actors->getNEntities();

	for (int i = 0; i < solidN; i++)
	{
		delete[] solidMesh[i];
	}
	delete[] solidMesh;

	for (int i = 0; i < actorN; i++)
	{
		delete[] actorMesh[i];
	}
	delete[] actorMesh;
}
