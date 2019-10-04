#pragma once

#include <vector>
#include "Definitions.h"
#include "Camera.h"
#include "Canvas.h"
#include "Button.h"
#include "ModelElementBuffer.h"


class Editor
{
public:
	Camera*				Cam;
	Canvas*				Screen;
	ModelElementBuffer* Model;

	Button				arrowButton;
	Button				crossButton;
	Button				lineButton;
	Button				moveButton;
	Button				rotateButton;

	Button				topViewButton;
	Button				frontViewButton;
	Button				sideViewButton;

	Side				currentView;
	editingMode			currentMode;
	tool				currentTool;

	bool				isGridSnapOn;
	bool				isOrthoOn;

	int					currentID;
	int					clicksInQueue;
	int					currentEdit;
	bool				startVertMoving;
	bool				endVertMoving;

	worldCoord			movementStart;
	worldCoord			movementEnd;

	worldCoord			rotationCentre;
	worldCoord			rotationStart;
	worldCoord			rotationEnd;
	double				rotationAngle;

	double		scale;
	double		tolerance;

	screenCoord planPosition;
	screenCoord frontPosition;
	screenCoord rightPosition;

	screenCoord mousePosition;

	worldCoord	mouseBeforeZoom;
	worldCoord	mouseAfterZoom;

	worldCoord	viewportCentre;

	screenCoord	dragStart;

	worldCoord	worldPosition;

	Editor(double, Camera*, Canvas*, ModelElementBuffer*);
	~Editor();

	worldCoord screen2world(screenCoord);
	screenCoord world2screen(worldCoord);

	void updateWorldPosition();
	void updatePan();
	void updateZoom();
	void updateScreen();

	void drawIcons();

	void mouseMotion(int, int);
	void leftMouseClick(screenCoord);

	void activateSelection();
	void activatePlacement();
	void activateLineDrawing();
	void activateRelocation();
	void activateRotation();

	void activateTopView();
	void activateFrontView();
	void activateRightView();

	void toggleGridSnap();
	void toggleOrtho();
	void switchOrthoOn();
	void switchOrthoOff();

	void alignToAxis(worldCoord*);
	void flattenVector(worldCoord*);
	double calculateAngle(worldCoord rotStart, worldCoord rotEnd);

	void deselectAll();

	void drawLine();

	void deleteSelected();
	void moveSelected();
	void rotateSelected();

	void quickSave();
};

