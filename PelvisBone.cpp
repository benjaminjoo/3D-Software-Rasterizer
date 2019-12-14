#include "PelvisBone.h"

#include <iostream>


PelvisBone::PelvisBone()
{
	std::cout << "PelvisBone constructor called" << std::endl;
}


PelvisBone::PelvisBone(std::string n, double w, double h, double d):
	name(n), width(w), height(h), jointDiameter(d)
{
	std::cout << "PelvisBone constructor called" << std::endl;

	upperJointPosition = { 0.0f, 0.0f, height, 1.0f };
	leftJointPosition = { -width * 0.5F, 0.0f, 0.0f, 1.0f };
	rightJointPosition = { width * 0.5F, 0.0f, 0.0f, 1.0f };

	upperJoint	= std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xff0000ff, 1, jointDiameter * 0.5f, 8);
	leftJoint	= std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xff0000ff, 1, jointDiameter * 0.5f, 8);
	rightJoint	= std::make_shared<SolidSphere>(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0xff0000ff, 1, jointDiameter * 0.5f, 8);
}


PelvisBone::~PelvisBone()
{
	std::cout << "PelvisBone destructor called" << std::endl;
}


int PelvisBone::getNMesh()
{
	int n = 0;

	if (upperJoint != nullptr)
		n++;
	if (leftJoint != nullptr)
		n++;
	if (rightJoint != nullptr)
		n++;
	if (body != nullptr)
		n++;

	if (leftLimb != nullptr)
		leftLimb->countMesh(&n);

	if (rightLimb != nullptr)
		rightLimb->countMesh(&n);

	return n;
}


void PelvisBone::getPoly(unsigned int* polyCount, triangle3dV** mesh)
{
	int i = 0;

	if (upperJoint != nullptr)
	{
		polyCount[i] = upperJoint->getTotalPoly();
		mesh[i] = new triangle3dV[polyCount[i]];
		upperJoint->getTriangleData_(mesh[i]);
		i++;
	}
	if (leftJoint != nullptr)
	{
		polyCount[i] = leftJoint->getTotalPoly();
		mesh[i] = new triangle3dV[polyCount[i]];
		leftJoint->getTriangleData_(mesh[i]);
		i++;
	}
	if (rightJoint != nullptr)
	{
		polyCount[i] = rightJoint->getTotalPoly();
		mesh[i] = new triangle3dV[polyCount[i]];
		rightJoint->getTriangleData_(mesh[i]);
		i++;
	}

	if (leftLimb != nullptr)
		leftLimb->getPolyData(&i, polyCount, mesh);
	
	if(rightLimb != nullptr)
		rightLimb->getPolyData(&i, polyCount, mesh);
}


void PelvisBone::getBonePosition(vect3* sc, vect3* mv, vect3* rt)
{
	int i = 0;

	if (upperJoint != nullptr)
	{
		sc[i] = { 1.0f, 1.0f, 1.0f, 1.0f };
		mv[i] = upperJointPosition;
		rt[i] = rotation;
		i++;
	}
	if (leftJoint != nullptr)
	{
		sc[i] = { 1.0f, 1.0f, 1.0f, 1.0f };
		mv[i] = leftJointPosition;
		rt[i] = rotation;
		i++;
	}
	if (rightJoint != nullptr)
	{
		sc[i] = { 1.0f, 1.0f, 1.0f, 1.0f };
		mv[i] = rightJointPosition;
		rt[i] = rotation;
		i++;
	}

	if (leftLimb != nullptr)
		leftLimb->getBonePosition(&i, sc, mv, rt);

	if (rightLimb != nullptr)
		rightLimb->getBonePosition(&i, sc, mv, rt);
}


void PelvisBone::setUpperJointPosition(vect3 p)
{
	upperJointPosition = p;
}


void PelvisBone::setLeftJointPosition(vect3 p)
{
	leftJointPosition = p;
}


void PelvisBone::setRightJointPosition(vect3 p)
{
	rightJointPosition = p;
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
		leftLimb->setUpperJointPosition(this->leftJointPosition);
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
		rightLimb->setUpperJointPosition(this->rightJointPosition);
	}
	else
	{
		rightLimb->attachChild(b);
	}
}


vect3 PelvisBone::calculateUpperJointPosition()
{
	vect3 newPos;

	newPos.x = 0.0f;
	newPos.y = rightJointPosition.y + cos(rotation.x + PI);
	newPos.z = rightJointPosition.z + sin(rotation.x + PI);
	newPos.w = 1.0f;

	return newPos;
}


vect3 PelvisBone::calculateLeftJointPosition()
{
	vect3 newPos;

	newPos.x = -width * 0.5f;
	newPos.y = rightJointPosition.y;
	newPos.z = rightJointPosition.z;
	newPos.w = 1.0f;

	return newPos;
}


vect3 PelvisBone::calculateRightJointPosition()
{
	vect3 newPos;

	newPos.x = width * 0.5f;
	newPos.y = leftJointPosition.y;
	newPos.z = leftJointPosition.z;
	newPos.w = 1.0f;

	return newPos;
}


void PelvisBone::updateSelfFromBelow(handedness s, vect3 p, vect3 rt)
{
	if (s == left)
	{
		this->setLeftJointPosition(p);

		this->updateRotation(rt);
		this->updateRotation(rotationPerTick);

		this->calculateRightJointPosition();
		this->calculateUpperJointPosition();

		std::cout << name << " - Left Joint updated starting from below" << std::endl;

		this->updateRightLimb();
	}
	else if (s == right)
	{
		this->setRightJointPosition(p);

		this->updateRotation(rt);
		this->updateRotation(rotationPerTick);

		this->calculateLeftJointPosition();
		this->calculateUpperJointPosition();

		std::cout << name << " - Right Joint updated starting from below" << std::endl;

		this->updateLeftLimb();
	}	
}


void PelvisBone::updateSelfFromAbove(vect3 p, vect3 rt)
{
	this->setUpperJointPosition(p);

	this->updateRotation(rt);
	this->updateRotation(rotationPerTick);
}


void PelvisBone::updateLeftLimb()
{
	leftLimb->updateSelfFromAbove(leftJointPosition, rotation);
}


void PelvisBone::updateRightLimb()
{
	rightLimb->updateSelfFromAbove(rightJointPosition, rotation);
}


void PelvisBone::initiateUpdateLeft()
{
	if (leftLimb != nullptr)
		leftLimb->updateFromExtreme();
}


void PelvisBone::initiateUpdateRight()
{
	if (rightLimb != nullptr)
		rightLimb->updateFromExtreme();
}
