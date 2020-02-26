#pragma once

#include <vector>
#include <memory>

#include "Definitions.h"
#include "Camera.h"
#include "Canvas.h"
#include "Button.h"
#include "ModelElementBuffer.h"


class Editor
{
public:

	std::shared_ptr<Camera>				Cam;
	std::shared_ptr<Canvas>				Screen;
	std::shared_ptr<ModelElementBuffer> Model;

	Button				arrowButton = { Screen, 0, 0, false,	tool::arrow,		LIGHT_GRAY };
	Button				crossButton = { Screen, 1, 0, true,		tool::cross,		LIGHT_GRAY };
	Button				lineButton = { Screen, 2, 0, false,		tool::line,			LIGHT_GRAY };
	Button				moveButton = { Screen, 3, 0, false,		tool::move,			LIGHT_GRAY };
	Button				rotateButton = { Screen, 4, 0, false,	tool::rotate,		LIGHT_GRAY };

	Button				topViewButton = { Screen, 5, 0, true,	tool::view_top,		LIGHT_GRAY };
	Button				frontViewButton = { Screen, 6, 0, false,tool::view_front,	LIGHT_GRAY };
	Button				sideViewButton = { Screen, 7, 0, false,	tool::view_side,	LIGHT_GRAY };

	Button				objSnapButton = { Screen, 8, 0, false,	tool::obj_snap,		LIGHT_GRAY };
	Button				grdSnapButton = { Screen, 9, 0, false,	tool::grid_snap,	LIGHT_GRAY };

	Side				currentView = Side::Top;
	editingMode			currentMode = editingMode::Placement;
	tool				currentTool = tool::cross;

	bool				isObjectSnapOn = false;
	bool				isGridSnapOn = false;
	bool				isOrthoOn = false;

	int					maxUndo = 20;
	int					currentID = 1;
	int					clicksInQueue = 0;
	int					currentEdit = 0;
	bool				startVertMoving = false;
	bool				endVertMoving = false;

	worldCoord			movementStart = { 0.0f, 0.0f, 0.0f };
	worldCoord			movementEnd = { 0.0f, 0.0f, 0.0f };

	worldCoord			rotationCentre = { 0.0f, 0.0f, 0.0f };
	worldCoord			rotationStart = { 0.0f, 0.0f, 0.0f };
	worldCoord			rotationEnd = { 0.0f, 0.0f, 0.0f };
	float				rotationAngle = 0.0f;

	float				scale = 1.0f;
	float				tolerance;

	screenCoord			planPosition = { 600, 300 };
	screenCoord			frontPosition = { 600, 300 };
	screenCoord			rightPosition = { 600, 300 };

	screenCoord			mousePosition = { 0, 0 };

	worldCoord			mouseBeforeZoom = { 0.0f, 0.0f, 0.0f };
	worldCoord			mouseAfterZoom = { 0.0f, 0.0f, 0.0f };

	worldCoord			viewportCentre = { -600.0f, -300.0f, 0.0f };

	screenCoord			dragStart = { 0, 0 };

	worldCoord			worldPosition = { 0.0f, 0.0f, 0.0f };


	Editor(float toler, std::shared_ptr<Camera> camera, std::shared_ptr < Canvas> screen, std::shared_ptr < ModelElementBuffer> buffer);
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

	void handleSelection(screenCoord X, worldCoord P);
	void handlePlacement(worldCoord P);
	void handleLineDrawing(worldCoord P);
	void handleRelocation(worldCoord P);
	void handleRotation(worldCoord P);

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
	float calculateAngle(worldCoord rotStart, worldCoord rotEnd);

	void selectAll();
	void deselectAll();
	unsigned int nVertSelected();
	unsigned int nLineSelected();

	void drawLine();

	void deleteSelected();
	void moveSelected();
	void copyMoveSelected();
	void rotateSelected();
	void copyRotateSelected();

	void quickSave();
};

