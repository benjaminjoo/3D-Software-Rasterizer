#include "Terrain.h"



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


