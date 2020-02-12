#pragma once

#include <cmath>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Definitions.h"


struct node
{
	bool occupied = false;

	node* lower_back_left	= nullptr;
	node* lower_back_right	= nullptr;
	node* lower_front_left	= nullptr;
	node* lower_front_right = nullptr;
	node* upper_back_left	= nullptr;
	node* upper_back_right	= nullptr;
	node* upper_front_left	= nullptr;
	node* upper_front_right = nullptr;
};


class Octree
{
private:
	std::string fileName = "";
	std::vector<vect3> points;
	unsigned maxDepth		= 8;
	unsigned levelOfDetail	= 8;

	float xMin = 0.0f;
	float xMax = 0.0f;
	float yMin = 0.0f;
	float yMax = 0.0f;
	float zMin = 0.0f;
	float zMax = 0.0f;

	float xRaw = 0.0f;
	float yRaw = 0.0f;
	float zRaw = 0.0f;

	float size = 0.0f;

	vect3 centre = { 0.0f, 0.0f, 0.0f, 1.0f };

	node* root = nullptr;

public:
	Octree(const std::string& fn, unsigned dp);
	~Octree();

	float max3(const float& a, const float& b, const float& c);

	void readData();
	void initVolume();
	float getSize();
	vect3 getCentre();
	void addPoint(unsigned level, node* insertionPoint, vect3 datum, const vect3& p);
	bool sampleTree(unsigned level, node* sampleLocation, vect3 datum, const vect3& s);
};