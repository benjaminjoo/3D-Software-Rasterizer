#pragma once

#include "Definitions.h"
#include "Camera.h"
#include "Lamp.h"
#include "Projection.h"

#include <memory>


class SolidBody
{
public:

	vect3			scale = { 1.0f, 1.0f, 1.0f, 1.0f };
	vect3			position = { 0.0f, 0.0f, 0.0f, 1.0f };
	vect3			rotation = { 0.0f, 0.0f, 0.0f, 1.0f };

	vect3			velocity = { 0.0f, 0.0f, 0.0f, 1.0f };
	vect3			angularVelocity = { 0.0f, 0.0f, 0.0f, 1.0f };

	matRT			material;
	Uint32			colour = 255;
	int				texture = 0;
	float			txU = 1.0f;

	int				nPoly = 0;
	triangle3dV* mesh = nullptr;

	std::shared_ptr<Projection> Projector = std::make_shared<Projection>();

protected:

	bool			isDynamic = false;
	bool			castsShadows = false;
	bool			bBoxActive = false;
	bool			gravitating = false;
	bool			fired = false;
	bool			inMotion = false;
	bool			visible = true;
	bool			breakable = false;
	bool			destroyed = false;
	bool			vanished = false;

	boundingBox		BB = { (0.0f, 0.0f, 0.0f, 1.0f), (0.0f, 0.0f, 0.0f, 1.0f) };
	float			bbRadius = 0.25f;

	hit_response	behaviour = hit_response::penetrate;
	unsigned int	ticksSinceHit = 0;
	unsigned int	ticksSinceFired = 0;

	int				nBounces = 0;

public:

	SolidBody();
	~SolidBody();

	virtual int	getTotalVert() = 0;
	virtual int	getTotalPoly() = 0;
	virtual void getTriangleData(triangle3dV*) = 0;

	void setScale(vect3);
	void setPosition(vect3);
	void setRotation(vect3);

	vect3 getScale();
	vect3 getPosition();
	vect3 getRotation();

	void setVelocity(vect3);
	void setAngularVelocity(vect3);

	void updateVelocity(vect3);
	void updateVelocity(float);
	void updateAngularVelocity(vect3);
	bool stoppedMoving();

	vect3 getVelocity();
	vect3 getAngularVelocity();

	void updatePosition();
	void updatePosition(vect3);
	void updateRotation();

	void setTexture(int);
	void setMaterial(matRT);

	bool getBBState();
	void activateBBox();
	void updatebBBox(vect3, vect3);
	boundingBox getBB();

	float getBBRadius();
	void setBBRadius(float);

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

	virtual void explode();
	void updateMesh();
	void scan(std::shared_ptr<Lamp> lamp, bool trans);
	void render(std::shared_ptr<Camera> eye, bool trans, std::shared_ptr<Lamp> spotlight,
		LightSource sun, const projectionStyle& style, float torch, float ill);
};

