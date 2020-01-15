#pragma once

#include "Definitions.h"
#include "Canvas.h"
#include "LightSource.h"

#include <cmath>
#include <memory>

class Projection
{
public:
	Projection();
	~Projection();

	static int GetYMax3(coord2* p);

	static int GetYMin3(coord2* p);

	static void clampValue(double* value, double lower, double upper);

	static transform3d getTransformation(double azm, double alt, double rol);

	static vect3 rotXrad(const double& sinA, const double& cosA, const vect3& v);

	static vect3 rotYrad(const double& sinA, const double& cosA, const vect3& v);

	static vect3 rotZrad(const double& sinA, const double& cosA, const vect3& v);

	static triangle3dV rotXT(double angle, triangle3dV T);

	static triangle3dV rotYT(double angle, triangle3dV T);

	static triangle3dV rotZT(double angle, triangle3dV T);

	static vect3 translate(double x, double y, double z, vect3 v);

	static triangle3dV translateT(double x, double y, double z, triangle3dV T);

	static vect3 scale(double scaleX, double scaleY, double scaleZ, vect3 v);

	static triangle3dV scaleT(double x, double y, double z, triangle3dV T);

	static vect3 multiplyVxM(const mat4x4& m, const vect3& v);

	static vect3 multiplyMxV(const mat4x4& m, const vect3& v);

	static void movePoly(vect3 m, triangle3dV& poly);

	static void rotateMesh(int n, triangle3dV* object, double rx, double ry, double rz);

	static void transformMesh(int n, triangle3dV* object, double scX, double scY, double scZ, double mvX, double mvY, double mvZ,
		double rX, double rY, double rZ);

	static void object2worldT(const vect3& sc, const vect3& mv, const vect3& rt, triangle3dV& T);

	static void world2view(triangle3dV& tr, transform3d T, double x, double y, double z);

	static vect3 world2view(transform3d T, vect3 tr, double x, double y, double z);

	static point3 world2viewP(transform3d T, point3 p, double x, double y, double z);

	static vect3 sun2view(transform3d T, vect3 v);

	static vect3 screen2view(coord2 pixel, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio);

	static void illuminatePoly(LightSource L, triangle3dV* T, const triangle3dV& P, const projectionStyle& style);

	static Uint32 modifyColour(const Uint32& inputColour, const double& illumination);

	static textCoord getUVCoord(const vect3& startV, const vect3& endV, const textCoord& startC, const textCoord& endC, const vect3& testV);

	static void fillTriangleSolidColour(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio);

	static void fillTriangleCheckerboard(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio);

	static void fillTriangleFlatShaded(const triangle2dG& t, std::shared_ptr<Canvas> screen);

	static void fillTriangleGouraudShaded(const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio);

	static void fillTriangleDepthVisualised(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, double zNear, double h_ratio, double v_ratio);

	static void fillTriangleSunlight(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio, txt* texture);

	static void fillTriangleTorchlight(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio, txt* texture,
				const double& lightIntensity, const double& maxIllumination);

	static void fillTriangleTorchlightSolidColour(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio,
				const double& lightIntensity, const double& maxIllumination);
};

