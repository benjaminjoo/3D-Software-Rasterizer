#pragma once

#include <memory>

#include "Definitions.h"
#include "Bone.h"
#include "LongBone.h"
#include "SolidBody.h"


class PelvisBone: public Bone
{
private:

	std::string name			= "Unnamed";

	double width				= 0.5f;
	double height				= 0.3f;
	double jointDiameter		= 0.1f;

	vect3 upperJointPosition	= { 0.0f, 0.0f, 0.0f, 1.0f };
	vect3 leftJointPosition		= { -0.25f, 0.0f, 0.0f, 1.0f };
	vect3 rightJointPosition	= { 0.25f, 0.0f, 0.0f, 1.0f };

	vect3 rotation				= { 0.0f, 0.0f, 0.0f, 1.0f };

	vect3 rotationPerTick		= { 0.0f, 0.0f, 0.0f, 1.0f };

	LongBone* leftLimb			= nullptr;
	LongBone* rightLimb			= nullptr;

	std::shared_ptr<SolidBody> upperJoint		= nullptr;
	std::shared_ptr<SolidBody> leftJoint		= nullptr;
	std::shared_ptr<SolidBody> rightJoint		= nullptr;
	std::shared_ptr<SolidBody> body				= nullptr;

	void setUpperJointPosition(vect3);
	void setLeftJointPosition(vect3);
	void setRightJointPosition(vect3);

	void updateRotation(vect3);

	vect3 calculateUpperJointPosition();
	vect3 calculateLeftJointPosition();
	vect3 calculateRightJointPosition();

	void updateSelfFromBelow(handedness, vect3, vect3);
	void updateSelfFromAbove(vect3, vect3);
	void updateLeftLimb();
	void updateRightLimb();

public:
	PelvisBone();
	PelvisBone(std::string, double, double, double);
	~PelvisBone();

	int getNMesh();
	void getPoly(unsigned int*, triangle3dV**);
	void getBonePosition(vect3*, vect3*, vect3*);

	void attachBoneLeft(LongBone*);
	void attachBoneRight(LongBone*);

	void initiateUpdateLeft();
	void initiateUpdateRight();
};

