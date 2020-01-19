#include "LightSource.h"
#include "Definitions.h"



LightSource::LightSource()
{
	azm = 0.0f;
	alt = 0.0f;

	direction.x = cos(alt * PI / 180.0f) * cos(azm * PI / 180.0f);
	direction.y = cos(alt * PI / 180.0f) * sin(azm * PI / 180.0f);
	direction.z = sin(alt * PI / 180.0f);
	direction.w = 1.0f;

	intensity = 0.0f;
}


LightSource::LightSource(double az, double al)
{
	azm = az;
	alt = al;

	direction.x = cos(alt * PI / 180.0f) * cos(azm * PI / 180.0f);
	direction.y = cos(alt * PI / 180.0f) * sin(azm * PI / 180.0f);
	direction.z = sin(alt * PI / 180.0f);
	direction.w = 0.0f;

	intensity = 1.0f;
}


LightSource::LightSource(double az, double al, double i)
{
	azm = az;
	alt = al;

	direction.x = cos(alt * PI / 180.0f) * cos(azm * PI / 180.0f);
	direction.y = cos(alt * PI / 180.0f) * sin(azm * PI / 180.0f);
	direction.z = sin(alt * PI / 180.0f);
	direction.w = 0.0f;

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


void LightSource::update()
{
	azm += deltaAzm;
	alt += deltaAlt;

	direction.x = cos(alt * PI / 180.0f) * cos(azm * PI / 180.0f);
	direction.y = cos(alt * PI / 180.0f) * sin(azm * PI / 180.0f);
	direction.z = sin(alt * PI / 180.0f);
	direction.w = 0.0f;
}


double LightSource::getIllumination(vect3& N)
{
	vect3 L = direction;
	return (N * L) * intensity;
}


double LightSource::getBlinnSpecular(vect3 Normal, vect3 View, double shine)
{
	vect3 Light = direction;
	double cosAngIncidence = Normal * Light;

	if (cosAngIncidence < 0.0f)
		cosAngIncidence = 0.0f;
	if (cosAngIncidence > 1.0f)
		cosAngIncidence = 1.0f;

	vect3 Half = (Light + View).norm();
	double blinnTerm = Normal * Half;

	if (blinnTerm < 0.0f)
		blinnTerm = 0.0f;
	if (blinnTerm > 1.0f)
		blinnTerm = 1.0f;

	blinnTerm = cosAngIncidence != 0.0f ? blinnTerm : 0.0f;
	blinnTerm = pow(blinnTerm, shine);

	return blinnTerm;
}


vect3 LightSource::getVector()
{
	return dirVector(azm, alt);
}
