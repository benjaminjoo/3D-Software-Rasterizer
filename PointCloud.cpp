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


PointCloud::PointCloud(SDL_Surface* T, float rad, int res)
{
	scale = { 1.0f, 1.0f, 1.0f, 1.0f };
	position = { 0.0f, 0.0f, 0.0f, 1.0f };
	rotation = { 0.0f, 0.0f, 0.0f, 1.0f };

	radius = rad;

	texture = 0;
	txU = 1.0f;

	auto image = std::make_shared<txt>();

	SDL_Surface* tempImage = SDL_ConvertSurfaceFormat(T, SDL_PIXELFORMAT_ARGB8888, 0);
	image->w = T->w;
	image->h = T->h;
	image->pixelsH = new Uint32[image->w * image->h];
	image->pixelsH = (Uint32*)tempImage->pixels;


	float angleV, angleH, stepV, stepH;
	stepV = 180.0f / res;
	stepH = 360.0f / static_cast<float>(res * 2);

	int total = 2 * res * (res - 1) + 2;

	point3 temp;

	temp.colour = 0x007f7fff;
	
	temp.P.x = 0.0f;				//South Pole
	temp.P.y = 0.0f;
	temp.P.z = -rad;
	temp.P.w = 1.0f;
	temp.colour = getPixel(image, 0.0f, 0.0f);

	if (temp.colour)
	{
		pointArray.push_back(temp);
	}

	for (int i = 1; i < res; i++)
	{
		for (int j = 0; j < res * 2; j++)
		{
			angleV = (-90.0f + i * stepV) * PI / 180.0f;
			angleH = (j * stepH) * PI / 180.0f;

			temp.P.x = rad * cos(angleV) * cos(angleH);
			temp.P.y = rad * cos(angleV) * sin(angleH);
			temp.P.z = rad * sin(angleV);
			temp.P.w = 1.0f;
			temp.colour = getPixel(image,
				static_cast<float>(j) / static_cast<float>(res * 2),
				static_cast<float>(i) / static_cast<float>(res)
			);

			if (temp.colour)
			{
				pointArray.push_back(temp);
			}
		}
	}

	temp.P.x = 0.0f;				//North Pole
	temp.P.y = 0.0f;
	temp.P.z = rad;
	temp.P.w = 1.0f;
	temp.colour = getPixel(image, 0.0f, 1.0f);

	if (temp.colour)
	{
		pointArray.push_back(temp);
	}
	
	nPoints = pointArray.size();
	nPoly = 0;

	mesh = new triangle3dV[nPoly];
	getTriangleData(mesh);
}


PointCloud::PointCloud(const std::string& fn, float sx, float sy, float sz)
	:
	fileName(fn)
{
	scale = { sx, sy, sz, 1.0f };
	position = { 0.0f, 0.0f, 0.0f, 1.0f };
	rotation = { 0.0f, 0.0f, 0.0f, 1.0f };

	texture = 0;
	txU = 1.0f;

	nPoints = 0;

	std::ifstream edgeInput(fileName);
	if (edgeInput.is_open())
	{
		std::string line = "";
		std::string currentPen = "";
		line3d temp;

		while (std::getline(edgeInput, line))
		{
			std::stringstream inputLine(line);
			if (line.length() >= 4)
			{
				
				std::string command(line.begin(), line.begin() + 4);
				if (command == "PEN ")
				{
					inputLine >> command >> currentPen;
				}
				if (command == "LIN_")
				{
					std::string ax = "", ay = "", az = "", bx = "", by = "", bz = "";
					inputLine >> command >>
						ax >> ay >> az >>
						bx >> by >> bz;

					try
					{
						std::string a_x = (*(ax.end() - 1) == ',') ? std::string(ax.begin(), ax.end() - 2) : ax;
						temp.A.x = std::stof(a_x) * scale.x;
						std::string a_y = (*(ay.end() - 1) == ',') ? std::string(ay.begin(), ay.end() - 2) : ay;
						temp.A.y = std::stof(a_y) * scale.y;
						std::string a_z = (*(az.end() - 1) == ',') ? std::string(az.begin(), az.end() - 2) : az;
						temp.A.z = std::stof(a_z) * scale.z;
						temp.A.w = 1.0f;

						//if (edgeArray.size() == 1)
						//{
						//	float r = sqrt(pow(temp.A.x, 2) + pow(temp.A.y, 2) + pow(temp.A.z, 2));
						//	std::cout << "Radius: " << r << std::endl;
						//}
						
						std::string b_x = (*(bx.end() - 1) == ',') ? std::string(bx.begin(), bx.end() - 2) : bx;
						temp.B.x = std::stof(b_x) * scale.x;
						std::string b_y = (*(by.end() - 1) == ',') ? std::string(by.begin(), by.end() - 2) : by;
						temp.B.y = std::stof(b_y) * scale.y;
						std::string b_z = (*(bz.end() - 1) == ',') ? std::string(bz.begin(), bz.end() - 2) : bz;
						temp.B.z = std::stof(b_z) * scale.z;
						temp.B.w = 1.0f;
					}
					catch (const std::exception& e)
					{
						std::cout << e.what() << std::endl;
					}

					if (currentPen == "pen_borders")
					{
						temp.colour = 0x003f3f3f;
					}
					else if (currentPen == "pen_coastline")
					{
						temp.colour = 0x00ffffff;
					}
					else
					{
						temp.colour = 0x000000ff;
					}

					edgeArray.push_back(temp);
				}
				
			}		
		}
		edgeInput.close();
		nEdge = edgeArray.size();
		std::cout << nEdge << " edges read..." << std::endl;
	}
	else
	{
		std::cout << "Could not read file: " << fileName << std::endl;
	}

	nPoly = 0;
}


PointCloud::~PointCloud()
{
}


Uint32 PointCloud::getPixel(const std::shared_ptr<txt>& img, float x, float y)
{
	int imageWidth = img->w;
	int imageHeight = img->h;

	int x_ = static_cast<int>(static_cast<float>(imageWidth)* x);
	int y_ = static_cast<int>(static_cast<float>(imageHeight)* y);
	if (x_ >= imageWidth)
		x_ = x_ % imageWidth;
	if (y_ >= imageHeight)
		y_ = y_ % imageHeight;

	return img->pixelsH[y_ * imageWidth + x_];
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


void PointCloud::setRadius(float r)
{
	radius = r;
}


void PointCloud::setGrid()
{
	grid			= true;
	this->constructGrid();
}


void PointCloud::setGrid(float vs, float hs, unsigned vres, unsigned hres, Uint32 c)
{
	grid			= true;
	gridVspacing	= vs;
	gridHspacing	= hs;
	gridVresol		= vres;
	gridHresol		= hres;
	gridColour		= c;
	this->constructGrid();
}


void PointCloud::constructGrid()
{
	int numV = static_cast<int>(180.0f / gridVspacing) - 1;
	float stepH = (360.0f / gridHresol) * PI / 180.0f;
	for (auto j = 0; j < numV; j++)
	{
		float angV = (-90.0f + static_cast<float>(j + 1) * gridVspacing) * PI / 180.0f;
		for (auto i = 0; i < gridHresol; i++)
		{
			float angH = i * stepH;
			point3 temp;
			
			temp.P.x = radius * cos(angV) * cos(angH);
			temp.P.y = radius * cos(angV) * sin(angH);
			temp.P.z = radius * sin(angV);
			temp.P.w = 1.0f;

			temp.N = (temp.P - position).norm();

			temp.colour = gridColour;

			gridArray.push_back(temp);
		}
	}

	int numH = static_cast<int>(360.0f / gridHspacing);
	float stepV = 360.0f / gridVresol;
	for (auto i = 0; i < numH; i++)
	{
		float angH = (i * gridHspacing) * PI / 180.0f;
		for (auto j = 0; j < (gridVresol / 2); j++)
		{
			float angV = (-90.0f + static_cast<float>(j + 1)* stepV) * PI / 180.0f;
			point3 temp;

			temp.P.x = radius * cos(angV) * cos(angH);
			temp.P.y = radius * cos(angV) * sin(angH);
			temp.P.z = radius * sin(angV);
			temp.P.w = 1.0f;

			temp.N = (temp.P - position).norm();

			temp.colour = gridColour;

			gridArray.push_back(temp);
		}
	}
}


void PointCloud::addSurfacePoint(float v, float h, float rad, vect3 ct, Uint32 c)
{
	surfacePoint temp(v, h, rad, ct, c);
	surfPointArray.push_back(temp);
}


void PointCloud::addSurfacePoint(float v, float h, float vv, float vh, float rad, vect3 ct, Uint32 c)
{
	surfacePoint temp(v, h, rad, ct, c);
	temp.setVelocity(vv, vh);
	surfPointArray.push_back(temp);
}


void PointCloud::updateSurfacePoints()
{
	for (auto& sp : surfPointArray)
	{
		sp.update();
	}
}


void PointCloud::renderCloud(std::shared_ptr<Camera> eye, std::shared_ptr<Canvas> screen)
{
	for (auto& p : pointArray)
		eye->renderPoint(1, p, screen->pixelBuffer, screen->depthBuffer);
	for (auto& e : edgeArray)
	{
		eye->renderEdge(e);
	}
	for (auto& gp : gridArray)
	{
		eye->renderPoint(1, gp, screen->pixelBuffer, screen->depthBuffer);
	}
	for (auto& sp : surfPointArray)
	{
		eye->renderPoint(3, sp.position(), screen->pixelBuffer, screen->depthBuffer);
	}
}


void PointCloud::renderMesh(std::shared_ptr<Camera> eye, LightSource sun,
	const projectionStyle& visualStyle, float torchIntensity, float maxIllumination)
{
	if (mesh != nullptr)
		eye->renderMesh(nPoly, mesh, nullptr, sun, visualStyle, torchIntensity, maxIllumination);
}
