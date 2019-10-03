#include "LightSource.h"
#include "Definitions.h"



LightSource::LightSource()
{
	azm = 0.0;
	alt = 0.0;

	intensity = 1.0;
}


LightSource::LightSource(double az, double al)
{
	azm = az;
	alt = al;

	intensity = 1.0;
}


LightSource::LightSource(double az, double al, double i)
{
	azm = az;
	alt = al;

	intensity = i;
}


LightSource::~LightSource()
{
}


double LightSource::getIllumination(vect3 n)
{
	return dotProduct(n, dirVector(azm, alt)) * intensity;
}
