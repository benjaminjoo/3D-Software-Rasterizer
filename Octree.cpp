#include "Octree.h"


Octree::Octree(const std::string& fn, unsigned dp) : fileName(fn), maxDepth(dp)
{
	levelOfDetail = std::min(levelOfDetail, maxDepth);
	levelOfDetail = std::max(levelOfDetail, 2u);

	readData();
	initVolume();

	root = new node;

	for (auto& p : points)
		addPoint(0, root, centre, p);
}


Octree::~Octree()
{
}


void Octree::readData()
{
	std::ifstream vertexInput(fileName);
	if (vertexInput.is_open())
	{
		std::string line = "";
		float x, y, z;
		while (std::getline(vertexInput, line))
		{
			std::stringstream inputLine(line);
			inputLine >> x >> y >> z;
			vect3 p{ x, y, z, 1.0f };
			points.push_back(p);
		}
		vertexInput.close();
		std::cout << points.size() << " vertices read..." << std::endl;
	}
	else
	{
		std::cout << "Could not read file: " << fileName << std::endl;
	}
}


void Octree::initVolume()
{
	xMin = points[0].x;
	xMax = points[0].x;
	yMin = points[0].y;
	yMax = points[0].y;
	zMin = points[0].z;
	zMax = points[0].z;

	for (auto& p : points)
	{
		if (p.x < xMin)
			xMin = p.x;
		if (p.x > xMax)
			xMax = p.x;

		if (p.y < yMin)
			yMin = p.y;
		if (p.y > yMax)
			yMax = p.y;

		if (p.z < zMin)
			zMin = p.z;
		if (p.z > zMax)
			zMax = p.z;
	}

	xRaw = xMax - xMin;
	yRaw = yMax - yMin;
	zRaw = zMax - zMin;

	size = max3(xRaw, yRaw, zRaw);

	centre.x = (xMin + xMax) * 0.5f;
	centre.y = (yMin + yMax) * 0.5f;
	centre.z = (zMin + zMax) * 0.5f;

	std::cout << "xMin: " << xMin << " xMax: " << xMax << std::endl;
	std::cout << "yMin: " << yMin << " yMax: " << yMax << std::endl;
	std::cout << "zMin: " << zMin << " zMax: " << zMax << std::endl;
	std::cout << "Size: " << size << std::endl;
	std::cout << "Cx: " << centre.x << " Cy: " << centre.y << " Cz: " << centre.z << std::endl;
}


float Octree::max3(const float& a, const float& b, const float& c)
{
	float result = a;
	if (b > result)
		result = b;
	if (c > result)
		result = c;
	return result;
}


float Octree::getSize()
{
	return size;
}


vect3 Octree::getCentre()
{
	return centre;
}


void Octree::addPoint(unsigned level, node* insertionPoint, vect3 datum, const vect3& p)
{
	insertionPoint->occupied = true;

	if (level < maxDepth)
	{
		unsigned char position = 0;

		if (p.x >= datum.x)
			position += 1;
		if (p.y >= datum.y)
			position += 2;
		if (p.z >= datum.z)
			position += 4;

		switch (position)
		{
		case 0:
		{
			if (insertionPoint->lower_front_left == nullptr)
			{
				insertionPoint->lower_front_left = new node;
				insertionPoint->lower_front_left->occupied = true;
			}
			datum.x -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			datum.y -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			datum.z -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			addPoint(level + 1, insertionPoint->lower_front_left, datum, p);
		}
		break;
		case 1:
		{
			if (insertionPoint->lower_front_right == nullptr)
			{
				insertionPoint->lower_front_right = new node;
				insertionPoint->lower_front_right->occupied = true;
			}
			datum.x += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			datum.y -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			datum.z -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			addPoint(level + 1, insertionPoint->lower_front_right, datum, p);
		}
		break;
		case 2:
		{
			if (insertionPoint->lower_back_left == nullptr)
			{
				insertionPoint->lower_back_left = new node;
				insertionPoint->lower_back_left->occupied = true;
			}
			datum.x -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			datum.y += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			datum.z -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			addPoint(level + 1, insertionPoint->lower_back_left, datum, p);
		}
		break;
		case 3:
		{
			if (insertionPoint->lower_back_right == nullptr)
			{
				insertionPoint->lower_back_right = new node;
				insertionPoint->lower_back_right->occupied = true;
			}
			datum.x += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			datum.y += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			datum.z -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			addPoint(level + 1, insertionPoint->lower_back_right, datum, p);
		}
		break;
		case 4:
		{
			if (insertionPoint->upper_front_left == nullptr)
			{
				insertionPoint->upper_front_left = new node;
				insertionPoint->upper_front_left->occupied = true;
			}
			datum.x -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			datum.y -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			datum.z += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			addPoint(level + 1, insertionPoint->upper_front_left, datum, p);
		}
		break;
		case 5:
		{
			if (insertionPoint->upper_front_right == nullptr)
			{
				insertionPoint->upper_front_right = new node;
				insertionPoint->upper_front_right->occupied = true;
			}
			datum.x += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			datum.y -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			datum.z += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			addPoint(level + 1, insertionPoint->upper_front_right, datum, p);
		}
		break;
		case 6:
		{
			if (insertionPoint->upper_back_left == nullptr)
			{
				insertionPoint->upper_back_left = new node;
				insertionPoint->upper_back_left->occupied = true;
			}
			datum.x -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			datum.y += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			datum.z += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			addPoint(level + 1, insertionPoint->upper_back_left, datum, p);
		}
		break;
		case 7:
		{
			if (insertionPoint->upper_back_right == nullptr)
			{
				insertionPoint->upper_back_right = new node;
				insertionPoint->upper_back_right->occupied = true;
			}
			datum.x += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			datum.y += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			datum.z += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
			addPoint(level + 1, insertionPoint->upper_back_right, datum, p);
		}
		break;
		}
	}
}


bool Octree::sampleTree(unsigned level, node* sampleLocation, vect3 datum, const vect3& s)
{
	if (level == 0)
	{
		sampleLocation = root;
		datum = centre;
	}
		
	if (sampleLocation != nullptr)
	{
		if (level == levelOfDetail)
			return true;
		else
		{
			unsigned char position = 0;

			if (s.x >= datum.x)
				position += 1;
			if (s.y >= datum.y)
				position += 2;
			if (s.z >= datum.z)
				position += 4;

			switch (position)
			{
			case 0:
			{
				if (sampleLocation->lower_front_left != nullptr)
				{
					datum.x -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					datum.y -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					datum.z -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					sampleTree(level + 1, sampleLocation->lower_front_left, datum, s);
				}
				else
					return false;
			}
			break;
			case 1:
			{
				if (sampleLocation->lower_front_right != nullptr)
				{
					datum.x += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					datum.y -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					datum.z -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					sampleTree(level + 1, sampleLocation->lower_front_right, datum, s);
				}
				else
					return false;
			}
			break;
			case 2:
			{
				if (sampleLocation->lower_back_left != nullptr)
				{
					datum.x -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					datum.y += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					datum.z -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					sampleTree(level + 1, sampleLocation->lower_back_left, datum, s);
				}
				else
					return false;
			}
			break;
			case 3:
			{
				if (sampleLocation->lower_back_right != nullptr)
				{
					datum.x += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					datum.y += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					datum.z -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					sampleTree(level + 1, sampleLocation->lower_back_right, datum, s);
				}
				else
					return false;
			}
			break;
			case 4:
			{
				if (sampleLocation->upper_front_left != nullptr)
				{
					datum.x -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					datum.y -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					datum.z += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					sampleTree(level + 1, sampleLocation->upper_front_left, datum, s);
				}
				else
					return false;
			}
			break;
			case 5:
			{
				if (sampleLocation->upper_front_right != nullptr)
				{
					datum.x += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					datum.y -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					datum.z += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					sampleTree(level + 1, sampleLocation->upper_front_right, datum, s);
				}
				else
					return false;
			}
			break;
			case 6:
			{
				if (sampleLocation->upper_back_left != nullptr)
				{
					datum.x -= (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					datum.y += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					datum.z += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					sampleTree(level + 1, sampleLocation->upper_back_left, datum, s);
				}
				else
					return false;
			}
			break;
			case 7:
			{
				if (sampleLocation->upper_back_right != nullptr)
				{
					datum.x += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					datum.y += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					datum.z += (size * 0.5f) / pow(2.0f, static_cast<float>(level + 1));
					sampleTree(level + 1, sampleLocation->upper_back_right, datum, s);
				}
				else
					return false;
			}
			break;
			}
		}
	}
	else
		return false;	
}
