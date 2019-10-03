#include "SolidBody.h"

#include "Definitions.h"

SolidBody::SolidBody()
{
	scaleX		= 1.0;
	scaleY		= 1.0;
	scaleZ		= 1.0;

	posX		= 0.0;
	posY		= 0.0;
	posZ		= 0.0;

	rotX		= 0.0;
	rotY		= 0.0;
	rotZ		= 0.0;

	position	= { 0.0, 0.0, 0.0, 1.0 };
	velocity	= { 0.0, 0.0, 0.0, 1.0 };

	colour		= 255;
	texture		= 0;
	txU			= 1.0;


}


SolidBody::~SolidBody()
{
}
