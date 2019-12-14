#pragma once

#include <memory>

#include "Definitions.h"
#include "Bone.h"
#include "SolidBody.h"
#include "SolidSphere.h"
#include "SolidPrism.h"


class LongBone: public Bone
{
	friend class PelvisBone;

private:

	std::string name			= "Unnamed";
	handedness side				= left;

	double length				= 1.0f;
	double width				= 0.25f;
	double jointDiameter		= 0.1f;

	vect3 upperJointPosition	= { 0.0f, 0.0f, 0.0f, 1.0f };
	vect3 lowerJointPosition	= { 0.25f, 0.0f, -1.0f, 1.0f };
	vect3 rotation				= { -PI * 0.5f, 0.0f, 0.0f, 1.0f };

	vect3 rotationPerTick		= { 0.0f, 0.0f, 0.0f, 1.0f };

	Bone* parent				= nullptr;
	LongBone* child				= nullptr;

	std::shared_ptr<SolidBody> upperJoint		= nullptr;
	std::shared_ptr<SolidBody> body				= nullptr;
	std::shared_ptr<SolidBody> lowerJoint		= nullptr;

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

	void updateSelfFromBelow(handedness, vect3, vect3);
	void updateSelfFromAbove(vect3, vect3);
	void updateChild();
	void updateParent();

	void updateFromExtreme();

	void countMesh(int*);
	void getPolyData(int*, unsigned int*, triangle3dV**);
	void getBonePosition(int*, vect3*, vect3*, vect3*);

public:

	LongBone();
	LongBone(std::string, handedness, double, double, double);
	~LongBone();
	
};

