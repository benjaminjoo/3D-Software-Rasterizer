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


void LightSource::setPosition(double az, double al)
{
	azm = az;
	alt = al;
}


double LightSource::getIllumination(vect3 n)
{
	return dotProduct(n, dirVector(azm, alt)) * intensity;
}


vect3 LightSource::getVector()
{
	return dirVector(azm, alt);
	//return { cos(azm / 180.0f * PI), sin(azm / 180.0f * PI), sin(alt / 180.0f * PI), 0.0 };
	//return { cos((azm + 90.0) / 180.0f * PI), sin((azm + 90.0) / 180.0f * PI), sin((alt + 90.0) / 180.0f * PI), 0.0 };
}
