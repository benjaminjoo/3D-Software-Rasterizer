#pragma once

#include "Definitions.h"


class SolidBody
{
protected:

	vect3			scale			= { 1.0f, 1.0f, 1.0f, 1.0f };
	vect3			position		= { 0.0f, 0.0f, 0.0f, 1.0f };
	vect3			rotation		= { 0.0f, 0.0f, 0.0f, 1.0f };

	vect3			velocity		= { 0.0f, 0.0f, 0.0f, 1.0f };
	vect3			angularVelocity = { 0.0f, 0.0f, 0.0f, 1.0f };

	Uint32			colour			= 255;
	int				texture			= 0;
	double			txU				= 1.0f;

	bool			castsShadows	= false;
	bool			bBoxActive		= false;
	bool			inMotion		= false;
	bool			visible			= true;

	boundingBox		BB				= { (0.0, 0.0, 0.0, 1.0), (0.0, 0.0, 0.0, 1.0) };
	double			bbRadius		= 0.25f;

	hit_response	behaviour		= penetrate;

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

	void updateVelocity(vect3);
	void updateAngularVelocity(vect3);

	vect3 getVelocity();
	vect3 getAngularVelocity();

	void updatePosition();	
	void updateRotation();

	void updatePosition(vect3);

	void setTexture(int);

	bool getBBState();
	void activateBBox();
	void updatebBBox(vect3, vect3);
	boundingBox getBB();
	double getBBRadius();

	bool isInMotion();
	void setInMotion();
	void stop();

	bool isVisible();
	void setVisibility(bool);
};

