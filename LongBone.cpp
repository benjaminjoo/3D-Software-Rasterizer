#include "LongBone.h"



LongBone::LongBone()
{
}


LongBone::LongBone(double l, double w):
	length(l), width(w)
{
}


LongBone::~LongBone()
{
}


void LongBone::setUpperJointPosition(vect3 p)
{
	upperJointPosition = p;
}


void LongBone::updateUpperJointPosition(vect3 dp)
{
	upperJointPosition += dp;
}


vect3 LongBone::getUpperJointPosition()
{
	return upperJointPosition;
}


void LongBone::setLowerJointPosition(vect3 p)
{
	lowerJointPosition = p;
}


void LongBone::updateLowerJointPosition(vect3 dp)
{
	lowerJointPosition += dp;
}


vect3 LongBone::getLowerJointPosition()
{
	return lowerJointPosition;
}


void LongBone::setRotation(vect3 r)
{
	rotation = r;
}


void LongBone::updateRotation(vect3 dr)
{
	rotation += dr;
}


vect3 LongBone::getRotation()
{
	return rotation;
}


vect3 LongBone::calculateUpperJointPosition()
{

}


vect3 LongBone::calculateLowerJointPosition()
{

}


void LongBone::updateSelfFromBelow(vect3 p, vect3 rt)
{
	this->setLowerJointPosition(p);
	this->updateRotation(rt);
	this->updateRotation(rotationPerTick);
	vect3 newUpperJointPosition = this->calculateUpperJointPosition();
	setUpperJointPosition(newUpperJointPosition);
	this->updateParent();
}


void LongBone::updateSelfFromAbove(vect3 p, vect3 rt)
{
	this->setUpperJointPosition(p);
	this->updateRotation(rt);
	this->updateRotation(rotationPerTick);
	vect3 newLowerJointPosition = this->calculateLowerJointPosition();
	setLowerJointPosition(newLowerJointPosition);
	this->updateParent();
}


void LongBone::updateChild()
{
	if (child != nullptr)
	{
		child->updateSelfFromAbove(lowerJointPosition, rotation);
	}		
}


void LongBone::updateParent()
{
	if (parent != nullptr)
	{
		parent->updateSelfFromBelow(upperJointPosition, rotation);
	}
}


void LongBone::updateFromExtreme()
{
	if (child == nullptr)
	{
		this->updateRotation(rotationPerTick);
		vect3 newUpperJointPosition = this->calculateUpperJointPosition();
		setUpperJointPosition(newUpperJointPosition);
		this->updateParent();
	}
	else
	{
		child->updateFromExtreme();
	}
}


void LongBone::attachChild(LongBone* ch)
{
	if (child == nullptr)
	{
		child = ch;
		child->parent = this;
		child->setUpperJointPosition(this->lowerJointPosition);
	}		
	else
		child->attachChild(ch);
}
