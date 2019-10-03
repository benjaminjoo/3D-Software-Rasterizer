#include "Context.h"



Context::Context()
{
	gouraudShading			= true;
	visualiseDepth			= false;
	editingMode				= false;
	showFPS					= false;
	gravityOn				= true;
	isPaused				= true;
	mouseLookBlocked		= false;
}


Context::~Context()
{
}
