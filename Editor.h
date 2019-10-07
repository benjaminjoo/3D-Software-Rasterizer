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

	Button				objSnapButton;
	Button				grdSnapButton;

	Side				currentView;
	editingMode			currentMode;
	tool				currentTool;

	bool				isObjectSnapOn;
	bool				isGridSnapOn;
	bool				isOrthoOn;

	int					maxUndo;
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
	void compensatePan();
	void compensateZoom();
	void updateScreen();
	void updateUtilities();
	void updateLines();
	void updateVertices();
	void hintResult();

	void drawIcons();

	void mouseMotion(int, int);
	void leftMouseClick(screenCoord);

	void activateSelection();
	void activatePlacement();
	void activateLineDrawing();
	void activateRelocation();
	void activateRotation();
	void activateCopyRelocation();
	void activateCopyRotation();

	void activateTopView();
	void activateFrontView();
	void activateRightView();

	void toggleObjectSnap();
	void toggleGridSnap();
	void toggleOrtho();
	void switchOrthoOn();
	void switchOrthoOff();

	bool snapToVert(worldCoord*);
	void alignToAxis(worldCoord*);
	void flattenVector(worldCoord*);
	double calculateAngle(worldCoord rotStart, worldCoord rotEnd);

	void selectAll();
	void deselectAll();

	void drawLine();

	void deleteSelected();
	void moveSelected();
	void copyMoveSelected();
	void rotateSelected();
	void copyRotateSelected();

	void quickSave();
};

