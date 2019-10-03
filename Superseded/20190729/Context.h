#pragma once

class Context
{
public:

	bool gouraudShading;
	bool visualiseDepth;
	bool editingMode;
	bool showFPS;
	bool gravityOn;
	bool isPaused;
	bool mouseLookBlocked;

	Context();
	~Context();
};

