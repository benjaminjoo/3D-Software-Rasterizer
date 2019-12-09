#pragma once

#include "Definitions.h"
#include "Bone.h"
#include "SolidBody.h"

class LongBone: public Bone
{
	friend class PelvisBone;

private:

	vect3 upperJointPosition	= { 0.0f, 0.0f, 0.0f, 1.0f };
	vect3 lowerJointPosition	= { 0.25f, 0.0f, -1.0f, 1.0f };
	vect3 rotation				= { 0.0f, 0.0f, 0.0f, 1.0f };

	vect3 rotationPerTick		= { 0.0f, 0.1f, 0.0f, 1.0f };

	double length				= 1.0f;
	double width				= 0.25f;

	Bone* parent				= nullptr;
	LongBone* child				= nullptr;

	SolidBody* model			= nullptr;

	void setUpperJointPosition(vect3);
	void updateUpperJointPosition(vect3);
	vect3 getUpperJointPosition();

	void setLowerJointPosition(vect3);
	void updateLowerJointPosition(vect3);
	vect3 getLowerJointPosition();

	void setRotation(vect3);
	void updateRotation(vect3);
	vect3 getRotation();

	void attachChild(LongBone*);

	vect3 calculateUpperJointPosition();
	vect3 calculateLowerJointPosition();

	void updateSelfFromBelow(vect3, vect3);
	void updateSelfFromAbove(vect3, vect3);
	void updateChild();
	void updateParent();

	void updateFromExtreme();

public:

	LongBone();
	LongBone(double, double);
	~LongBone();
	
};

