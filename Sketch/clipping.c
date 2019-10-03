#define MAXCLIPVERTS		10

struct plane
{
	vect3 N;
	vect3 P;
};

struct box
{
	plane nearPlane;
	plane farPlane;
	plane leftPlane;
	plane rightPlane;
	plane topPlane;
	plane bottomPlane;
};

double fovH, fovV, aspectRatio;
aspectRatio = (double)SCREEN_WIDTH / (double)SCREEN_HEIGHT;
fovH = 90.0;
fovV = (atan(tan(fovH / 2) / aspectRatio)) * 2;

box frustum;

frustum.nearPlane.N 	= {             0.0,            0.0,            1.0,            0.0 };
frustum.nearPlane.P 	= {             0.0,            0.0,          zNear,            1.0 };
frustum.farPlane.N 	= {             0.0,            0.0,           -1.0,            0.0 };
frustum.farPlane.P 	= {             0.0,            0.0,           zFar,            1.0 };
frustum.leftPlane.N 	= {   cos(fovH / 2),  	        0.0,  sin(fovH / 2), 	        0.0 };
frustum.leftPlane.P 	= { 	        0.0,            0.0,            0.0,            1.0 };
frustum.rightPlane.N 	= {  -cos(fovH / 2),            0.0,  sin(fovH / 2),            0.0 };
frustum.rightPlane.P 	= {             0.0,            0.0,            0.0,            1.0 };
frustum.topPlane.N 	= {             0.0, -cos(fovV / 2),  sin(fovV / 2),            0.0 };
frustum.topPlane.P 	= {             0.0,            0.0,            0.0,            1.0 };
frustum.bottomPlane.N 	= {             0.0,  cos(fovV / 2),  sin(fovV / 2),            0.0 };
frustum.bottomPlane.P 	= {             0.0,            0.0,            0.0,            1.0 };

vect3 nullVector { 0.0, 0.0, 0.0, 0.0 }
vect3 vertexList[MAXCLIPVERTS];
vect3 vertexTemp[MAXCLIPVERTS];

for(int i = 0; i < MAXCLIPVERTS; i++)
{
	vertexList[i] = nullVector;
	vertexTemp[i] = nullVector;
}

vertexList[0] = viewT.A;	
vertexList[1] = viewT.B;
vertexList[2] = viewT.C;





