#include "SpotLight.h"



SpotLight::SpotLight()
{
	position = { 0.0, 0.0, 0.0, 1.0 };

	intensity = 1.0;
}


SpotLight::SpotLight(vect3 pos, double i)
{
	position = pos;

	intensity = i;
}


SpotLight::~SpotLight()
{
}


void SpotLight::setPosition(double x, double y, double z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}


vect3 SpotLight::getPosition()
{
	return position;
}


double SpotLight::getIntensity()
{
	return intensity;
}
