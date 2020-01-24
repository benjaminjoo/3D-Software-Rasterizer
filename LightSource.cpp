#include "LightSource.h"
#include "Definitions.h"



LightSource::LightSource()
{
	azm = 0.0f;
	alt = 0.0f;

	direction.x = (float)cos(alt * PI / 180.0f) * (float)cos(azm * PI / 180.0f);
	direction.y = (float)cos(alt * PI / 180.0f) * (float)sin(azm * PI / 180.0f);
	direction.z = (float)sin(alt * PI / 180.0f);
	direction.w = 1.0f;

	intensity = 0.0f;
}


LightSource::LightSource(float az, float al)
{
	azm = az;
	alt = al;

	direction.x = (float)cos(alt * PI / 180.0f) * (float)cos(azm * PI / 180.0f);
	direction.y = (float)cos(alt * PI / 180.0f) * (float)sin(azm * PI / 180.0f);
	direction.z = (float)sin(alt * PI / 180.0f);
	direction.w = 0.0f;

	intensity = 1.0f;
}


LightSource::LightSource(float az, float al, float i)
{
	azm = az;
	alt = al;

	direction.x = (float)cos(alt * PI / 180.0f) * (float)cos(azm * PI / 180.0f);
	direction.y = (float)cos(alt * PI / 180.0f) * (float)sin(azm * PI / 180.0f);
	direction.z = (float)sin(alt * PI / 180.0f);
	direction.w = 0.0f;

	intensity = i;
}


LightSource::~LightSource()
{
}


void LightSource::setPosition(float az, float al)
{
	azm = az;
	alt = al;
}


void LightSource::update()
{
	azm += deltaAzm;
	alt += deltaAlt;

	direction.x = (float)cos(alt * PI / 180.0f) * (float)cos(azm * PI / 180.0f);
	direction.y = (float)cos(alt * PI / 180.0f) * (float)sin(azm * PI / 180.0f);
	direction.z = (float)sin(alt * PI / 180.0f);
	direction.w = 0.0f;

	direction.norm();
}


float LightSource::getIllumination(vect3& N)
{
	vect3 L = direction;
	return (N * L) * intensity;
}


float LightSource::getBlinnSpecular(vect3 Normal, vect3 View, float shine)
{
	vect3 Light = direction;
	float cosAngIncidence = Normal * Light;

	if (cosAngIncidence < 0.0f)
		cosAngIncidence = 0.0f;
	if (cosAngIncidence > 1.0f)
		cosAngIncidence = 1.0f;

	vect3 Half = (Light + View).norm();
	float blinnTerm = Normal * Half;

	if (blinnTerm < 0.0f)
		blinnTerm = 0.0f;
	if (blinnTerm > 1.0f)
		blinnTerm = 1.0f;

	blinnTerm = cosAngIncidence != 0.0f ? blinnTerm : 0.0f;
	blinnTerm = (float)pow(blinnTerm, shine);

	return blinnTerm;
}


vect3 LightSource::getVector()
{
	return dirVector(azm, alt);
}
