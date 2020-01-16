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

	int GetYMax3(coord2* p);

	int GetYMin3(coord2* p);

	void clampValue(double* value, double lower, double upper);

	transform3d getTransformation(double azm, double alt, double rol);

	vect3 rotXrad(const double& sinA, const double& cosA, const vect3& v);

	vect3 rotYrad(const double& sinA, const double& cosA, const vect3& v);

	vect3 rotZrad(const double& sinA, const double& cosA, const vect3& v);

	triangle3dV rotXT(double angle, triangle3dV T);

	triangle3dV rotYT(double angle, triangle3dV T);

	triangle3dV rotZT(double angle, triangle3dV T);

	vect3 translate(double x, double y, double z, vect3 v);

	triangle3dV translateT(double x, double y, double z, triangle3dV T);

	vect3 scale(double scaleX, double scaleY, double scaleZ, vect3 v);

	triangle3dV scaleT(double x, double y, double z, triangle3dV T);

	vect3 multiplyVxM(const mat4x4& m, const vect3& v);

	vect3 multiplyMxV(const mat4x4& m, const vect3& v);

	mat4x4 multiplyMxM(const mat4x4& p, const mat4x4& q);

	void movePoly(vect3 m, triangle3dV& poly);

	void rotateMesh(int n, triangle3dV* object, double rx, double ry, double rz);

	void transformMesh(int n, triangle3dV* object, double scX, double scY, double scZ, double mvX, double mvY, double mvZ,
		double rX, double rY, double rZ);

	void object2worldT(const vect3& sc, const vect3& mv, const vect3& rt, triangle3dV& T);

	void world2view(triangle3dV& tr, transform3d T, double x, double y, double z);

	void world2view(triangle3dV& tr, mat4x4& rot, mat4x4& mov);

	vect3 world2view(transform3d T, vect3 tr, double x, double y, double z);

	point3 world2viewP(point3 p, mat4x4& rot, mat4x4& mov);

	vect3 sun2view(transform3d T, vect3 v);

	vect3 screen2view(coord2 pixel, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio);

	void illuminatePoly(LightSource L, triangle3dV* T, const triangle3dV& P, const projectionStyle& style);

	Uint32 modifyColour(const Uint32& inputColour, const double& illumination);

	textCoord getUVCoord(const vect3& startV, const vect3& endV, const textCoord& startC, const textCoord& endC, const vect3& testV);

	void fillTriangleSolidColour(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio);

	void fillTriangleCheckerboard(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio);

	void fillTriangleFlatShaded(const triangle2dG& t, std::shared_ptr<Canvas> screen);

	void fillTriangleGouraudShaded(const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio);

	void fillTriangleDepthVisualised(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, double zNear, double h_ratio, double v_ratio);

	void fillTriangleSunlight(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio, txt* texture);

	void fillTriangleTorchlight(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio, txt* texture,
		const double& lightIntensity, const double& maxIllumination);

	void fillTriangleTorchlightSolidColour(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, double h_ratio, double v_ratio,
				const double& lightIntensity, const double& maxIllumination);
};

