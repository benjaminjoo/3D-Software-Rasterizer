#include "Projection.h"



Projection::Projection()
{
}


Projection::~Projection()
{
}


int Projection::GetYMax3(coord2* p)
{
	int yMax = p[0].y;
	for (int i = 1; i < 3; i++)
		if (p[i].y > yMax)
			yMax = p[i].y;
	return yMax;
}


int Projection::GetYMin3(coord2 * p)
{
	int yMin = p[0].y;
	for (int i = 1; i < 3; i++)
		if (p[i].y < yMin)
			yMin = p[i].y;
	return yMin;
}


vect3 Projection::screen2view(coord2 pixel, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio)
{
	int w = screen->getWidth();
	int h = screen->getHeight();

	vect3 vertex;
	double t = 1.0f / pixel.z;

	vertex.x = double(pixel.x) - double(w / 2) * t / (w * 0.475 * h_ratio);
	vertex.y = double(h / 2) - double(pixel.y) * t / (w * 0.475 * v_ratio);
	vertex.z = t;
	vertex.w = 1.0f;

	return vertex;
}


textCoord Projection::getUVCoord(const vect3& startV, const vect3& endV, const textCoord& startC, const textCoord& endC, const vect3& testV)
{
	textCoord testC;

	double a = dotProduct(subVectors(testV, startV), subVectors(endV, startV));
	double b = dotProduct(subVectors(endV, startV), subVectors(endV, startV));
	double d = (b != 0.0) ? (a / b) : (0.0);

	testC.u = startC.u + d * (endC.u - startC.u);
	testC.v = startC.v + d * (endC.v - startC.v);

	return testC;
}


void Projection::fillTriangleFlatShaded(const triangle2dG& t, std::shared_ptr<Canvas> screen)
{
	int w = screen->getWidth();
	int h = screen->getHeight();

	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int dx, dy;
	double xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	double zLimit[2] = { 0, 0 };
	int endIndex;
	int startX, endX;
	double startZ, endZ, zCurrent, illCurrent;

	illCurrent = t.illumA;
	double deltaZ;

	byte r, g, b;
	r = ((byte)(t.h >> 16 & 0xFF));
	g = ((byte)(t.h >> 8 & 0xFF));
	b = ((byte)(t.h & 0xFF));

	Uint32 pixelColour = getColour(0, (byte)(r * illCurrent), (byte)(g * illCurrent), (byte)(b * illCurrent));

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z;
			yy = (double)hg - (double)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * (yy / (double)dy);
			wd = t.a.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				endIndex++;
			}
		}
		//Side B-C:
		if ((t.b.y <= hg && t.c.y > hg) || (t.c.y <= hg && t.b.y > hg))
		{
			dx = t.c.x - t.b.x; dy = t.c.y - t.b.y; dz = t.c.z - t.b.z;
			yy = (double)hg - (double)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (double)dy);
			wd = t.b.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				endIndex++;
			}
		}
		//Side C-A:
		if ((t.c.y <= hg && t.a.y > hg) || (t.a.y <= hg && t.c.y > hg))
		{
			dx = t.a.x - t.c.x; dy = t.a.y - t.c.y; dz = t.a.z - t.c.z;
			yy = (double)hg - (double)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (double)dy);
			wd = t.c.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				endIndex++;
			}
		}
		if (endIndex == 2)
		{
			if (lineEnd[0] <= lineEnd[1])
			{
				startX = lineEnd[0]; startZ = zLimit[0];
				endX = lineEnd[1]; endZ = zLimit[1];
			}
			else
			{
				startX = lineEnd[1]; endX = lineEnd[0];
				startZ = zLimit[1]; endZ = zLimit[0];
			}
			int span = abs(endX - startX + 1);
			deltaZ = (endZ - startZ) / (double)span;
			zCurrent = startZ;
			int hgw = hg * w;
			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					if (1 / zCurrent < screen->depthBuffer[hg * w + i])
					{
						screen->pixelBuffer[hgw + i] = pixelColour;
						screen->depthBuffer[hgw + i] = 1 / zCurrent;
					}
					zCurrent += deltaZ;
				}
			}
		}
	}
}


void Projection::fillTriangleGouraudShaded(const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio)
{
	int w = screen->getWidth();
	int h = screen->getHeight();

	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int dx, dy;
	double dIllum, illum;
	double xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	double zLimit[2] = { 0.0, 0.0 };
	double illLimit[2] = { 0.0, 0.0 };
	int endIndex;
	int startX, endX;
	double startZ, endZ, zCurrent, startIll, endIll, illCurrent;

	double deltaZ, deltaIll;

	byte r, g, b;
	r = ((byte)(t.h >> 16 & 0xFF));
	g = ((byte)(t.h >> 8 & 0xFF));
	b = ((byte)(t.h & 0xFF));

	double corr = 0.0;

	coord2 currentP, startP, endP;
	vect3 startV, endV;

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z; dIllum = t.illumB - t.illumA;
			yy = (double)hg - (double)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * ((yy + corr) / (double)dy);
			illum = t.illumA + dIllum * (yy / (double)dy);
			wd = t.a.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0;
				endIndex++;
			}
		}
		//Side B-C:
		if ((t.b.y <= hg && t.c.y > hg) || (t.c.y <= hg && t.b.y > hg))
		{
			dx = t.c.x - t.b.x; dy = t.c.y - t.b.y; dz = t.c.z - t.b.z; dIllum = t.illumC - t.illumB;
			yy = (double)hg - (double)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * ((yy + corr) / (double)dy);
			illum = t.illumB + dIllum * (yy / (double)dy);
			wd = t.b.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0;
				endIndex++;
			}
		}
		//Side C-A:
		if ((t.c.y <= hg && t.a.y > hg) || (t.a.y <= hg && t.c.y > hg))
		{
			dx = t.a.x - t.c.x; dy = t.a.y - t.c.y; dz = t.a.z - t.c.z; dIllum = t.illumA - t.illumC;
			yy = (double)hg - (double)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * ((yy + corr) / (double)dy);
			illum = t.illumC + dIllum * (yy / (double)dy);
			wd = t.c.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0;
				endIndex++;
			}
		}
		if (endIndex == 2)
		{
			if (lineEnd[0] <= lineEnd[1])
			{
				startX = lineEnd[0];
				endX = lineEnd[1];
				startZ = zLimit[0];
				endZ = zLimit[1];
				startIll = illLimit[0];
				endIll = illLimit[1];
			}
			else
			{
				startX = lineEnd[1];
				endX = lineEnd[0];
				startZ = zLimit[1];
				endZ = zLimit[0];
				startIll = illLimit[1];
				endIll = illLimit[0];
			}
			int span = abs(endX - startX + 1);
			deltaZ = (endZ - startZ) / (double)span;
			deltaIll = ((endIll - startIll) / (double)span) / 100.0;
			zCurrent = startZ;
			illCurrent = startIll / 100.0;

			startP.x = startX;	startP.y = hg;	startP.z = startZ;
			endP.x = endX;		endP.y = hg;	endP.z = endZ;
			startV = screen2view(startP, screen, h_ratio, v_ratio);
			endV = screen2view(endP, screen, h_ratio, v_ratio);

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					if (1 / zCurrent < screen->depthBuffer[hg * w + i])
					{
						screen->pixelBuffer[hg * w + i] = getColour(0, (byte)(r * illCurrent), (byte)(g * illCurrent), (byte)(b * illCurrent));
						screen->depthBuffer[hg * w + i] = 1 / zCurrent;
					}
					zCurrent += deltaZ;
					illCurrent += deltaIll;
				}
			}
		}
	}
}


void Projection::fillTriangleCheckerboard(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio)
{
	int w = screen->getWidth();
	int h = screen->getHeight();

	coord2 pt[3] = { t.a, t.b, t.c };
	int yMin, yMax;

	yMin = GetYMin3(pt);
	yMax = GetYMax3(pt);

	int wd = 0;
	int halfW = std::round(w * 0.5);
	int halfH = std::round(h * 0.5);
	int dx, dy;
	double dIllum, illum;
	double xx, yy, zz, dz;
	int lineEnd[2] = { 0, 0 };
	double zLimit[2] = { 0.0, 0.0 };
	double illLimit[2] = { 0.0, 0.0 };

	vect3 nullVect = { 0.0, 0.0, 0.0, 0.0 };
	vect3 sides[4] = { nullVect, nullVect, nullVect, nullVect };
	vect3 sideL[2] = { nullVect, nullVect };
	vect3 sideR[2] = { nullVect, nullVect };
	textCoord nullCoord = { 0.0, 0.0 };
	textCoord uvSides[4] = { nullCoord, nullCoord, nullCoord, nullCoord };
	textCoord uvSideL[2] = { nullCoord, nullCoord };
	textCoord uvSideR[2] = { nullCoord, nullCoord };

	int endIndex;
	int startX, endX;
	double startZ, endZ, zCurrent, startIll, endIll, illCurrent;
	double invertStartPz, invertEndPz, invertCurrentPz;
	double hCorr = w * 0.475 * h_ratio;
	double vCorr = w * 0.475 * v_ratio;
	double deltaZ, deltaIll;

	coord2 currentP, startP, endP;
	vect3 currentVert, startVert, endVert;
	textCoord startUV, endUV, sampleUV;
	int sampleXold = 0, sampleYold = 0, sampleXnew = 0, sampleYnew = 0;

	for (int hg = yMin; hg < yMax; hg++)
	{
		endIndex = 0;
		//Side A-B:
		if ((t.a.y <= hg && t.b.y > hg) || (t.b.y <= hg && t.a.y > hg))
		{
			dx = t.b.x - t.a.x; dy = t.b.y - t.a.y; dz = t.b.z - t.a.z; dIllum = t.illumB - t.illumA;
			yy = (double)hg - (double)t.a.y; xx = dx * (yy / dy); zz = t.a.z + dz * (yy / (double)dy);
			illum = t.illumA + dIllum * (yy / (double)dy);
			wd = t.a.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0;

				sides[endIndex * 2] = T.A;
				sides[endIndex * 2 + 1] = T.B;
				uvSides[endIndex * 2] = t.At;
				uvSides[endIndex * 2 + 1] = t.Bt;

				endIndex++;
			}
		}
		//Side B-C:
		if ((t.b.y <= hg && t.c.y > hg) || (t.c.y <= hg && t.b.y > hg))
		{
			dx = t.c.x - t.b.x; dy = t.c.y - t.b.y; dz = t.c.z - t.b.z; dIllum = t.illumC - t.illumB;
			yy = (double)hg - (double)t.b.y; xx = dx * (yy / dy); zz = t.b.z + dz * (yy / (double)dy);
			illum = t.illumB + dIllum * (yy / (double)dy);
			wd = t.b.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0;

				sides[endIndex * 2] = T.B;
				sides[endIndex * 2 + 1] = T.C;
				uvSides[endIndex * 2] = t.Bt;
				uvSides[endIndex * 2 + 1] = t.Ct;

				endIndex++;
			}
		}
		//Side C-A:
		if ((t.c.y <= hg && t.a.y > hg) || (t.a.y <= hg && t.c.y > hg))
		{
			dx = t.a.x - t.c.x; dy = t.a.y - t.c.y; dz = t.a.z - t.c.z; dIllum = t.illumA - t.illumC;
			yy = (double)hg - (double)t.c.y; xx = dx * (yy / dy); zz = t.c.z + dz * (yy / (double)dy);
			illum = t.illumC + dIllum * (yy / (double)dy);
			wd = t.c.x + std::round(xx);
			if (endIndex < 2)
			{
				lineEnd[endIndex] = wd;
				zLimit[endIndex] = zz;
				illLimit[endIndex] = illum * 100.0;

				sides[endIndex * 2] = T.C;
				sides[endIndex * 2 + 1] = T.A;
				uvSides[endIndex * 2] = t.Ct;
				uvSides[endIndex * 2 + 1] = t.At;

				endIndex++;
			}
		}
		if (endIndex == 2)
		{
			if (lineEnd[0] <= lineEnd[1])
			{
				startX = lineEnd[0];
				endX = lineEnd[1];
				startZ = zLimit[0];
				endZ = zLimit[1];
				startIll = illLimit[0];
				endIll = illLimit[1];

				sideL[0] = sides[0];
				sideL[1] = sides[1];
				sideR[0] = sides[2];
				sideR[1] = sides[3];

				uvSideL[0] = uvSides[0];
				uvSideL[1] = uvSides[1];
				uvSideR[0] = uvSides[2];
				uvSideR[1] = uvSides[3];
			}
			else
			{
				startX = lineEnd[1];
				endX = lineEnd[0];
				startZ = zLimit[1];
				endZ = zLimit[0];
				startIll = illLimit[1];
				endIll = illLimit[0];

				sideL[0] = sides[2];
				sideL[1] = sides[3];
				sideR[0] = sides[0];
				sideR[1] = sides[1];

				uvSideL[0] = uvSides[2];
				uvSideL[1] = uvSides[3];
				uvSideR[0] = uvSides[0];
				uvSideR[1] = uvSides[1];
			}
			int span = abs(endX - startX + 1);
			deltaZ = (endZ - startZ) / (double)span;
			deltaIll = ((endIll - startIll) / (double)span) / 100.0;
			zCurrent = startZ;
			illCurrent = startIll / 100.0;

			startP.x = startX;	startP.y = hg;	startP.z = startZ;
			endP.x = endX;		endP.y = hg;	endP.z = endZ;

			invertStartPz = 1 / startP.z;
			startVert.x = (startP.x - double(halfW)) * invertStartPz / hCorr;
			startVert.y = (double(halfH) - startP.y) * invertStartPz / vCorr;
			startVert.z = invertStartPz;

			invertEndPz = 1 / endP.z;
			endVert.x = (endP.x - double(halfW)) * invertEndPz / hCorr;
			endVert.y = (double(halfH) - endP.y) * invertEndPz / vCorr;
			endVert.z = invertEndPz;


			startUV = getUVCoord(sideL[0], sideL[1], uvSideL[0], uvSideL[1], startVert);
			endUV = getUVCoord(sideR[0], sideR[1], uvSideR[0], uvSideR[1], endVert);


			currentP.y = hg;

			for (int i = startX; i < endX + 1; i++)
			{
				if ((i >= 0 && i < w) && (hg >= 0 && hg < h))
				{
					if (1 / zCurrent < screen->depthBuffer[hg * w + i])
					{
						currentP.x = i;
						currentP.z = zCurrent;

						invertCurrentPz = 1 / currentP.z;
						currentVert.x = (currentP.x - double(halfW)) * invertCurrentPz / hCorr;
						currentVert.y = (double(halfH) - currentP.y) * invertCurrentPz / vCorr;
						currentVert.z = invertCurrentPz;

						sampleUV = getUVCoord(startVert, endVert, startUV, endUV, currentVert);

						sampleXnew = (int)(64.0 * sampleUV.u);
						sampleYnew = (int)(64.0 * sampleUV.v);

						Uint32 blue = getColour(0, 0, 0, (byte)(255 * illCurrent));
						Uint32 white = getColour(0, (byte)(255 * illCurrent), (byte)(255 * illCurrent), (byte)(255 * illCurrent));

						if (int(sampleXnew / 16) % 2)
						{
							if (int(sampleYnew / 16) % 2)
							{
								screen->pixelBuffer[hg * w + i] = white;
							}
							else
							{
								screen->pixelBuffer[hg * w + i] = blue;
							}
						}
						else
						{
							if (int(sampleYnew / 16) % 2)
							{
								screen->pixelBuffer[hg * w + i] = blue;
							}
							else
							{
								screen->pixelBuffer[hg * w + i] = white;
							}
						}
						screen->depthBuffer[hg * w + i] = 1 / zCurrent;
					}
					zCurrent += deltaZ;
					illCurrent += deltaIll;
				}
			}
		}
	}
}
