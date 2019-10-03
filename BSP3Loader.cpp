#include <fstream>
#include <iostream>

#include "BSP3Loader.h"



BSP3Loader::BSP3Loader(std::string f)
{
	fileName	= f;
	scale		= { 1.0, 1.0, 1.0, 1.0 };
}


BSP3Loader::BSP3Loader(std::string f, vect3 s)
{
	fileName	= f;
	scale		= s;
}


BSP3Loader::~BSP3Loader()
{
}


void BSP3Loader::readData()
{
	std::ifstream modelFile(fileName.c_str(), std::ifstream::in | std::ifstream::binary);

	char magic_spell[4] = "";
	char version[4] = "";

	char offset[4] = "";
	char length[4] = "";

	lump direntry[17] = { 0, 0 };

	if (modelFile)
	{
		modelFile.read(magic_spell, 4);
		std::cout << magic_spell << std::endl;

		modelFile.read(version, 4);
		int version_no = *((int*)version);
		std::cout << version_no << std::endl;

		for (int i = 0; i < 17; i++)
		{
			modelFile.read(offset, 4);
			modelFile.read(length, 4);
			direntry[i].offset = *((int*)offset);
			direntry[i].length = *((int*)length);
			std::cout << "Offset " << i << " : " << direntry[i].offset << std::endl;
			std::cout << "Length " << i << " : " << direntry[i].length << std::endl;
		}

		/*
		*	EXTRACTING VERTICES FROM BSP FILE
		*/

		modelFile.seekg(direntry[10].offset, modelFile.beg);
		for (int i = 0; i < (direntry[10].length / 44); i++)
		{
			char 	vertexString[4] = "";

			modelFile.read(vertexString, 4);
			float vert_x = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			float vert_y = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			float vert_z = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			float surface_u = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			float surface_v = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			float lightmap_u = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			float lightmap_v = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			float normal_x = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			float normal_y = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			float normal_z = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			unsigned char vert_r = vertexString[0];
			unsigned char vert_g = vertexString[1];
			unsigned char vert_b = vertexString[2];
			unsigned char vert_a = vertexString[3];

			point3 tempPoint;
			tempPoint.P.x = (double)vert_x * scale.x;
			tempPoint.P.y = (double)vert_y * scale.y;
			tempPoint.P.z = (double)vert_z * scale.z;
			tempPoint.P.w = 1.0;
			tempPoint.colour = getColour(vert_a, vert_r, vert_g, vert_b);

			vertexContainer.push_back(tempPoint);

			vert tempVert;
			tempVert.x		= vert_x * scale.x;
			tempVert.y		= vert_y * scale.y;
			tempVert.z		= vert_z * scale.z;
			tempVert.u		= surface_u;
			tempVert.v		= surface_v;
			tempVert.m		= lightmap_u;
			tempVert.n		= lightmap_v;
			tempVert.nx		= normal_x;
			tempVert.ny		= normal_y;
			tempVert.nz		= normal_z;
			tempVert.r		= vert_r;
			tempVert.g		= vert_g;
			tempVert.b		= vert_b;
			tempVert.a		= vert_a;

			idVertices.push_back(tempVert);
		}
		vertexContainer.shrink_to_fit();
		idVertices.shrink_to_fit();

		/*
		*	EXTRACTING MESHVERTS FROM BSP FILE
		*/

		modelFile.seekg(direntry[11].offset, modelFile.beg);
		for (int i = 0; i < (direntry[11].length / 4); i++)
		{
			char 	vertexString[4] = "";

			modelFile.read(vertexString, 4);
			int vert_index = *((int*)vertexString);

			idMeshVerts.push_back(vert_index);
		}
		idMeshVerts.shrink_to_fit();

		/*
		*	EXTRACTING FACES FROM BSP FILE
		*/

		modelFile.seekg(direntry[13].offset, modelFile.beg);
		for (int i = 0; i < (direntry[13].length / 104); i++)
		{
			char 	vertexString[4] = "";

			modelFile.read(vertexString, 4);
			int texture = *((int*)vertexString);

			modelFile.read(vertexString, 4);
			int effect = *((int*)vertexString);

			modelFile.read(vertexString, 4);
			int type = *((int*)vertexString);

			modelFile.read(vertexString, 4);
			int first_vert = *((int*)vertexString);

			modelFile.read(vertexString, 4);
			int nVert = *((int*)vertexString);

			modelFile.read(vertexString, 4);
			int meshVert = *((int*)vertexString);

			modelFile.read(vertexString, 4);
			int nMeshVert = *((int*)vertexString);

			modelFile.read(vertexString, 4);
			int lm_index = *((int*)vertexString);

			modelFile.read(vertexString, 4);
			int lm_start_x = *((int*)vertexString);

			modelFile.read(vertexString, 4);
			int lm_start_y = *((int*)vertexString);

			modelFile.read(vertexString, 4);
			int lm_size_x = *((int*)vertexString);

			modelFile.read(vertexString, 4);
			int lm_size_y = *((int*)vertexString);

			modelFile.read(vertexString, 4);
			float lm_origin_x = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			float lm_origin_y = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			float lm_origin_z = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			float lm_svect_x = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			float lm_svect_y = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			float lm_svect_z = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			float lm_tvect_x = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			float lm_tvect_y = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			float lm_tvect_z = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			float normal_x = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			float normal_y = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			float normal_z = *((float*)vertexString);

			modelFile.read(vertexString, 4);
			int patch_size_x = *((int*)vertexString);

			modelFile.read(vertexString, 4);
			int patch_size_y = *((int*)vertexString);

			face tempFace;

			tempFace.texture	= texture;
			tempFace.effect		= effect;
			tempFace.type		= type;
			tempFace.firstVert	= first_vert;
			tempFace.nVert		= nVert;
			tempFace.meshVert	= meshVert;
			tempFace.nMeshVert	= nMeshVert;
			tempFace.lightMap	= lm_index;
			tempFace.lmStart	= { lm_start_x, lm_start_y };
			tempFace.lmSize		= { lm_size_x, lm_size_y };
			tempFace.lmOrigin	= { lm_origin_x, lm_origin_y, lm_origin_z, 1.0 };
			tempFace.lmSvect	= { lm_svect_x, lm_svect_y, lm_svect_z, 1.0 };
			tempFace.lmTvect	= { lm_tvect_x, lm_tvect_y, lm_tvect_z, 1.0 };
			tempFace.N			= { normal_x, normal_y, normal_z, 1.0 };
			tempFace.patchSize	= { patch_size_x, patch_size_y };

			idFaces.push_back(tempFace);
		}
		idFaces.shrink_to_fit();
	}



	modelFile.close();
}


int	BSP3Loader::getTotalVert()
{
	return vertexContainer.size();
}


int BSP3Loader::getTotalPoly()
{
	int nFace = idFaces.size();
	int nPoly = 0;
	for (int i = 0; i < nFace; i++)
	{
		if (idFaces[i].type == 1 || idFaces[i].type == 3)
		{
			nPoly += (idFaces[i].nMeshVert / 3);
		}
	}
	return nPoly;
}


void BSP3Loader::getVertexData(point3* v)
{
	int nVert = this->getTotalVert();
	for (int i = 0; i < nVert; i++)
	{
		v[i] = vertexContainer[i];
	}
}


void BSP3Loader::getTriangleData_(triangle3dV* T)
{
	int nFace		= idFaces.size();
	int nPoly		= 0;
	int firstIndex	= 0;
	int aOffset		= 0;
	int bOffset		= 0;
	int cOffset		= 0;
	int aIndex		= 0;
	int bIndex		= 0;
	int cIndex		= 0;
	for (int i = 0; i < nFace; i++)
	{
		if (idFaces[i].type == 1 || idFaces[i].type == 3)
		{
			firstIndex = idFaces[i].firstVert;
			nPoly = idFaces[i].nMeshVert / 3;
			for (int j = 0; j < nPoly; j++)
			{
				aOffset = idMeshVerts[idFaces[i].meshVert + 0 + j * 3];
				bOffset = idMeshVerts[idFaces[i].meshVert + 1 + j * 3];
				cOffset = idMeshVerts[idFaces[i].meshVert + 2 + j * 3];
				aIndex = firstIndex + aOffset;
				bIndex = firstIndex + bOffset;
				cIndex = firstIndex + cOffset;

				triangle3dV temp;

				temp.A = { idVertices[aIndex].x, idVertices[aIndex].y, idVertices[aIndex].z, 1.0 };
				temp.B = { idVertices[bIndex].x, idVertices[bIndex].y, idVertices[bIndex].z, 1.0 };
				temp.C = { idVertices[cIndex].x, idVertices[cIndex].y, idVertices[cIndex].z, 1.0 };

				temp.An = { idVertices[aIndex].nx, idVertices[aIndex].ny, idVertices[aIndex].nz, 1.0 };
				temp.Bn = { idVertices[bIndex].nx, idVertices[bIndex].ny, idVertices[bIndex].nz, 1.0 };
				temp.Cn = { idVertices[cIndex].nx, idVertices[cIndex].ny, idVertices[cIndex].nz, 1.0 };

				temp.At = { idVertices[aIndex].u, idVertices[aIndex].v };
				temp.Bt = { idVertices[bIndex].u, idVertices[bIndex].v };
				temp.Ct = { idVertices[cIndex].u, idVertices[cIndex].v };

				if (idFaces[i].type == 1)
				{
					temp.N = idFaces[i].N;
					temp.colour = getColour(0, 127, 127, 255);
					temp.texture = 3;
				}
				else if (idFaces[i].type == 3)
				{
					temp.N = unitVector(crossProduct(subVectors(temp.A, temp.B), subVectors(temp.C, temp.B)));
					temp.colour = getColour(0, 255, 127, 0);
					temp.texture = 15;
				}
				
				polyContainer.push_back(temp);
			}
		}
	}
	polyContainer.shrink_to_fit();

	for (int i = 0; i < this->getTotalPoly(); i++)
	{
		T[i] = polyContainer[i];
	}
}


vect3 BSP3Loader::getPosition()
{
	return position;
}


void BSP3Loader::constructShadowVolume(vect3)
{

}
