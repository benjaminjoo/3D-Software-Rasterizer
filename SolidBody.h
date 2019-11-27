#pragma once

#include "Definitions.h"


class SolidBody
{
protected:

	vect3			scale			= { 1.0, 1.0, 1.0, 1.0 };
	vect3			position		= { 0.0, 0.0, 0.0, 1.0 };
	vect3			rotation		= { 0.0, 0.0, 0.0, 1.0 };

	vect3			velocity		= { 0.0, 0.0, 0.0, 1.0 };
	vect3			angularVelocity = { 0.0, 0.0, 0.0, 1.0 };

	Uint32			colour			= 255;
	int				texture			= 0;
	double			txU				= 1.0f;

	bool			castsShadows	= false;

	bool			bBoxActive		= false;
	boundingBox		BB;

public:

	SolidBody();
	~SolidBody();

	virtual int				getTotalVert()					= 0;
	virtual int				getTotalPoly()					= 0;
	virtual void			getTriangleData_(triangle3dV*)	= 0;
	virtual vect3			getPosition()					= 0;
	virtual void			constructShadowVolume(vect3)	= 0;

	bool assertShadowCasting();

	void setScale(vect3);
	void setPosition(vect3);
	void setRotation(vect3);

	void setVelocity(vect3);
	void setAngularVelocity(vect3);

	void setTexture(int);

	vect3 getVelocity();
	vect3 getAngularVelocity();

	void updatePosition();
	void updateRotation();

	bool getBBState();
	void activateBBox();
	void updatebBBox(vect3, vect3);
	boundingBox getBB();
};

