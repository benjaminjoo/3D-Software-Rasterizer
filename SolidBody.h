#pragma once

#include "Definitions.h"


class SolidBody
{
protected:

	vect3			scale;
	vect3			position;
	vect3			rotation;

	vect3			velocity;
	vect3			angularVelocity;

	Uint32			colour;
	int				texture;
	double			txU;

	bool			castsShadows;

	bool			bBoxActive;
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

	vect3 getVelocity();
	vect3 getAngularVelocity();

	void updatePosition();
	void updateRotation();

	bool getBBState();
	void activateBBox();
	void updatebBBox(vect3, vect3);
	boundingBox getBB();
};

