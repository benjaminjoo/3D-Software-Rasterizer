#pragma once

#include "Definitions.h"
#include "Canvas.h"

#include <cmath>
#include <memory>

class Projection
{
public:
	Projection();
	~Projection();

	static int GetYMax3(coord2* p);

	static int GetYMin3(coord2* p);

	static vect3 screen2view(coord2 pixel, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio);

	static textCoord getUVCoord(const vect3& startV, const vect3& endV, const textCoord& startC, const textCoord& endC, const vect3& testV);

	static void fillTriangleCheckerboard(const triangle3dV&, const triangle2dG&, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio);

	static void fillTriangleFlatShaded(const triangle2dG& t, std::shared_ptr<Canvas> screen);

	static void fillTriangleGouraudShaded(const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio);
};

