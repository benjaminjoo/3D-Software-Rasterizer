#include "Definitions.h"


float distancePN(polyNode a, polyNode b)
{
	return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}


Uint32 getColour(const unsigned char& a, const unsigned char& r, const unsigned char& g, const unsigned char& b)
{
	return (a << 24) | (r << 16) | (g << 8) | (b << 0);
}


Uint32 getColour(const float& r, const float& g, const float& b)
{
	unsigned char aa = 0, rr = 0, gg = 0, bb = 0;
	float rgbMax = std::max({ r, g, b });
	if (rgbMax > 1.0f)
	{
		rr = static_cast<unsigned char>(r / rgbMax * 255.0f);
		gg = static_cast<unsigned char>(g / rgbMax * 255.0f);
		bb = static_cast<unsigned char>(b / rgbMax * 255.0f);
	}
	else
	{
		rr = static_cast<unsigned char>(r * 255.0f);
		gg = static_cast<unsigned char>(g * 255.0f);
		bb = static_cast<unsigned char>(b * 255.0f);
	}

	//rr = static_cast<unsigned char>(std::min(r * 255.0f, 255.0f));
	//gg = static_cast<unsigned char>(std::min(g * 255.0f, 255.0f));
	//bb = static_cast<unsigned char>(std::min(b * 255.0f, 255.0f));

	return (aa << 24) | (rr << 16) | (gg << 8) | (bb << 0);
}


polyNode subNodes(polyNode a, polyNode b)
{
	polyNode t;

	t.x = a.x - b.x;
	t.y = a.y - b.y;
	t.s = a.s;

	return t;
}


polyNode addNodes(polyNode a, polyNode b)
{
	polyNode t;

	t.x = a.x + b.x;
	t.y = a.y + b.y;
	t.s = a.s;

	return t;
}


polyNode unitNode(polyNode a)
{
	polyNode t;

	float length = sqrtf(a.x * a.x + a.y * a.y);
	t.x = a.x / length;
	t.y = a.y / length;
	t.s = a.s;

	return t;
}


float dotNodes(polyNode a, polyNode b)
{
	return a.x * b.x + a.y * b.y;
}


polyNode getNormal(polyNode a, polyNode b)
{
	polyNode tempNode = subNodes(a, b);
	polyNode tempNormal;
	tempNormal.x = tempNode.y;
	tempNormal.y = -tempNode.x;
	tempNormal.s = 0;

	return tempNormal;
}


bool checkPolygonForSplitting(int n, vect2* V, polyNode p, polyNode q)
{
	bool needsSplitting = false;
	polyNode normal = getNormal(p, q);
	polyNode currentStart, currentEnd;
	float sStart, sEnd;
	polyNode P, Q;
	for (int i = 0; i < n - 1; i++)
	{
		P.x = V[i].x;
		P.y = V[i].y;
		P.s = 0;
		Q.x = V[i + 1].x;
		Q.y = V[i + 1].y;
		Q.s = 0;
		currentStart = subNodes(P, p);
		currentEnd = subNodes(Q, p);
		sStart = dotNodes(currentStart, normal);
		sEnd = dotNodes(currentEnd, normal);
		if (sign(sStart) && sign(sEnd))
		{
			if (sign(sStart) != sign(sEnd)) { needsSplitting = true; }
		}
	}
	return needsSplitting;
}


bool checkPolygonForSplitting(polygon4uv V, polyNode p, polyNode q)
{
	bool needsSplitting = false;
	polyNode normal = getNormal(p, q);
	polyNode currentStart, currentEnd;
	float sStart, sEnd;
	polyNode P, Q;
	for (int i = 0; i < 3; i++)
	{
		P.x = V.points[i].x;
		P.y = V.points[i].y;
		P.s = 0;
		Q.x = V.points[i + 1].x;
		Q.y = V.points[i + 1].y;
		Q.s = 0;
		currentStart = subNodes(P, p);
		currentEnd = subNodes(Q, p);
		sStart = dotNodes(currentStart, normal);
		sEnd = dotNodes(currentEnd, normal);
		if (sign(sStart) && sign(sEnd))
		{
			if (sign(sStart) != sign(sEnd)) { needsSplitting = true; }
		}
	}
	return needsSplitting;
}


bool iSect2dLine(vect2 a, vect2 b, polyNode p, polyNode q, vect2* r)
{
	polyNode a_, b_;
	a_.x = a.x; a_.y = a.y; a_.s = 0;
	b_.x = b.x; b_.y = b.y; b_.s = 0;
	polyNode ap = subNodes(a_, p);
	polyNode bp = subNodes(b_, p);

	polyNode normal = getNormal(p, q);

	float sA = dotNodes(ap, normal);
	float sB = dotNodes(bp, normal);

	float t;

	if (sign(sA) != sign(sB))
	{
		polyNode d = subNodes(b_, a_);
		float dist = dotNodes(d, normal);

		if (sA < 0.0f)
		{
			if (dist)
			{
				t = (dist - sB) / dist;

				r->x = a.x + t * (b.x - a.x);
				r->y = a.y + t * (b.y - a.y);
				r->u = a.u + t * (b.u - a.u);
				r->v = a.v + t * (b.v - a.v);
			}
		}
		if (sB < 0.0f)
		{
			if (dist)
			{
				t = (-dist - sA) / dist;

				r->x = b.x - t * (a.x - b.x);
				r->y = b.y - t * (a.y - b.y);
				r->u = b.u - t * (a.u - b.u);
				r->v = b.v - t * (a.v - b.v);
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}


void printCoord3(vect3 V)
{
	printf("x: %4.4f\ty: %4.4f\tz: %4.4f\n", V.x, V.y, V.z);
}


vect3 invertVector(vect3 a)
{
	vect3 temp;

	temp.x = -a.x;
	temp.y = -a.y;
	temp.z = -a.z;
	temp.x = 1.0f;

	return temp;
}


vect3 addVectors(const vect3& a, const vect3& b)
{
	vect3 temp;

	temp.x = a.x + b.x;
	temp.y = a.y + b.y;
	temp.z = a.z + b.z;
	temp.w = 1.0f;

	return temp;
}


bool match(const vect3& a, const vect3& b)
{
	if (a.x - b.x == 0.0f &&
		a.y - b.y == 0.0f &&
		a.z - b.z == 0.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}


vect3 subVectors(const vect3& a, const vect3& b)
{
	vect3 temp;

	temp.x = a.x - b.x;
	temp.y = a.y - b.y;
	temp.z = a.z - b.z;
	temp.w = 1.0f;

	return temp;
}


vect3 halfwayPoint(vect3 a, vect3 b)
{
	vect3 temp;

	temp.x = (a.x + b.x) * 0.5f;
	temp.y = (a.y + b.y) * 0.5f;
	temp.z = (a.z + b.z) * 0.5f;
	temp.w = 1.0f;

	return temp;
}


float distanceSquared(vect3 a, vect3 b)
{
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
}


float lengthSquared(vect3 v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}


vect3 unitVector(vect3 v)
{
	vect3 temp;

	float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

	temp.x = v.x / length;
	temp.y = v.y / length;
	temp.z = v.z / length;
	temp.w = 1.0f;

	return temp;
}


worldCoord unitVector2(worldCoord v)
{
	worldCoord temp;

	float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

	temp.x = v.x / length;
	temp.y = v.y / length;
	temp.z = v.z / length;

	return temp;
}


vect3 dirVector(float azm, float alt)
{
	vect3 temp;

	temp.x = cosf(alt * PI / 180.0f) * cosf(azm * PI / 180.0f);
	temp.y = cosf(alt * PI / 180.0f) * sinf(azm * PI / 180.0f);
	temp.z = sinf(alt * PI / 180.0f);
	temp.w = 1.0f;

	return temp;
}


float dotProduct(const vect3& a, const vect3& b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}


worldCoord rotate2(worldCoord target, Side currentView, worldCoord origin, float angle)
{
	worldCoord temp = target - origin;
	worldCoord result = { 0.0f, 0.0f, 0.0f };

	if (currentView == Side::Top)
	{
		result.x = temp.x * cosf(angle) - temp.y * sinf(angle);
		result.y = temp.y * cosf(angle) + temp.x * sinf(angle);
		result.z = target.z;
	}
	else if (currentView == Side::Front)
	{
		result.x = temp.x * cosf(angle) - temp.z * sinf(angle);
		result.y = target.y;
		result.z = temp.z * cosf(angle) + temp.x * sinf(angle);
	}
	else if (currentView == Side::Right)
	{
		result.x = target.x;
		result.y = temp.y * cosf(angle) - temp.z * sinf(angle);
		result.z = temp.z * cosf(angle) + temp.y * sinf(angle);
	}

	result.x += origin.x;
	result.y += origin.y;
	result.z += origin.z;

	return result;
}


float dotProductSquared(const vect3& a, const vect3& b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z) * (a.x * b.x + a.y * b.y + a.z * b.z);
}


float distPoint2Plane(vect3 P, triangle3dV T)
{
	//return abs((T.A - P) * T.N);
	return -dotProduct(subVectors(T.A, P), T.N);
}


float distPoint2Line(worldCoord P, Side view, line3 L)
{
	worldCoord normal = rotate2(unitVector2(L.vert[1] - L.vert[0]), view, { 0.0f, 0.0f, 0.0f }, PI * 0.5f);
	switch (view)
	{
	case Side::Top:
		normal.z = 0.0f;
		break;
	case Side::Front:
		normal.y = 0.0f;
		break;
	case Side::Right:
		normal.x = 0.0f;
		break;
	}
	return abs((L.vert[0] - P) * normal);
}


float distPoint2LineSquared(vect3 p, vect3 a, vect3 b)
{
	return lengthSquared(crossProduct(subVectors(p, a), subVectors(p, b))) / lengthSquared(subVectors(b, a));
}


bool pointIsAroundLine(worldCoord P, Side view, line3 L)
{
	worldCoord A = L.vert[0], B = L.vert[1];
	switch (view)
	{
	case Side::Top:
	{
		A.z = 0.0f;
		B.z = 0.0f;
		P.z = 0.0f;
	}
	break;
	case Side::Front:
	{
		A.y = 0.0f;
		B.y = 0.0f;
		P.y = 0.0f;
	}
	break;
	case Side::Right:
	{
		A.x = 0.0f;
		B.x = 0.0f;
		P.x = 0.0f;
	}
	break;
	}
	float sA = (B - A) * (A - P);
	float sB = (B - A) * (B - P);

	return (sign(sA) != sign(sB)) ? true : false;
}


vect3 crossProduct(vect3 a, vect3 b)
{
	vect3 t;

	t.x = a.y * b.z - a.z * b.y;
	t.y = a.z * b.x - a.x * b.z;
	t.z = a.x * b.y - a.y * b.x;

	t.w = 1.0f;

	return t;
}


vect3 midPoint(vect3 a, vect3 b)
{
	vect3 temp;

	temp.x = (a.x + b.x) / 2;
	temp.y = (a.y + b.y) / 2;
	temp.z = (a.z + b.z) / 2;
	temp.w = 1.0f;

	return temp;
}


coord2 view2screen(vect3 vertex, int width, int height, float hR, float vR)
{
	coord2 pixel;

	pixel.x = (int)(width * 0.5f + (vertex.x / vertex.z) * width * 0.5f * hR * 0.95f);
	pixel.y = (int)(height * 0.5f - (vertex.y / vertex.z) * width * 0.5f * vR * 0.95f);
	pixel.z = vertex.z;

	return pixel;
}


template <class T>
T getMinN(int n, T* list)
{
	T minimum = list[0];
	for (auto i = 0; i < n; i++)
	{
		if (list[i] < minimum) { minimum = list[i]; }
	}
	return minimum;
}



vect3 getXYZmin(int n, triangle3dV* T, float padding)
{
	vect3 xyzMin;
	xyzMin.x = T[0].A.x;
	xyzMin.y = T[0].A.y;
	xyzMin.z = T[0].A.z;
	xyzMin.w = 1.0f;
	for (int i = 0; i < n; i++)
	{
		if (T[i].A.x < xyzMin.x) { xyzMin.x = T[i].A.x; }
		if (T[i].A.y < xyzMin.y) { xyzMin.y = T[i].A.y; }
		if (T[i].A.z < xyzMin.z) { xyzMin.z = T[i].A.z; }
		if (T[i].B.x < xyzMin.x) { xyzMin.x = T[i].B.x; }
		if (T[i].B.y < xyzMin.y) { xyzMin.y = T[i].B.y; }
		if (T[i].B.z < xyzMin.z) { xyzMin.z = T[i].B.z; }
		if (T[i].C.x < xyzMin.x) { xyzMin.x = T[i].C.x; }
		if (T[i].C.y < xyzMin.y) { xyzMin.y = T[i].C.y; }
		if (T[i].C.z < xyzMin.z) { xyzMin.z = T[i].C.z; }
	}
	xyzMin.x -= padding;
	xyzMin.y -= padding;
	xyzMin.z -= padding;
	return xyzMin;
}


vect3 getXYZmax(int n, triangle3dV* T, float padding)
{
	vect3 xyzMax;
	xyzMax.x = T[0].A.x;
	xyzMax.y = T[0].A.y;
	xyzMax.z = T[0].A.z;
	xyzMax.w = 1.0f;
	for (int i = 0; i < n; i++)
	{
		if (T[i].A.x > xyzMax.x) { xyzMax.x = T[i].A.x; }
		if (T[i].A.y > xyzMax.y) { xyzMax.y = T[i].A.y; }
		if (T[i].A.z > xyzMax.z) { xyzMax.z = T[i].A.z; }
		if (T[i].B.x > xyzMax.x) { xyzMax.x = T[i].B.x; }
		if (T[i].B.y > xyzMax.y) { xyzMax.y = T[i].B.y; }
		if (T[i].B.z > xyzMax.z) { xyzMax.z = T[i].B.z; }
		if (T[i].C.x > xyzMax.x) { xyzMax.x = T[i].C.x; }
		if (T[i].C.y > xyzMax.y) { xyzMax.y = T[i].C.y; }
		if (T[i].C.z > xyzMax.z) { xyzMax.z = T[i].C.z; }
	}
	xyzMax.x += padding;
	xyzMax.y += padding;
	xyzMax.z += padding;
	return xyzMax;
}


bool checkBoundingBoxForCollision(vect3 p, boundingBox B)
{
	if ((p.x >= B.minExt.x && p.x <= B.maxExt.x) &&
		(p.y >= B.minExt.y && p.y <= B.maxExt.y) &&
		(p.z >= B.minExt.z && p.z <= B.maxExt.z))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool onScreen(coord2 test, int w, int h)
{
	return (test.x >= 0 && test.x < w && test.y >= 0 && test.y < h);
}


void drawCrosshair(Uint32* pixels, int w, int h, int hole, int size, Uint32 colour)
{
	for (int i = 0; i < (size - hole); i++)
	{
		pixels[(h / 2 - 1) * w + w / 2 + hole + i] = colour;
		pixels[(h / 2) * w + w / 2 + hole + i] = colour;

		pixels[(h / 2 + hole + i) * w + w / 2 - 1] = colour;
		pixels[(h / 2 + hole + i) * w + w / 2] = colour;
	}

	for (int i = size - hole; i > 0; i--)
	{
		pixels[(h / 2 - 1) * w - w / 2 - hole - i] = colour;
		pixels[(h / 2) * w - w / 2 - hole - i] = colour;

		pixels[(h / 2 - hole - i) * w + w / 2 - 1] = colour;
		pixels[(h / 2 - hole - i) * w + w / 2] = colour;
	}
}


int sign(const float& a)
{
	int t;
	if (a > 0) { t = 1; }
	else if (a == 0) { t = 0; }
	else { t = -1; }

	return t;
}


int roundInt(float a)
{
	float t = a - (int)a;
	if (a >= 0.5)
	{
		return (int)a + 1;
	}
	else
	{
		return (int)a;
	}
}


float pow10(int n)
{
	float result = 1.0;

	if (n > 0)
	{
		for (int i = 0; i < n; i++)
		{
			result *= 10;
		}
	}
	else if (n < 0)
	{
		for (int i = 0; i > n; i--)
		{
			result /= 10;
		}
	}

	return result;
}


int pow2(int n)
{
	int result = 1;

	for (int i = 0; i < n; i++)
	{
		result *= 2;
	}

	return result;
}


int* getFractionals(float number, int nDecimals)
{
	int dec;
	if (number > 0)
	{
		dec = int((number - (int)(number)) * pow10(nDecimals));
	}
	else dec = 0;

	int* decimalDigits = new int[nDecimals];

	for (int i = 0; i < nDecimals; i++)
	{
		if (i < nDecimals)
		{
			decimalDigits[i] = (int)(dec / pow10(nDecimals - 1 - i));
			int rem = dec % (int)(pow10(nDecimals - 1 - i));
			dec = rem;
		}
	}

	return decimalDigits;
}


int* getIntegers(float number, int* n)
{
	int rawInt = (int)(number);
	int nDigits = 0;
	while (rawInt > 0)
	{
		rawInt /= 10;
		nDigits++;
	}

	int* intDigits = new int[nDigits];
	rawInt = (int)(number);

	for (int i = nDigits - 1; i >= 0; i--)
	{
		intDigits[i] = rawInt % 10;
		rawInt /= 10;
	}

	*n = nDigits;
	return intDigits;
	delete[] intDigits;
}


void freePointerToPolyArray(triangle3dV** ptr, int nDimensions)
{
	for (int i = 0; i < nDimensions; i++)
	{
		if (ptr[i] != nullptr)
		{
			delete[] ptr[i];
		}
	}
	if (ptr != nullptr)
	{
		delete[] ptr;
	}
}


bool* getSinglePETSCIICharacter(char char_no)
{
	bool* current;

	switch (char_no)
	{
	case 33:
		current = petscii_1;
		break;
	case 34:
		current = petscii_2;
		break;
	case 35:
		current = petscii_3;
		break;
	case 36:
		current = petscii_4;
		break;
	case 37:
		current = petscii_5;
		break;
	case 38:
		current = petscii_6;
		break;
	case 39:
		current = petscii_7;
		break;
	case 40:
		current = petscii_8;
		break;
	case 41:
		current = petscii_9;
		break;
	case 42:
		current = petscii_10;
		break;
	case 43:
		current = petscii_11;
		break;
	case 44:
		current = petscii_12;
		break;
	case 45:
		current = petscii_13;
		break;
	case 46:
		current = petscii_14;
		break;
	case 47:
		current = petscii_15;
		break;
	case 48:
		current = petscii_16;
		break;
	case 49:
		current = petscii_17;
		break;
	case 50:
		current = petscii_18;
		break;
	case 51:
		current = petscii_19;
		break;
	case 52:
		current = petscii_20;
		break;
	case 53:
		current = petscii_21;
		break;
	case 54:
		current = petscii_22;
		break;
	case 55:
		current = petscii_23;
		break;
	case 56:
		current = petscii_24;
		break;
	case 57:
		current = petscii_25;
		break;
	case 58:
		current = petscii_26;
		break;
	case 59:
		current = petscii_27;
		break;
	case 60:
		current = petscii_28;
		break;
	case 61:
		current = petscii_29;
		break;
	case 62:
		current = petscii_30;
		break;
	case 63:
		current = petscii_31;
		break;
	case 64:
		current = petscii_32;
		break;
	case 65:
		current = petscii_65;
		break;
	case 66:
		current = petscii_66;
		break;
	case 67:
		current = petscii_67;
		break;
	case 68:
		current = petscii_68;
		break;
	case 69:
		current = petscii_69;
		break;
	case 70:
		current = petscii_70;
		break;
	case 71:
		current = petscii_71;
		break;
	case 72:
		current = petscii_72;
		break;
	case 73:
		current = petscii_73;
		break;
	case 74:
		current = petscii_74;
		break;
	case 75:
		current = petscii_75;
		break;
	case 76:
		current = petscii_76;
		break;
	case 77:
		current = petscii_77;
		break;
	case 78:
		current = petscii_78;
		break;
	case 79:
		current = petscii_79;
		break;
	case 80:
		current = petscii_80;
		break;
	case 81:
		current = petscii_81;
		break;
	case 82:
		current = petscii_82;
		break;
	case 83:
		current = petscii_83;
		break;
	case 84:
		current = petscii_84;
		break;
	case 85:
		current = petscii_85;
		break;
	case 86:
		current = petscii_86;
		break;
	case 87:
		current = petscii_87;
		break;
	case 88:
		current = petscii_88;
		break;
	case 89:
		current = petscii_89;
		break;
	case 90:
		current = petscii_90;
		break;
	case 97:
		current = petscii_33;
		break;
	case 98:
		current = petscii_34;
		break;
	case 99:
		current = petscii_35;
		break;
	case 100:
		current = petscii_36;
		break;
	case 101:
		current = petscii_37;
		break;
	case 102:
		current = petscii_38;
		break;
	case 103:
		current = petscii_39;
		break;
	case 104:
		current = petscii_40;
		break;
	case 105:
		current = petscii_41;
		break;
	case 106:
		current = petscii_42;
		break;
	case 107:
		current = petscii_43;
		break;
	case 108:
		current = petscii_44;
		break;
	case 109:
		current = petscii_45;
		break;
	case 110:
		current = petscii_46;
		break;
	case 111:
		current = petscii_47;
		break;
	case 112:
		current = petscii_48;
		break;
	case 113:
		current = petscii_49;
		break;
	case 114:
		current = petscii_50;
		break;
	case 115:
		current = petscii_51;
		break;
	case 116:
		current = petscii_52;
		break;
	case 117:
		current = petscii_53;
		break;
	case 118:
		current = petscii_54;
		break;
	case 119:
		current = petscii_55;
		break;
	case 120:
		current = petscii_56;
		break;
	case 121:
		current = petscii_57;
		break;
	case 122:
		current = petscii_58;
		break;
	case 91:
		current = petscii_59;
		break;
	case 93:
		current = petscii_61;
		break;
	default:
		current = petscii_0;
		break;
	}

	return current;
}