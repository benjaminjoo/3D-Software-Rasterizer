#pragma once

#include "Definitions.h"
#include "Bone.h"
#include "LongBone.h"
#include "SolidBody.h"

class PelvisBone: public Bone
{
private:

	vect3 position = { 0.0f, 0.0f, 0.0f, 1.0f };
	vect3 rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	vect3 leftLimbPosition = { -0.25f, 0.0f, 0.0f, 1.0f };
	vect3 rightLimbPosition = { 0.25f, 0.0f, 0.0f, 1.0f };

	vect3 rotationPerTick = { 0.0f, 0.1f, 0.0f, 1.0f };

	LongBone* leftLimb = nullptr;
	LongBone* rightLimb = nullptr;

	void updateRotation(vect3);

	void attachBoneLeft(LongBone*);
	void attachBoneRight(LongBone*);

	void updateSelfFromBelow(vect3, vect3);
	void updateSelfFromAbove(vect3, vect3);
	void updateLeftLimb();
	void updateRightLimb();

	void initiateUpdateLeft();
	void initiateUpdateRight();

public:
	PelvisBone();
	~PelvisBone();
};

