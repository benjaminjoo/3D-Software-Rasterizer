#include "PointCloud.h"



PointCloud::PointCloud(const std::string& fn, Uint32 col, bool sm) : fileName(fn), colour(col), smooth(sm)
{
	scale = { 1.0f, 1.0f, 1.0f, 1.0f };
	position = { 0.0f, 0.0f, 0.0f, 1.0f };
	rotation = { 0.0f, 0.0f, 0.0f, 1.0f };

	texture = 0;
	txU = 1.0f;

	std::string vertexFileName = fileName + ".vert";
	std::ifstream vertexInput(vertexFileName);
	if (vertexInput.is_open())
	{
		std::string line = "";
		float x, y, z;
		while (std::getline(vertexInput, line))
		{
			std::stringstream inputLine(line);
			inputLine >> x >> y >> z;
			point3 temp;
			temp.P = { x, y, z, 1.0f };
			temp.colour = colour;
			pointArray.push_back(temp);
		}
		vertexInput.close();
		nPoints = pointArray.size();
		std::cout << nPoints << " vertices read..." << std::endl;
	}
	else
	{
		std::cout << "Could not read file: " << fileName << ".vert" << std::endl;
	}

	std::string triangleFileName = fileName + ".tri";
	std::ifstream triangleInput(triangleFileName);
	if (triangleInput.is_open())
	{
		std::string line = "";
		unsigned a_, b_, c_;
		while (std::getline(triangleInput, line))
		{
			std::stringstream inputLine(line);
			inputLine >> a_ >> b_ >> c_;
			triangle3dV temp;

			if (a_ >= 1 && a_ <= nPoints &&
				b_ >= 1 && b_ <= nPoints &&
				c_ >= 0 && c_ <= nPoints)
			{
				temp.A = pointArray[a_ - 1].P;
				temp.B = pointArray[b_ - 1].P;
				temp.C = pointArray[c_ - 1].P;
				temp.N = ((temp.B - temp.A) ^ (temp.C - temp.A)).norm();
				temp.An = temp.N;
				temp.Bn = temp.N;
				temp.Cn = temp.N;
				temp.At = { 0.0f, 0.0f };
				temp.Bt = { 1.0f, 0.0f };
				temp.Ct = { 0.0f, 1.0f };
				temp.colour = colour;
				temp.texture = 0;

				pointArray[a_ - 1].N = pointArray[a_ - 1].N + temp.N;
				pointArray[b_ - 1].N = pointArray[b_ - 1].N + temp.N;
				pointArray[c_ - 1].N = pointArray[c_ - 1].N + temp.N;

				triangleArray.push_back(temp);
			}
		}

		for (auto& p : pointArray)
			p.N = p.N.norm();

		if (smooth)
			smoothSurfaces();

		nPoly = triangleArray.size();
	}
	else
	{
		std::cout << "Could not read file: " << fileName << ".tri" << std::endl;
	}

	//smoothSurfaces();

	std::cout << nPoly << " polygons created..." << std::endl;
	mesh = new triangle3dV[nPoly];
	getTriangleData(mesh);
}


PointCloud::~PointCloud()
{
}


void PointCloud::smoothSurfaces()
{
	for (auto& p : triangleArray)
	{
		vect3 accA, accB, accC;

		accA = accB = accC = { 0.0f, 0.0f, 0.0f, 0.0f };

		int neighbourCountA = 0, neighbourCountB = 0, neighbourCountC = 0;

		//Vertex A

		for (auto& a : triangleArray)
		{
			if ((match(p.A, a.A) || match(p.A, a.B) || match(p.A, a.C)) && dotProduct(p.N, a.N) > 0.5f)
			{
				accA = addVectors(accA, a.N);
				neighbourCountA++;
				if (neighbourCountA >= 5)
					break;
			}
		}

		//Vertex B

		for (auto& b : triangleArray)
		{
			if ((match(p.B, b.A) || match(p.B, b.B) || match(p.B, b.C)) && dotProduct(p.N, b.N) > 0.5f)
			{
				accB = addVectors(accB, b.N);
				neighbourCountB++;
				if (neighbourCountB >= 5)
					break;
			}
		}

		//Vertex C

		for (auto& c : triangleArray)
		{
			if ((match(p.C, c.A) || match(p.C, c.B) || match(p.C, c.C)) && dotProduct(p.N, c.N) > 0.5f)
			{
				accC = addVectors(accC, c.N);
				neighbourCountC++;
				if (neighbourCountC >= 5)
					break;
			}
		}

		p.An = unitVector(accA);
		p.Bn = unitVector(accB);
		p.Cn = unitVector(accC);
	}
}


int	PointCloud::getTotalVert()
{
	return nPoints;
}


int	PointCloud::getTotalPoly()
{
	return nPoly;
}


void PointCloud::invertFaces()
{
	for (unsigned i = 0; i < nPoly; i++)
	{
		mesh[i].N = mesh[i].N.scale(-1.0f);
		mesh[i].An = mesh[i].N;
		mesh[i].Bn = mesh[i].N;
		mesh[i].Cn = mesh[i].N;
	}
}


void PointCloud::getTriangleData(triangle3dV* P)
{
	for (unsigned int i = 0; i < triangleArray.size(); i++)
	{
		P[i] = triangleArray[i];
	}

	Projector->transformMesh(nPoly, P, scale.x, scale.y, scale.z,
		position.x, position.y, position.z,
		rotation.x, rotation.y, rotation.z);
}


void PointCloud::getPoints(std::vector<vertex3>& P)
{
	int id = 0;
	for (auto& p : pointArray)
	{
		vertex3 temp;
		temp.pos.x = p.P.x;
		temp.pos.y = p.P.y;
		temp.pos.z = p.P.z;
		temp.deleted = false;
		temp.selected = false;
		temp.id = id;
		P.push_back(temp);
		id++;
	}
}


void PointCloud::renderCloud(std::shared_ptr<Camera> eye, std::shared_ptr<Canvas> screen)
{
	for (auto& p : pointArray)
		eye->renderPoint(p, screen->pixelBuffer, screen->depthBuffer);
}


void PointCloud::renderMesh(std::shared_ptr<Camera> eye, LightSource sun,
	const projectionStyle& visualStyle, float torchIntensity, float maxIllumination)
{
	if (mesh != nullptr)
		eye->renderMesh(nPoly, mesh, nullptr, sun, visualStyle, torchIntensity, maxIllumination);
}
