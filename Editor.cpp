#include "Editor.h"


Editor::Editor(double toler, Camera* ca, Canvas* sc, ModelElementBuffer* buff)
{
	currentView		= Top;
	currentMode		= Placement;
	currentTool		= cross;

	isGridSnapOn	= false;
	isOrthoOn		= false;

	currentID		= 0;
	clicksInQueue	= 0;

	movementStart	= { 0.0f, 0.0f, 0.0f };
	movementEnd		= { 0.0f, 0.0f, 0.0f };

	rotationCentre	= { 0.0f, 0.0f, 0.0f };
	rotationStart	= { 0.0f, 0.0f, 0.0f };
	rotationEnd		= { 0.0f, 0.0f, 0.0f };
	rotationAngle	= 0.0f;

	C				= ca;	//Camera
	S				= sc;	//Canvas
	M				= buff;	//Model elements

	arrowButton		= { S, 0, 0, false,	arrow,		LIGHT_GRAY };
	crossButton		= { S, 1, 0, true,	cross,		LIGHT_GRAY };
	moveButton		= { S, 2, 0, false,	move,		LIGHT_GRAY };
	rotateButton	= { S, 3, 0, false,	rotate,		LIGHT_GRAY };

	topViewButton	= { S, 5, 0, true,	view_top,	LIGHT_GRAY };
	frontViewButton = { S, 6, 0, false,	view_front,	LIGHT_GRAY };
	sideViewButton	= { S, 7, 0, false,	view_side,	LIGHT_GRAY };

	scale			= 1.0f;
	tolerance		= toler;

	planPosition	= { 600, 300 };
	frontPosition	= { 600, 300 };
	rightPosition	= { 600, 300 };

	mousePosition	= { 0, 0 };

	mouseBeforeZoom = { 0.0f, 0.0f, 0.0f };
	mouseAfterZoom	= { 0.0f, 0.0f, 0.0f };

	viewportCentre	= { -600.0f, -300.0f, 0.0f };;

	dragStart		= { 0, 0 };

	worldPosition	= { 0.0f, 0.0f, 0.0f };
}


Editor::~Editor()
{
}


worldCoord Editor::screen2world(screenCoord Sc)
{
	worldCoord temp = { 0.0f, 0.0f, 0.0f };

	if (currentView == Top)
	{
		temp.x = ((double)(Sc.x) / scale) + viewportCentre.x;
		temp.y = ((double)(Sc.y) / scale) + viewportCentre.y;
		temp.z = 0.0f;
	}
	if (currentView == Front)
	{
		temp.x = ((double)(Sc.x) / scale) + viewportCentre.x;
		temp.y = 0.0f;
		temp.z = ((double)(Sc.y) / scale) + viewportCentre.y;
	}
	if (currentView == Right)
	{
		temp.x = 0.0f;
		temp.y = ((double)(Sc.x) / scale) + viewportCentre.x;
		temp.z = ((double)(Sc.y) / scale) + viewportCentre.y;
	}

	if (isGridSnapOn)
	{
		temp.x = (int)(temp.x / tolerance) * tolerance;
		temp.y = (int)(temp.y / tolerance) * tolerance;
		temp.z = (int)(temp.z / tolerance) * tolerance;
	}

	return temp;
}


screenCoord Editor::world2screen(worldCoord Wc)
{
	screenCoord temp = { 0, 0 };

	if (currentView == Top)
	{
		temp.x = (int)(((Wc.x) - viewportCentre.x) * scale);
		temp.y = (int)(((Wc.y) - viewportCentre.y) * scale);
	}
	else if (currentView == Front)
	{
		temp.x = (int)(((Wc.x) - viewportCentre.x) * scale);
		temp.y = (int)(((Wc.z) - viewportCentre.y) * scale);
	}
	else if (currentView == Right)
	{
		temp.x = (int)(((Wc.y) - viewportCentre.x) * scale);
		temp.y = (int)(((Wc.z) - viewportCentre.y) * scale);
	}

	return temp;
}


void Editor::updateWorldPosition()
{
	if (currentView == Top)
	{
		worldPosition.x = viewportCentre.x + (double)planPosition.x / scale;
		worldPosition.y = -(viewportCentre.y + (double)planPosition.y / scale);
		worldPosition.z = 0.0f;
	}
	else if (currentView == Front)
	{
		worldPosition.x = viewportCentre.x + (double)frontPosition.x / scale;
		worldPosition.y = 0.0f;
		worldPosition.z = -(viewportCentre.y + (double)frontPosition.y / scale);
	}
	else if (currentView == Right)
	{
		worldPosition.x = 0.0f;
		worldPosition.y = viewportCentre.x + (double)rightPosition.x / scale;
		worldPosition.z = -(viewportCentre.y + (double)rightPosition.y / scale);
	}
}


void Editor::updatePan()
{
	SDL_GetMouseState(&dragStart.x, &dragStart.y);

	viewportCentre.x += ((double)mousePosition.x - (double)dragStart.x) / scale;
	viewportCentre.y += ((double)mousePosition.y - (double)dragStart.y) / scale;

	mousePosition = dragStart;
}


void Editor::updateZoom()
{
	mouseAfterZoom = screen2world(mousePosition);

	if (currentView == Top)
	{
		viewportCentre.x += (mouseBeforeZoom.x - mouseAfterZoom.x);
		viewportCentre.y += (mouseBeforeZoom.y - mouseAfterZoom.y);
	}
	else if (currentView == Front)
	{
		viewportCentre.x += (mouseBeforeZoom.x - mouseAfterZoom.x);
		viewportCentre.y += (mouseBeforeZoom.z - mouseAfterZoom.z);
	}
	else if (currentView == Right)
	{
		viewportCentre.x += (mouseBeforeZoom.y - mouseAfterZoom.y);
		viewportCentre.y += (mouseBeforeZoom.z - mouseAfterZoom.z);
	}
}


void Editor::updateScreen()
{
	SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
	mouseBeforeZoom = screen2world(mousePosition);

	this->updateWorldPosition();

	if (currentView == Top)
	{
		S->drawMouseCursor(currentMode, planPosition,	LIGHT_BLUE);
	}
	else if (currentView == Front)
	{
		S->drawMouseCursor(currentMode, frontPosition,	LIGHT_BLUE);
	}
	else if (currentView == Right)
	{
		S->drawMouseCursor(currentMode, rightPosition,	LIGHT_BLUE);
	}

	//Display scale
	S->displayValue(scale * 100.0f, 2, 1, 2, WHITE);
	S->displayString("%", 2, 2, WHITE);

	//Display mouse cursor world coordinates
	S->displayValue(worldPosition.x, 4, 60, 2, RED	);
	S->displayValue(worldPosition.y, 4, 40, 2, GREEN);
	S->displayValue(worldPosition.z, 4, 20, 2, BLUE	);

	//Draw axes
	C->drawLine(world2screen({ -1000.0, 0.0, 0.0 }), world2screen({ 1000.0, 0.0, 0.0 }), 3, RED		, S->pixelBuffer);
	C->drawLine(world2screen({ 0.0, -1000.0, 0.0 }), world2screen({ 0.0, 1000.0, 0.0 }), 3, GREEN	, S->pixelBuffer);
	C->drawLine(world2screen({ 0.0, 0.0, -1000.0 }), world2screen({ 0.0, 0.0, 1000.0 }), 3, BLUE	, S->pixelBuffer);

	for (auto i = 0; i < M->getVertex3BufferSize(); i++)
	{
		if (!M->isVertex3Deleted(i))						//Draw all visible vertices
		{
			vertex3 tempVert = M->getVertex3(i);
			screenCoord temp = world2screen(tempVert.pos);
			if (M->isVertex3Selected(i))
			{
				C->drawSpot(temp, RED,	S->pixelBuffer);
			}
			else
			{
				C->drawSpot(temp, BLUE,	S->pixelBuffer);
			}
		}
		if (currentMode == Relocation && clicksInQueue == 1)
		{
			if (M->isVertex3Selected(i))					//Draw all visible vertices currently being moved
			{
				vertex3 tempVert = M->getVertex3(i);
				worldCoord tempMove	= subVectors2(movementStart, mouseBeforeZoom);
				if (isOrthoOn) { this->alignToAxis(&tempMove); }
				tempVert.pos.x -= tempMove.x;
				tempVert.pos.y -= tempMove.y;
				tempVert.pos.z -= tempMove.z;
				screenCoord temp = world2screen(tempVert.pos);
				C->drawSpot(temp, ORANGE, S->pixelBuffer);
			}
		}
		if (currentMode == Rotation && clicksInQueue == 2)	//Draw all visible vertices currently being rotated
		{
			if (M->isVertex3Selected(i))
			{
				vertex3 tempVert		= M->getVertex3(i);
				worldCoord startVect	= unitVector2(subVectors2(rotationStart,	rotationCentre));
				if (isOrthoOn)
				{
					worldCoord temp = subVectors2(mouseBeforeZoom, rotationCentre);
					this->alignToAxis(&temp);
					mouseBeforeZoom = addVectors2(rotationCentre, temp);
				}
				worldCoord endVect		= unitVector2(subVectors2(mouseBeforeZoom,	rotationCentre));
				double rotAngle			= calculateAngle(startVect, endVect);
				tempVert.pos			= rotate2(tempVert.pos, currentView, rotationCentre, rotAngle);
				screenCoord temp		= world2screen(tempVert.pos);
				C->drawSpot(temp, ORANGE, S->pixelBuffer);
			}
		}
	}

	if (currentMode == Relocation)							//Hint line of movement
	{
		if (clicksInQueue == 1)
		{
			worldCoord tempMouse = screen2world(mousePosition);
			worldCoord tempMove = subVectors2(tempMouse, movementStart);
			if (isOrthoOn) { this->alignToAxis(&tempMove); }
			worldCoord tempEnd;
			tempEnd.x = movementStart.x + tempMove.x;
			tempEnd.y = movementStart.y + tempMove.y;
			tempEnd.z = movementStart.z + tempMove.z;
			C->drawLine(world2screen(movementStart), world2screen(tempEnd), 4, DARK_GRAY, S->pixelBuffer);
		}
	}

	if (currentMode == Rotation)							//Hint rotation angle
	{
		if (clicksInQueue == 1)
		{
			if (isOrthoOn)
			{
				worldCoord temp = subVectors2(mouseBeforeZoom, rotationCentre);
				this->alignToAxis(&temp);
				mouseBeforeZoom = addVectors2(rotationCentre, temp);
			}
			C->drawLine(world2screen(rotationCentre), world2screen(mouseBeforeZoom), 2, DARK_GRAY, S->pixelBuffer);
		}
		else if (clicksInQueue == 2)
		{
			if (isOrthoOn)
			{
				worldCoord temp = subVectors2(mouseBeforeZoom, rotationCentre);
				this->alignToAxis(&temp);
				mouseBeforeZoom = addVectors2(rotationCentre, temp);
			}
			C->drawLine(world2screen(rotationCentre), world2screen(rotationStart), 2, DARK_GRAY, S->pixelBuffer);
			C->drawLine(world2screen(rotationCentre), world2screen(mouseBeforeZoom), 2, DARK_GRAY, S->pixelBuffer);
		}
	}

	this->drawIcons();
}


void Editor::drawIcons()
{
	arrowButton.displayIcon();
	crossButton.displayIcon();
	moveButton.displayIcon();
	rotateButton.displayIcon();

	topViewButton.displayIcon();
	frontViewButton.displayIcon();
	sideViewButton.displayIcon();
}


void Editor::mouseMotion(int mx, int my)
{
	if (currentView == Top)
	{
		planPosition.x = mx;
		planPosition.y = my;
		if (planPosition.x < 0) { planPosition.x = 0; }
		if (planPosition.x > EDITOR_WIDTH - 1) { planPosition.x = EDITOR_WIDTH - 1; }
		if (planPosition.y < 0) { planPosition.y = 0; }
		if (planPosition.y > EDITOR_HEIGHT - 1) { planPosition.y = EDITOR_HEIGHT - 1; }
	}
	else if (currentView == Front)
	{
		frontPosition.x = mx;
		frontPosition.y = my;
		if (frontPosition.x < 0) { frontPosition.x = 0; }
		if (frontPosition.x > EDITOR_WIDTH - 1) { frontPosition.x = EDITOR_WIDTH - 1; }
		if (frontPosition.y < 0) { frontPosition.y = 0; }
		if (frontPosition.y > EDITOR_HEIGHT - 1) { frontPosition.y = EDITOR_HEIGHT - 1; }
	}
	else if (currentView == Right)
	{
		rightPosition.x = mx;
		rightPosition.y = my;
		if (rightPosition.x < 0) { rightPosition.x = 0; }
		if (rightPosition.x > EDITOR_WIDTH - 1) { rightPosition.x = EDITOR_WIDTH - 1; }
		if (rightPosition.y < 0) { rightPosition.y = 0; }
		if (rightPosition.y > EDITOR_HEIGHT - 1) { rightPosition.y = EDITOR_HEIGHT - 1; }
	}
}


void Editor::leftMouseClick(screenCoord X)
{
	if (X.y > 31)
	{
		if (currentMode == Selection)
		{
			for (auto i = 0; i < M->getVertex3BufferSize(); i++)
			{
				if (!M->isVertex3Deleted(i))
				{
					vertex3 tempVert = M->getVertex3(i);
					worldCoord tempCoordW = tempVert.pos;
					screenCoord P = world2screen(tempCoordW);
					if (abs(P.x - X.x) <= 10 && abs(P.y - X.y) <= 10)
					{
						M->selectVertex3byIndex(i);
					}
				}
			}
		}
		if (currentMode == Placement)
		{
			worldCoord temp = screen2world(X);
			printf("Entity added at x: %.4f\ty: %.4f\n", temp.x, temp.y);
			vertex3 tempVert = { currentID, temp, false, false };
			M->addVertex3(tempVert);
			currentID++;
		}
		if (currentMode == Relocation)
		{
			if (clicksInQueue == 0)
			{
				movementStart = screen2world(X);
				clicksInQueue++;
			}
			else
			{
				movementEnd = screen2world(X);
				this->moveSelected();
				clicksInQueue = 0;
			}
		}
		if (currentMode == Rotation)
		{
			if (clicksInQueue == 0)
			{
				rotationCentre = screen2world(X);
				clicksInQueue++;
			}
			else if (clicksInQueue == 1)
			{
				rotationStart = screen2world(X);
				if (isOrthoOn)
				{
					worldCoord temp = subVectors2(rotationStart, rotationCentre);
					this->alignToAxis(&temp);
					rotationStart = addVectors2(rotationCentre, temp);
				}
				clicksInQueue++;
			}
			else if (clicksInQueue == 2)
			{
				rotationEnd = screen2world(X);
				{
					worldCoord temp = subVectors2(rotationEnd, rotationCentre);
					this->alignToAxis(&temp);
					rotationEnd = addVectors2(rotationCentre, temp);
				}
				this->rotateSelected();
				clicksInQueue = 0;
			}

		}
	}
	else
	{
		int clickLocation = X.x / 32;
		switch (clickLocation)
		{
			case 0:
				this->activateSelection();
			break;
			case 1:
				this->activatePlacement();
			break;
			case 2:
				this->activateRelocation();
			break;
			case 3:
				this->activateRotation();
			break;

			case 5:
				this->activateTopView();
			break;
			case 6:
				this->activateFrontView();
			break;
			case 7:
				this->activateRightView();
			break;
		}	
	}
}


void Editor::activateSelection()
{
	currentTool = arrow;
	currentMode = Selection;
	arrowButton.turnOn();
	crossButton.turnOff();
	moveButton.turnOff();
	rotateButton.turnOff();
}


void Editor::activatePlacement()
{
	currentTool = cross;
	currentMode = Placement;
	arrowButton.turnOff();
	crossButton.turnOn();
	moveButton.turnOff();
	rotateButton.turnOff();
}


void Editor::activateRelocation()
{
	currentTool = move;
	currentMode = Relocation;
	arrowButton.turnOff();
	crossButton.turnOff();
	moveButton.turnOn();
	rotateButton.turnOff();
}


void Editor::activateRotation()
{
	currentTool = rotate;
	currentMode = Rotation;
	arrowButton.turnOff();
	crossButton.turnOff();
	moveButton.turnOff();
	rotateButton.turnOn();
}


void Editor::activateTopView()
{
	currentView = Top;
	topViewButton.turnOn();
	frontViewButton.turnOff();
	sideViewButton.turnOff();
}


void Editor::activateFrontView()
{
	currentView = Front;
	topViewButton.turnOff();
	frontViewButton.turnOn();
	sideViewButton.turnOff();
}


void Editor::activateRightView()
{
	currentView = Right;
	topViewButton.turnOff();
	frontViewButton.turnOff();
	sideViewButton.turnOn();
}


void Editor::toggleGridSnap()
{
	isGridSnapOn = isGridSnapOn ? false : true;
}


void Editor::toggleOrtho()
{
	isOrthoOn = isOrthoOn ? false : true;
}


void Editor::switchOrthoOn()
{
	isOrthoOn = true;
}


void Editor::switchOrthoOff()
{
	isOrthoOn = false;
}


void Editor::alignToAxis(worldCoord* V)
{
	char greatest = ' ';
	double maxExtent = abs(V->x); greatest = 'x';
	if (abs(V->y) > maxExtent) { maxExtent = abs(V->y); greatest = 'y'; }
	if (abs(V->z) > maxExtent) { maxExtent = abs(V->z); greatest = 'z'; }
	
	if (greatest == 'x')
	{
		V->y = V->z = 0.0f;
	}
	else if (greatest == 'y')
	{
		V->x = V->z = 0.0f;
	}
	else if (greatest == 'z')
	{
		V->x = V->y = 0.0f;
	}
}


double Editor::calculateAngle(worldCoord rotStart, worldCoord rotEnd)
{
	double rotAngle = 0.0f;
	worldCoord cp = crossProduct(rotStart, rotEnd);
	switch (currentView)
	{
	case Top:
		rotAngle = atan2(cp.z, dotProduct2(rotStart, rotEnd));
		break;
	case Front:
		rotAngle = -atan2(cp.y, dotProduct2(rotStart, rotEnd));
		break;
	case Right:
		rotAngle = atan2(cp.x, dotProduct2(rotStart, rotEnd));
		break;
	}
	return rotAngle;
}


void Editor::deselectAll()
{
	for (auto i = 0; i < M->getVertex3BufferSize(); i++)
	{
		M->deselectVertex3byIndex(i);
	}
	clicksInQueue = 0;
}


void Editor::deleteSelected()
{
	for (auto i = 0; i < M->getVertex3BufferSize(); i++)
	{
		if (!M->isVertex3Deleted(i) && M->isVertex3Selected(i))
		{
			M->deleteVertex3byIndex(i);
		}
	}
}


void Editor::moveSelected()
{
	for (auto i = 0; i < M->getVertex3BufferSize(); i++)
	{
		if (!M->isVertex3Deleted(i) && M->isVertex3Selected(i))
		{
			worldCoord move = subVectors2(movementEnd, movementStart);
			if (isOrthoOn) { this->alignToAxis(&move); }
			M->moveVertex3byIndex(i, move);
		}
	}
}


void Editor::rotateSelected()
{
	for (auto i = 0; i < M->getVertex3BufferSize(); i++)
	{
		if (!M->isVertex3Deleted(i) && M->isVertex3Selected(i))
		{
			worldCoord startVect	= unitVector2(subVectors2(rotationStart,	rotationCentre));
			worldCoord endVect		= unitVector2(subVectors2(rotationEnd,		rotationCentre));
			rotationAngle			= calculateAngle(startVect, endVect);
			M->rotVertex3byIndex(i, currentView, rotationCentre, rotationAngle);
		}
	}
}


void Editor::quickSave()
{
	M->exportTextFile();
}