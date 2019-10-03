#include "ShadowVolume.h"
#include "Definitions.h"


ShadowVolume::ShadowVolume()
{
	isActive = false;

	A = { {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0} };
	B = { {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0} };
	C = { {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0} };

	T = { {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0} };
}


ShadowVolume::ShadowVolume(bool p, plane a, plane b, plane c, plane t)
{
	isActive	= p;

	A			= a;
	B			= b;
	C			= c;

	T			= t;
}


ShadowVolume::~ShadowVolume()
{
}


void ShadowVolume::setPlaneA(plane a)
{
	A = a;
}


void ShadowVolume::setPlaneB(plane b)
{
	B = b;
}


void ShadowVolume::setPlaneC(plane c)
{
	C = c;
}


void ShadowVolume::setPlaneT(plane t)
{
	T = t;
}


bool ShadowVolume::checkVert(vect3 P)
{
	//double a = dotProduct(subVectors(A.P, P), A.N);
	//double b = dotProduct(subVectors(B.P, P), B.N);
	//double c = dotProduct(subVectors(C.P, P), C.N);
	//double t = dotProduct(subVectors(T.P, P), T.N);

	double a = dotProduct(subVectors(P, A.P), A.N);
	double b = dotProduct(subVectors(P, B.P), B.N);
	double c = dotProduct(subVectors(P, C.P), C.N);
	double t = dotProduct(subVectors(P, T.P), T.N);
	
	if (a > 0.0 && b > 0.0 && c > 0.0 && t > 0.0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
