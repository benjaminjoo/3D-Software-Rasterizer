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
	bool			gravitating		= false;
	bool			fired			= false;
	bool			inMotion		= false;	
	bool			visible			= true;
	bool			breakable		= false;
	bool			destroyed		= false;
	bool			vanished		= false;

	boundingBox		BB				= { (0.0, 0.0, 0.0, 1.0), (0.0, 0.0, 0.0, 1.0) };
	double			bbRadius		= 0.25f;

	hit_response	behaviour		= penetrate;
	unsigned int	ticksSinceHit	= 0;
	unsigned int	ticksSinceFired	= 0;

	int				nBounces		= 0;

public:

	SolidBody();
	~SolidBody();

	virtual int				getTotalVert()					= 0;
	virtual int				getTotalPoly()					= 0;
	virtual void			getTriangleData_(triangle3dV*)	= 0;
	virtual void			constructShadowVolume(vect3)	= 0;

	bool assertShadowCasting();

	void setScale(vect3);
	void setPosition(vect3);
	void setRotation(vect3);

	vect3 getScale();
	vect3 getPosition();
	vect3 getRotation();

	void setVelocity(vect3);
	void setAngularVelocity(vect3);

	void updateVelocity(vect3);
	void updateVelocity(double);
	void updateAngularVelocity(vect3);
	bool stoppedMoving();

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
	void setBBRadius(double);

	bool isGravitating();
	void setGravity(bool);

	bool isFired();
	void setFired(bool);

	bool isInMotion();
	void setMotion(bool);

	bool isVisible();
	void setVisibility(bool);

	bool isBreakable();
	void setBreakability(bool);

	bool isDestroyed();
	void destroy();

	bool isVanished();
	void vanish();

	hit_response getBehaviour();
	void setBehaviour(hit_response);

	unsigned int getTicksSinceHit();
	void incrementTicksSinceHit();

	unsigned int getTicksSinceFired();
	void setTicksSinceFired(unsigned int);
	void incrementTicksSinceFired();

	void updateColour(Uint32);

	void incrementBounceCount();
	int getBounceCount();
};

