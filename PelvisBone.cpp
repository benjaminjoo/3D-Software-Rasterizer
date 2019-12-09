#include "PelvisBone.h"



PelvisBone::PelvisBone()
{
}


PelvisBone::~PelvisBone()
{
}


void PelvisBone::updateRotation(vect3 dr)
{
	rotation += dr;
}


void PelvisBone::attachBoneLeft(LongBone* b)
{
	if(leftLimb == nullptr)
	{
		leftLimb = b;
		leftLimb->parent = this;
		leftLimb->setUpperJointPosition(this->leftLimbPosition);
	}
	else
	{
		leftLimb->attachChild(b);
	}
}


void PelvisBone::attachBoneRight(LongBone* b)
{
	if (rightLimb == nullptr)
	{
		rightLimb = b;
		rightLimb->parent = this;
		rightLimb->setUpperJointPosition(this->rightLimbPosition);
	}
	else
	{
		rightLimb->attachChild(b);
	}
}


void PelvisBone::updateSelfFromBelow(vect3 p, vect3 rt)
{

}


void PelvisBone::updateSelfFromAbove(vect3 p, vect3 rt)
{

}


void PelvisBone::updateLeftLimb()
{

}


void PelvisBone::updateRightLimb()
{

}


void PelvisBone::initiateUpdateLeft()
{
	leftLimb->updateFromExtreme();
}


void PelvisBone::initiateUpdateRight()
{
	rightLimb->updateFromExtreme();
}
