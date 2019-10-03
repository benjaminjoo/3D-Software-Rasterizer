#define _CRT_SECURE_NO_DEPRECATE

#include "RayTracer.h"
#include "Camera.h"
#include "Canvas.h"
#include "stdio.h"


RayTracer::RayTracer(int w, int h, Camera* P, Canvas* S, Shapes* B)
{
	width	= w;
	height	= h;

	Player	= P;
	Screen	= S;

	Solids	= B;
	solidN = Solids->getNEntities();
	solidPolyCount = new int[Solids->getNEntities()];
	Solids->getPolyCountEntities(solidPolyCount);

	triangleMesh = new triangle3dV * [solidN];
	for (unsigned int i = 0; i < solidN; i++)
	{
		triangleMesh[i] = new triangle3dV[solidPolyCount[i]];
	}
	Solids->getPolyData_(solidN, triangleMesh);


	field	= new vect3[width * height];

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			vect3 temp;
			
			//temp.x = -1.0f + ((1.0f / (width * 0.5)) * 0.5) + i * (1.0f / (width * 0.5));
			//temp.y = 0.5f - ((1.0f / (width * 0.5)) * 0.5) - j * (1.0f / (width * 0.5));
			//temp.z = 1.0f;
			//temp.w = 1.0f;

			temp.x = -449.5f + (double)i * 1.0f;
			temp.y = 224.5f - (double)j * 1.0f;
			temp.z = 450.0f;
			temp.w = 1.0f;
			
			field[j * width + i] = unitVector(temp);
			//printf("%.4f\t%.4f\t%.4f\n", temp.x, temp.y, temp.z);
		}
	}
}


RayTracer::~RayTracer()
{
	delete[] field;
}


bool RayTracer::checkIntersection(int rayNo, Uint32* pixelColour)
{
	vect3 ray = field[rayNo];
	std::vector<zColour> pixelList;
	zColour tempPixel;
	bool hit = false;
	transform3d T = Player->getTransformation();
	for (int b = 0; b < solidN; b++)
	{
		for (int p = 0; p < solidPolyCount[b]; p++)
		{
			triangle3dV P = Player->world2viewT(T, triangleMesh[b][p]);

			double dist2plane = dotProduct(P.A, P.N);
			double s = dist2plane / dotProduct(ray, P.N);
			double t = dotProduct(ray, P.A);

			if (dist2plane < 0.0f && t > 0.0f)
			{

				vect3 intersection = scale(s, ray);
				double distance = sqrt(intersection.x * intersection.x +
					intersection.y * intersection.y + intersection.z * intersection.z);

				double sA = dotProduct(subVectors(P.A, intersection), subVectors(P.A, P.C));
				double sB = dotProduct(subVectors(P.B, intersection), subVectors(P.B, P.A));
				double sC = dotProduct(subVectors(P.C, intersection), subVectors(P.C, P.B));

				if (sA > 0.0 && sB > 0.0 && sC > 0.0)
				{
					hit = true;
					tempPixel.z = distance;
					tempPixel.colour = P.colour;

					pixelList.push_back(tempPixel);
				}
			}
		}
	}
	pixelList.shrink_to_fit();
	if (hit)
	{
		int nHits = pixelList.size();
		double zMin = pixelList[0].z;
		*pixelColour = pixelList[0].colour;
		for (int i = 0; i < nHits; i++)
		{
			if (pixelList[i].z < zMin)
			{
				*pixelColour = pixelList[i].colour;
				//pixelColour = &(pixelList[i].colour);
				//byte r = pixelList[i].colour >> 16 & 255;
				//byte g = pixelList[i].colour >> 8 & 255;
				//byte b = pixelList[i].colour & 255;
				//printf("%d %d %d\n", r, g, b);

				zMin = pixelList[i].z;
			}
		}
	}
	return hit;
}


void RayTracer::calculatePixel()
{
	
	//Uint32 temp = 0;
	//this->checkIntersection(202500, &temp);
	//byte r = temp >> 16 & 255;
	//byte g = temp >> 8 & 255;
	//byte b = temp >> 255;
	//printf("%d %d %d\n", r, g, b);
	
	
	FILE* f = fopen("screenshot_raytraced.ppm", "w");
	if (f == NULL)
	{
		printf("Could not write to file...\n");
	}
	else
	{
		fprintf(f, "P3\n");
		fprintf(f, "%d %d\n", SCREEN_WIDTH, SCREEN_HEIGHT);
		fprintf(f, "255\n");

		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				Uint32 temp = 0;
				this->checkIntersection((j * width + i), &temp);

				byte r = temp >> 16 & 255;
				byte g = temp >> 8 & 255;
				byte b = temp & 255;
				fprintf(f, "%d %d %d\n", r, g, b);
				temp = 0;
			}
		}

		fclose(f);
	}
	
}