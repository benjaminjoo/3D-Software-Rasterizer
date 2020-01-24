#include "Terrain.h"


Terrain::Terrain(std::string fn) : fileName(fn)
{
	std::ifstream dataInput(fileName);

	if (dataInput.is_open())
	{
		std::string inputLine = "";

		std::getline(dataInput, inputLine);
		std::stringstream header_01(inputLine);
		std::string numColText = "";
		header_01 >> numColText >> sizeX;

		std::getline(dataInput, inputLine);
		std::stringstream header_02(inputLine);
		std::string numRowText = "";
		header_02 >> numRowText >> sizeY;

		std::getline(dataInput, inputLine);
		std::stringstream header_03(inputLine);
		std::string cornerXText = "";
		header_03 >> cornerXText >> nwX;

		std::getline(dataInput, inputLine);
		std::stringstream header_04(inputLine);
		std::string cornerYText = "";
		header_04 >> cornerYText >> nwY;

		std::getline(dataInput, inputLine);
		std::stringstream header_05(inputLine);
		std::string cellSizeText = "";
		header_05 >> cellSizeText >> unit;

		std::getline(dataInput, inputLine);
		std::stringstream header_06(inputLine);

		heightMap = new float[sizeX * sizeY];
		pixelGrid = new Uint32[sizeX * sizeY];

		//Reads height values from text file into heightMap, and finds min & max values at the same time

		float hMin = 0.0f;
		float hMax = 0.0f;

		for (int j = 0; j < sizeY; j++)
		{
			std::getline(dataInput, inputLine);
			std::stringstream currentLine(inputLine);
			for (int i = 0; i < sizeX; i++)
			{
				currentLine >> heightMap[j * sizeX + i];
				if (i == 0 && j == 0)
				{
					hMin = heightMap[0];
					hMax = heightMap[0];
				}
				else
				{
					if (heightMap[j * sizeX + i] < hMin)
						hMin = heightMap[j * sizeX + i];
					if (heightMap[j * sizeX + i] > hMax)
						hMax = heightMap[j * sizeX + i];
				}			
			}
		}

		std::cout << "Lowest point: " << hMin << std::endl;
		std::cout << "Lighest point: " << hMax << std::endl;

		calculatePixels(std::max(abs(hMin), abs(hMax)));
		if (pixelGrid != nullptr)
			outputImage("heightmap.ppm");

		dataInput.close();
	}
	else
	{
		std::cout << "Could not read file: " << fileName << std::endl;
	}
}


Terrain::Terrain(std::string fn, float x, float y, Uint32 col) :
	fileName(fn), colour(col), offsetX(x), offsetY(y)
{
	std::ifstream dataInput(fileName);

	if (dataInput.is_open())
	{
		std::string inputLine = "";

		std::getline(dataInput, inputLine);
		std::stringstream header_01(inputLine);
		std::string numColText = "";
		header_01 >> numColText >> sizeX;

		std::getline(dataInput, inputLine);
		std::stringstream header_02(inputLine);
		std::string numRowText = "";
		header_02 >> numRowText >> sizeY;

		std::getline(dataInput, inputLine);
		std::stringstream header_03(inputLine);
		std::string cornerXText = "";
		header_03 >> cornerXText >> nwX;

		std::getline(dataInput, inputLine);
		std::stringstream header_04(inputLine);
		std::string cornerYText = "";
		header_04 >> cornerYText >> nwY;

		std::getline(dataInput, inputLine);
		std::stringstream header_05(inputLine);
		std::string cellSizeText = "";
		header_05 >> cellSizeText >> unit;

		std::getline(dataInput, inputLine);
		std::stringstream header_06(inputLine);

		pGrid = new point3[sizeX * sizeY];

		for (int j = 0; j < sizeY; j++)
		{
			std::getline(dataInput, inputLine);
			std::stringstream currentLine(inputLine);
			for (int i = 0; i < sizeX; i++)
			{
				if (absolutePosition)
				{
					pGrid[j * sizeX + i].P.x = nwX + i * unit + offsetX;
					pGrid[j * sizeX + i].P.y = nwY + j * unit + offsetY;
				}
				else
				{
					pGrid[j * sizeX + i].P.x = i * unit;
					pGrid[j * sizeX + i].P.y = j * unit;
				}
				currentLine >> pGrid[j * sizeX + i].P.z;
				pGrid[j * sizeX + i].P.w = 1.0f;
				pGrid[j * sizeX + i].colour = colour;
			}
		}

		dataInput.close();
	}
	else
	{
		std::cout << "Could not read file: " << fileName << std::endl;
	}
}


Terrain::Terrain(std::string fn, int sx, int sy, float cx, float cy, float u, float sc, float dp, Uint32 col, bool side) :
	fileName(fn), sizeX(sx), sizeY(sy), centreX(cx), centreY(cy), unit(u), scale(sc), depth(dp), colour(col), sidesOn(side)
{
	pixelGrid = new Uint32[sizeX * sizeY];
	for (int i = 0; i < sizeX * sizeY; i++)
		pixelGrid[i] = 0;

	heightMap = new float[sizeX * sizeY];
	for (int i = 0; i < sizeX * sizeY; i++)
		heightMap[i] = 0.0f;

	pGrid = new point3[sizeX * sizeY];

	addTexture(IMG_Load(fileName.c_str()));

	getSample();

	createPointCloud(scale * 25.0f);

	polyCount = (sizeX - 1) * (sizeY - 1) * 2;
	mesh = new triangle3dV[polyCount];
	for (int i = 0; i < polyCount; i++)
	{
		mesh[i].colour = colour;
		mesh[i].texture = 0;
		if (i < polyCount / 2)
		{
			mesh[i].At = { 0.0f, 0.0f };
			mesh[i].Bt = { 0.0f, 1.0f };
			mesh[i].Ct = { 1.0f, 1.0f };
		}
		else
		{
			mesh[i].At = { 0.0f, 0.0f };
			mesh[i].Bt = { 1.0f, 1.0f };
			mesh[i].Ct = { 1.0f, 0.0f };
		}
	}

	createMesh();
}


Terrain::~Terrain()
{
	delete[] pixelGrid;
	delete[] heightMap;
	delete[] pGrid;
}


void Terrain::addTexture(SDL_Surface* T)
{
	if (T == nullptr)
	{
		std::cout << "Image loading failed..." << std::endl;
	}
	else
	{
		SDL_Surface* tempImage = SDL_ConvertSurfaceFormat(T, SDL_PIXELFORMAT_ARGB8888, 0);
		texture.pixels = (Uint32*)tempImage->pixels;
		texture.w = T->w;
		texture.h = T->h;
		std::cout << "Image succesfully loaded (" << texture.w << "x" << texture.h << ")" << std::endl;
	}
}


void Terrain::getSample()
{
	if (texture.pixels != nullptr)
	{
		std::cout << "Sampling image file..." << std::endl;
		float totalW = texture.w * scale;
		float totalH = texture.h * scale;

		float gridCentreX = centreX * scale;
		float gridCentreY = centreY * scale;

		float gridStartX = gridCentreX - (sizeX - 1) * unit * scale * 0.5f;
		float gridStartY = gridCentreY - (sizeY - 1) * unit * scale * 0.5f;

		wrap(gridStartX, totalW);
		wrap(gridStartY, totalH);

		for (int j = 0; j < sizeY; j++)
			for (int i = 0; i < sizeX; i++)
			{
				float currentGridCentreX = gridStartX + i * unit * scale;
				float currentGridCentreY = gridStartY + j * unit * scale;

				//std::cout << i << "x" << j << " cx : " << currentGridCentreX << std::endl;
				//std::cout << i << "x" << j << " cy : " << currentGridCentreY << std::endl;

				int upperLX = static_cast<int>(currentGridCentreX - unit * scale * 0.5f);
				int upperLY = static_cast<int>(currentGridCentreY - unit * scale * 0.5f);
				int lowerRX = static_cast<int>(currentGridCentreX + unit * scale * 0.5f);
				int lowerRY = static_cast<int>(currentGridCentreY + unit * scale * 0.5f);

				int deltaX = abs(lowerRX - upperLX);
				int deltaY = abs(upperLY - lowerRY);

				int rAcc = 0;
				int gAcc = 0;
				int bAcc = 0;

				int sCount = 0;
				for (int q = 0; q < deltaY; q++)
					for (int p = 0; p < deltaX; p++)
					{
						int x = upperLX + p;
						int y = upperLY + q;
						wrap(x, texture.w);
						wrap(y, texture.h);
						int index = y * texture.w + x;
						colour32 temp;
						temp.argb = texture.pixels[index];
						rAcc += temp.c[2];
						gAcc += temp.c[1];
						bAcc += temp.c[0];
						sCount++;
					}
				colour32 result;
				result.argb = 0;
				float height = 0.0f;
				if(sCount > 0)
				{
					int rAverage = static_cast<int>(static_cast<float>(rAcc) / static_cast<float>(sCount));
					int gAverage = static_cast<int>(static_cast<float>(gAcc) / static_cast<float>(sCount));
					int bAverage = static_cast<int>(static_cast<float>(bAcc) / static_cast<float>(sCount));
					result.c[2] = rAverage;
					result.c[1] = gAverage;
					result.c[0] = bAverage;
					height = (rAverage * 0.3f + gAverage * 0.59f + bAverage * 0.11f) / 255.0f;
					//if (gAverage > bAverage)
					//	height = gAverage / 255.0f;
					//else
					//	height = -255.0f + bAverage / 255.0f;
				}
				pixelGrid[j * sizeX + i] = result.argb;
				heightMap[j * sizeX + i] = height;
			}
	}
}


void Terrain::displaySample(std::shared_ptr<Canvas> screen, int s)
{
	int w = screen->getWidth();
	int h = screen->getHeight();
	int marginX = (w - sizeX * s) / 2;
	int marginY = (h - sizeY * s) / 2;
	for (int j = 0; j < sizeY; j++)
		for (int i = 0; i < sizeX; i++)
			for (int q = 0; q < s; q++)
				for (int p = 0; p < s; p++)
				{
					int x = marginX + i * s + p;
					int y = marginY + j * s + q;
					if (x >= 0 && x < w && y >= 0 && y < h)
					{
						//screen->putPixel(x, y, pixelGrid[j * sizeX + i]);
						colour32 p;
						p.c[2] = static_cast<unsigned char>(255.0 * heightMap[j * sizeX + i]);
						p.c[1] = static_cast<unsigned char>(255.0 * heightMap[j * sizeX + i]);
						p.c[0] = static_cast<unsigned char>(255.0 * heightMap[j * sizeX + i]);
						screen->putPixel(x, y, p.argb);
					}					
				}
}


void Terrain::calculatePixels(float span)
{
	if (heightMap != nullptr && span > 0.0f)
	{
		float grade;
		for (int j = 0; j < sizeY; j++)
		{
			for (int i = 0; i < sizeX; i++)
			{
				float zValue = heightMap[j * sizeX + i];
				if (zValue >= 0.0f)
				{
					grade = (span - zValue) / span;
					if (grade <= 0.5f)
						pixelGrid[j * sizeX + i] = getColour(grade * 2.0f, 0.0f, 0.0f);
					else
						pixelGrid[j * sizeX + i] = getColour(1.0, (grade - 0.5f) * 2.0f, (grade - 0.5f) * 2.0f);
				}
				else
				{
					grade = (span - abs(zValue)) / span;
					if (grade <= 0.5f)
						pixelGrid[j * sizeX + i] = getColour(0.0f, 0.0f, grade * 2.0f);
					else
						pixelGrid[j * sizeX + i] = getColour((grade - 0.5f) * 2.0f, (grade - 0.5f) * 2.0f, 1.0f);
				}
			}
		}
	}
}


void Terrain::outputImage(std::string fName)
{
	std::ofstream outputFile(fName);
	if (outputFile.is_open())
	{
		outputFile << "P3" << std::endl;
		outputFile << sizeX << " " << sizeY << std::endl;
		outputFile << "255" << std::endl;

		for (int j = 0; j < sizeX; j++)
		{
			for (int i = 0; i < sizeY; i++)
			{
				colour32 temp;
				temp.argb = pixelGrid[j * sizeX + i];
				outputFile	<< static_cast<int>(temp.c[2]) << " "
							<< static_cast<int>(temp.c[1]) << " "
							<< static_cast<int>(temp.c[0]) << std::endl;
			}
		}

		outputFile.close();
	}
}


void Terrain::createPointCloud(float hg_scale)
{
	for (int j = 0; j < sizeY; j++)
		for (int i = 0; i < sizeX; i++)
		{
			pGrid[j * sizeX + i].P.x = i * unit * scale;
			pGrid[j * sizeX + i].P.y = j * unit * scale;
			pGrid[j * sizeX + i].P.z = heightMap[j * sizeX + i] * hg_scale;
			pGrid[j * sizeX + i].P.w = 1.0f;
			pGrid[j * sizeX + i].colour = pixelGrid[j * sizeX + i];
		}
}


void Terrain::createMesh()
{
	//Surface - A

	for (int j = 0; j < sizeY - 1; j++)
	{
		for (int i = 0; i < sizeX - 1; i++)
		{
			vect3 A = pGrid[j * sizeX + i].P;
			vect3 B = pGrid[(j + 1) * sizeX + i].P;
			vect3 C = pGrid[(j + 1) * sizeX + i + 1].P;
			vect3 N = ((A - B) ^ (C - B)).norm();

			mesh[j * (sizeX - 1) + i].A = A;
			mesh[j * (sizeX - 1) + i].B = B;
			mesh[j * (sizeX - 1) + i].C = C;
			mesh[j * (sizeX - 1) + i].An = N;
			mesh[j * (sizeX - 1) + i].Bn = N;
			mesh[j * (sizeX - 1) + i].Cn = N;
			mesh[j * (sizeX - 1) + i].N = N;
		}
	}

	//Surface - B

	for (int j = 0; j < sizeY - 1; j++)
	{
		for (int i = 0; i < sizeX - 1; i++)
		{
			vect3 A = pGrid[j * sizeX + i].P;
			vect3 B = pGrid[(j + 1) * sizeX + i + 1].P;
			vect3 C = pGrid[j * sizeX + i + 1].P;
			vect3 N = ((B - C) ^ (A - C)).norm();

			mesh[polyCount / 2 + j * (sizeX - 1) + i].A = A;
			mesh[polyCount / 2 + j * (sizeX - 1) + i].B = B;
			mesh[polyCount / 2 + j * (sizeX - 1) + i].C = C;
			mesh[polyCount / 2 + j * (sizeX - 1) + i].An = N;
			mesh[polyCount / 2 + j * (sizeX - 1) + i].Bn = N;
			mesh[polyCount / 2 + j * (sizeX - 1) + i].Cn = N;
			mesh[polyCount / 2 + j * (sizeX - 1) + i].N = N;
		}
	}
}


void Terrain::renderGrid(std::shared_ptr<Camera> eye, std::shared_ptr<Canvas> screen, mat4x4& RM)
{
	for (int i = 0; i < sizeX * sizeY; i++)
		eye->renderPoint(pGrid[i], RM, screen->pixelBuffer, screen->depthBuffer);
}


void Terrain::renderMesh(std::shared_ptr<Camera> eye, mat4x4& rot, mat4x4& mov, LightSource Sun,
	const projectionStyle& visualStyle, float torchIntensity, float maxIllumination)
{
	eye->renderMesh(polyCount, mesh, rot, mov, Sun, visualStyle, torchIntensity, maxIllumination);
}


