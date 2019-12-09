#pragma once

#include "Definitions.h"

class LongBone;
class PelvisBone;

class Bone
{
	friend class LongBone;
	friend class PelvisBone;

private:

	Bone* parent = nullptr;

	virtual void updateSelfFromBelow(vect3, vect3) = 0;
	virtual void updateSelfFromAbove(vect3, vect3) = 0;

public:
	Bone();
	~Bone();
};

