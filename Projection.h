#pragma once

#include "Definitions.h"
#include "Canvas.h"
#include "LightSource.h"
#include "Lamp.h"

#include <cmath>
#include <memory>

class Projection
{
public:
	Projection();
	~Projection();

	int GetYMax3(coord2* p);

	int GetYMin3(coord2* p);

	float getMax(int n, float* v);

	float getMin(int n, float* v);

	void clampValue(float* value, float lower, float upper);

	vect3 rotXrad(const float& sinA, const float& cosA, const vect3& v);

	vect3 rotYrad(const float& sinA, const float& cosA, const vect3& v);

	vect3 rotZrad(const float& sinA, const float& cosA, const vect3& v);

	triangle3dV rotXT(float angle, triangle3dV T);

	triangle3dV rotYT(float angle, triangle3dV T);

	triangle3dV rotZT(float angle, triangle3dV T);

	vect3 translate(float x, float y, float z, vect3 v);

	triangle3dV translateT(float x, float y, float z, triangle3dV T);

	vect3 scale(float scaleX, float scaleY, float scaleZ, vect3 v);

	triangle3dV scaleT(float x, float y, float z, triangle3dV T);

	vect3 multiplyVxM(const mat4x4& m, const vect3& v);

	vect3 multiplyMxV(const mat4x4& m, const vect3& v);

	mat4x4 multiplyMxM(const mat4x4& p, const mat4x4& q);

	void movePoly(vect3 m, triangle3dV& poly);

	void rotateMesh(int n, triangle3dV* object, float rx, float ry, float rz);

	void transformMesh(int n, triangle3dV* object, float scX, float scY, float scZ, float mvX, float mvY, float mvZ,
		float rX, float rY, float rZ);

	mat4x4 getTranslation(vect3 mv);

	mat4x4 getRotation(axis t, float a);

	void object2worldT(const vect3& sc, const vect3& mv, const vect3& rt, triangle3dV& T);

	void world2view(triangle3dV& tr, mat4x4& rot, mat4x4& mov);

	point3 world2viewP(point3 p, mat4x4& rot, mat4x4& mov);

	vect3 screen2view(coord2 pixel, std::shared_ptr<Canvas> screen, float h_ratio, float v_ratio);

	void illuminatePoly(LightSource L, triangle3dV* T, const triangle3dV& P, const projectionStyle& style, const float& min);

	Uint32 modifyColour(const Uint32& inputColour, const float& illumination);

	textCoord getUVCoord(const vect3& startV, const vect3& endV, const textCoord& startC, const textCoord& endC, const vect3& testV);

	void fillTriangleDepth(const triangle2dG& t, std::shared_ptr<Canvas> screen, float zNear, float h_ratio, float v_ratio);

	void fillTriangleSolidColour(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, float h_ratio, float v_ratio);

	void fillTriangleCheckerboard(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, float h_ratio, float v_ratio);

	void fillTriangleShadows(const triangle3dV& W, const triangle3dV& V, const triangle2dG& S,
							std::shared_ptr<Lamp> spotlight, std::shared_ptr<Canvas> screen, float h_ratio, float v_ratio);

	void fillTriangleFlatShaded(const triangle2dG& t, std::shared_ptr<Canvas> screen);

	void fillTriangleGouraudShaded(const triangle2dG& t, std::shared_ptr<Canvas> screen, float h_ratio, float v_ratio);

	void fillTriangleDepthVisualised(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, float zNear, float h_ratio, float v_ratio);

	void fillTriangleSunlight(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, float h_ratio, float v_ratio, txt* texture);

	void fillTriangleTorchlight(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, float h_ratio, float v_ratio, txt* texture,
		const float& lightIntensity, const float& maxIllumination);

	void fillTriangleTorchlightSolidColour(const triangle3dV& T, const triangle2dG& t, std::shared_ptr<Canvas> screen, float h_ratio, float v_ratio,
				const float& lightIntensity, const float& maxIllumination);

	void fillTriangleBlinnPhong(const triangle2dG& t, float* spec, std::shared_ptr<Canvas> screen, float h_ratio, float v_ratio);
};

