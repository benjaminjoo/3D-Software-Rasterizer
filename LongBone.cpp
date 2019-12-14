#include "LongBone.h"

#include <iostream>

LongBone::LongBone()
{
	std::cout << "LongBone constructor called" << std::endl;
}


LongBone::LongBone(std::string n, handedness s, double l, double w, double d):
	name(n), side(s), length(l), width(w), jointDiameter(d)
{
	std::cout << "LongBone constructor called" << std::endl;

	upperJointPosition = { 0.0f, 0.0f, 0.0f, 1.0f };
	lowerJointPosition = { width, length, 0.0f, 1.0f };

	upperJoint	= std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xffffff00, 1, jointDiameter * 0.5f, 8);
	body		= std::make_shared<SolidPrism>(0.0f, 0.0f, 0.0f, 0xffffff00, 0.05f, length, 0.05f);
	lowerJoint	= std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xffff0000, 1, jointDiameter * 0.5f, 8);
}


LongBone::~LongBone()
{
	std::cout << "LongBone destructor called" << std::endl;
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
	vect3 newPos;

	newPos.x = lowerJointPosition.x - width;
	newPos.y = length * cos(rotation.x + PI);
	newPos.z = length * sin(rotation.x + PI);
	newPos.w = 1.0f;

	return newPos;
}


vect3 LongBone::calculateLowerJointPosition()
{
	vect3 newPos;

	newPos.x = lowerJointPosition.x + width;
	newPos.y = length * cos(rotation.x);
	newPos.z = length * sin(rotation.x);
	newPos.w = 1.0f;

	return newPos;
}


void LongBone::updateSelfFromBelow(handedness s, vect3 p, vect3 rt)
{
	this->setLowerJointPosition(p);
	this->updateRotation(rt);
	this->updateRotation(rotationPerTick);
	vect3 newUpperJointPosition = this->calculateUpperJointPosition();
	setUpperJointPosition(newUpperJointPosition);

	std::cout << name << " - Updated self starting from below" << std::endl;

	this->updateParent();
}


void LongBone::updateSelfFromAbove(vect3 p, vect3 rt)
{
	this->setUpperJointPosition(p);
	this->updateRotation(rt);
	this->updateRotation(rotationPerTick);
	vect3 newLowerJointPosition = this->calculateLowerJointPosition();
	setLowerJointPosition(newLowerJointPosition);

	std::cout << name << " - Updated self starting from above" << std::endl;

	this->updateChild();
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
		parent->updateSelfFromBelow(side, upperJointPosition, rotation);
	}
}


void LongBone::updateFromExtreme()
{
	if (child == nullptr)
	{
		this->updateRotation(rotationPerTick);
		vect3 newUpperJointPosition = this->calculateUpperJointPosition();
		setUpperJointPosition(newUpperJointPosition);

		std::cout << name << " - Updated self starting from extreme" << std::endl;

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


void LongBone::countMesh(int* n)
{
	if (upperJoint != nullptr)
		(*n)++;
	if (body != nullptr)
		(*n)++;
	if (lowerJoint != nullptr)
		(*n)++;

	if (child != nullptr)
		child->countMesh(n);
}


void LongBone::getPolyData(int* i, unsigned int* polyCount, triangle3dV** mesh)
{
	if (upperJoint != nullptr)
	{
		polyCount[*i] = upperJoint->getTotalPoly();
		mesh[*i] = new triangle3dV[polyCount[*i]];
		upperJoint->getTriangleData_(mesh[*i]);
		(*i)++;
	}
	if (body != nullptr)
	{
		polyCount[*i] = body->getTotalPoly();
		mesh[*i] = new triangle3dV[polyCount[*i]];
		body->getTriangleData_(mesh[*i]);
		(*i)++;
	}
	if (lowerJoint != nullptr)
	{
		polyCount[*i] = lowerJoint->getTotalPoly();
		mesh[*i] = new triangle3dV[polyCount[*i]];
		lowerJoint->getTriangleData_(mesh[*i]);
		(*i)++;
	}

	if (child != nullptr)
		child->getPolyData(i, polyCount, mesh);
}


void LongBone::getBonePosition(int* i, vect3* sc, vect3* mv, vect3* rt)
{
	if (upperJoint != nullptr)
	{
		sc[*i] = { 1.0f, 1.0f, 1.0f, 1.0f };
		mv[*i] = upperJointPosition;
		rt[*i] = rotation;
		(*i)++;
	}
	if (body != nullptr)
	{
		sc[*i] = { 1.0f, 1.0f, 1.0f, 1.0f };
		mv[*i] = upperJointPosition;
		rt[*i] = rotation;
		(*i)++;
	}
	if (lowerJoint != nullptr)
	{
		sc[*i] = { 1.0f, 1.0f, 1.0f, 1.0f };
		mv[*i] = lowerJointPosition;
		rt[*i] = rotation;
		(*i)++;
	}

	if (child != nullptr)
		child->getBonePosition(i, sc, mv, rt);
}
