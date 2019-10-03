#include "SolidLathe.h"



SolidLathe::SolidLathe()
{
	scale		= { 1.0, 1.0, 1.0, 1.0 };
	position	= { 0.0, 0.0, 0.0, 1.0 };
	rotation	= { 0.0, 0.0, 0.0, 1.0 };

	colour		= 255;
	texture		= 0;

	nProfile	= 2;
	*polyLine	= { (0.0, 0.0, 1), (0.0, 2.0, 1) };

	offset		= 1.0;

	aStart		= 0.0;
	aEnd		= 360.0;

	resolH		= 24;
	resolV		= 24;
}


SolidLathe::SolidLathe(double sx, double sy, double sz, double px, double py, double pz, double rx, double ry, double rz,
	Uint32 col, int t, int nP, polyNode* pLine, double off, double aS, double aE, int rH, int rV)
{
	scale		= { sx, sy, sz, 1.0 };
	position	= { px, py, pz, 1.0 };
	rotation	= { rx, ry, rz, 1.0 };

	colour		= col;
	texture		= t;

	nProfile	= nP;
	polyLine	= pLine;

	offset		= off;

	aStart		= aS;
	aEnd		= aE;

	resolH		= rH;
	resolV		= rV;
}


SolidLathe::~SolidLathe()
{
}


int	SolidLathe::numberOfStepsH()
{
	return abs(int(double(resolH) / 360.0 * (aEnd - aStart)));
}
