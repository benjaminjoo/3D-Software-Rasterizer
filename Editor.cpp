#include "Editor.h"


Editor::Editor(float toler, std::shared_ptr<Camera> camera, std::shared_ptr < Canvas> screen, std::shared_ptr < ModelElementBuffer> buffer) :
	tolerance(toler), Cam(camera), Screen(screen), Model(buffer)
{
	std::cout << "Editor constructor called" << std::endl;
}


Editor::~Editor()
{
	std::cout << "Editor destructor called" << std::endl;
}


worldCoord Editor::screen2world(screenCoord Sc)
{
	worldCoord temp = { 0.0f, 0.0f, 0.0f };

	if (currentView == Top)
	{
		temp.x = ((float)(Sc.x) / scale) + viewportCentre.x;
		temp.y = ((float)(Sc.y) / scale) + viewportCentre.y;
		temp.z = 0.0f;
	}
	else if (currentView == Front)
	{
		temp.x = ((float)(Sc.x) / scale) + viewportCentre.x;
		temp.y = 0.0f;
		temp.z = ((float)(Sc.y) / scale) + viewportCentre.y;
	}
	else if (currentView == Right)
	{
		temp.x = 0.0f;
		temp.y = ((float)(Sc.x) / scale) + viewportCentre.x;
		temp.z = ((float)(Sc.y) / scale) + viewportCentre.y;
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
		worldPosition.x = viewportCentre.x + (float)planPosition.x / scale;
		worldPosition.y = -(viewportCentre.y + (float)planPosition.y / scale);
		worldPosition.z = 0.0f;
	}
	else if (currentView == Front)
	{
		worldPosition.x = viewportCentre.x + (float)frontPosition.x / scale;
		worldPosition.y = 0.0f;
		worldPosition.z = -(viewportCentre.y + (float)frontPosition.y / scale);
	}
	else if (currentView == Right)
	{
		worldPosition.x = 0.0f;
		worldPosition.y = viewportCentre.x + (float)rightPosition.x / scale;
		worldPosition.z = -(viewportCentre.y + (float)rightPosition.y / scale);
	}
}


void Editor::compensatePan()
{
	SDL_GetMouseState(&dragStart.x, &dragStart.y);

	viewportCentre.x += ((float)mousePosition.x - (float)dragStart.x) / scale;
	viewportCentre.y += ((float)mousePosition.y - (float)dragStart.y) / scale;

	mousePosition = dragStart;
}


void Editor::compensateZoom()
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


void Editor::updateUtilities()
{
	SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
	mouseBeforeZoom = screen2world(mousePosition);

	this->updateWorldPosition();

	if (currentView == Top)
	{
		Screen->drawMouseCursor(currentMode, planPosition, LIGHT_BLUE);
		if (isObjectSnapOn && this->snapToVert(&mouseBeforeZoom))
		{
			Screen->drawSnapTarget(planPosition, WHITE);
		}
	}
	else if (currentView == Front)
	{
		Screen->drawMouseCursor(currentMode, frontPosition, LIGHT_BLUE);
		if (isObjectSnapOn && this->snapToVert(&mouseBeforeZoom))
		{
			Screen->drawSnapTarget(frontPosition, WHITE);
		}
	}
	else if (currentView == Right)
	{
		Screen->drawMouseCursor(currentMode, rightPosition, LIGHT_BLUE);
		if (isObjectSnapOn && this->snapToVert(&mouseBeforeZoom))
		{
			Screen->drawSnapTarget(rightPosition, WHITE);
		}
	}

	//Display scale
	Screen->displayValue(scale * 100.0f, 2, 1, 2, WHITE);
	Screen->displayString("%", 2, 2, WHITE);

	//Display mouse cursor world coordinates
	Screen->displayValue(worldPosition.x, 4, 60, 2, RED);
	Screen->displayValue(worldPosition.y, 4, 40, 2, GREEN);
	Screen->displayValue(worldPosition.z, 4, 20, 2, BLUE);

	//Draw axes
	Screen->drawLine(world2screen({ -1000.0, 0.0, 0.0 }), world2screen({ 1000.0, 0.0, 0.0 }), 3, RED);
	Screen->drawLine(world2screen({ 0.0, -1000.0, 0.0 }), world2screen({ 0.0, 1000.0, 0.0 }), 3, GREEN);
	Screen->drawLine(world2screen({ 0.0, 0.0, -1000.0 }), world2screen({ 0.0, 0.0, 1000.0 }), 3, BLUE);
}


void Editor::updateLines()
{
	for (auto i = 0; i < Model->getLine3BufferSize(); i++)
	{
		if (!Model->isLine3Deleted(i))
		{
			line3 tempLine = Model->getLine3(i);
			screenCoord tempStart = world2screen(tempLine.vert[0]);
			screenCoord tempEnd = world2screen(tempLine.vert[1]);

			if (Model->isLine3Selected(i))
			{
				Screen->drawLine(tempStart, tempEnd, 1, RED);
				if (clicksInQueue > 0 && tempLine.id == currentEdit)
				{
					worldCoord move = mouseBeforeZoom - movementStart;
					if (isOrthoOn) { this->alignToAxis(&move); }
					screenCoord currentPosition = world2screen(movementStart + move);
					if (startVertMoving)
					{
						Screen->drawLine(currentPosition, tempEnd, 1, ORANGE);
						Screen->drawLine(world2screen(movementStart), currentPosition, 4, DARK_GRAY);
					}
					if (endVertMoving)
					{
						Screen->drawLine(tempStart, currentPosition, 1, ORANGE);
						Screen->drawLine(world2screen(movementStart), currentPosition, 4, DARK_GRAY);
					}
				}
				else if (clicksInQueue == 0)
				{
					if (abs(tempStart.x - mousePosition.x) <= 5 &&
						abs(tempStart.y - mousePosition.y) <= 5)
					{
						Screen->drawSpot(tempStart, RED);
					}
					if (abs(tempEnd.x - mousePosition.x) <= 5 &&
						abs(tempEnd.y - mousePosition.y) <= 5)
					{
						Screen->drawSpot(tempEnd, RED);
					}
				}
			}
			else
			{
				Screen->drawLine(tempStart, tempEnd, 1, BLUE);
			}
		}
		if ((currentMode == Relocation || currentMode == CopyRelocation) && clicksInQueue == 1)
		{
			if (Model->isLine3Selected(i) && !Model->isLine3Deleted(i))						//Draw all visible lines currently being moved
			{
				line3 tempLine = Model->getLine3(i);
				worldCoord tempMove = movementStart - mouseBeforeZoom;
				if (isOrthoOn) { this->alignToAxis(&tempMove); }

				tempLine.vert[0] -= tempMove;
				tempLine.vert[1] -= tempMove;

				screenCoord tempStart = world2screen(tempLine.vert[0]);
				screenCoord tempEnd = world2screen(tempLine.vert[1]);

				Screen->drawLine(tempStart, tempEnd, 1, ORANGE);
			}
		}
		if ((currentMode == Rotation || currentMode == CopyRotation) && clicksInQueue == 2)		//Draw all visible lines currently being rotated
		{
			if (Model->isLine3Selected(i) && !Model->isLine3Deleted(i))
			{
				line3 tempLine = Model->getLine3(i);
				worldCoord startVect = unitVector2(rotationStart - rotationCentre);
				if (isOrthoOn)
				{
					worldCoord temp = mouseBeforeZoom - rotationCentre;
					this->alignToAxis(&temp);
					mouseBeforeZoom = rotationCentre + temp;
				}
				worldCoord endVect = unitVector2(mouseBeforeZoom - rotationCentre);
				float rotAngle = calculateAngle(startVect, endVect);
				tempLine.vert[0] = rotate2(tempLine.vert[0], currentView, rotationCentre, rotAngle);
				tempLine.vert[1] = rotate2(tempLine.vert[1], currentView, rotationCentre, rotAngle);
				screenCoord tempStart = world2screen(tempLine.vert[0]);
				screenCoord tempEnd = world2screen(tempLine.vert[1]);
				Screen->drawLine(tempStart, tempEnd, 1, ORANGE);
			}
		}
	}
}


void Editor::updateVertices()
{
	for (auto i = 0; i < Model->getVertex3BufferSize(); i++)
	{
		if (!Model->isVertex3Deleted(i))						//Draw all visible vertices
		{
			vertex3 tempVert = Model->getVertex3(i);
			screenCoord temp = world2screen(tempVert.pos);
			if (Model->isVertex3Selected(i))
			{
				Screen->drawSpot(temp, RED);
			}
			else
			{
				Screen->drawSpot(temp, BLUE);
			}
		}
		if ((currentMode == Relocation || currentMode == CopyRelocation) && clicksInQueue == 1)
		{
			if (Model->isVertex3Selected(i) && !Model->isVertex3Deleted(i))					//Draw all visible vertices currently being moved
			{
				vertex3 tempVert = Model->getVertex3(i);
				worldCoord tempMove = movementStart - mouseBeforeZoom;
				if (isOrthoOn) { this->alignToAxis(&tempMove); }
				tempVert.pos -= tempMove;
				screenCoord temp = world2screen(tempVert.pos);
				Screen->drawSpot(temp, ORANGE);
			}
		}
		if ((currentMode == Rotation || currentMode == CopyRotation) && clicksInQueue == 2)		//Draw all visible vertices currently being rotated
		{
			if (Model->isVertex3Selected(i) && !Model->isVertex3Deleted(i))
			{
				vertex3 tempVert = Model->getVertex3(i);
				worldCoord startVect = unitVector2(rotationStart - rotationCentre);
				if (isOrthoOn)
				{
					worldCoord temp = mouseBeforeZoom - rotationCentre;
					this->alignToAxis(&temp);
					mouseBeforeZoom = rotationCentre + temp;
				}
				worldCoord endVect = unitVector2(mouseBeforeZoom - rotationCentre);
				float rotAngle = calculateAngle(startVect, endVect);
				tempVert.pos = rotate2(tempVert.pos, currentView, rotationCentre, rotAngle);
				screenCoord temp = world2screen(tempVert.pos);
				Screen->drawSpot(temp, ORANGE);
			}
		}
	}
}


void Editor::hintResult()
{
	if (currentMode == Relocation || currentMode == CopyRelocation)								//Hint line of movement
	{
		if (clicksInQueue == 1)
		{
			worldCoord tempMouse = screen2world(mousePosition);
			worldCoord tempMove = tempMouse - movementStart;
			if (isOrthoOn) { this->alignToAxis(&tempMove); }
			worldCoord tempEnd;
			tempEnd = movementStart + tempMove;
			Screen->drawLine(world2screen(movementStart), world2screen(tempEnd), 4, DARK_GRAY);
		}
	}

	if (currentMode == Rotation || currentMode == CopyRotation)								//Hint rotation angle
	{
		if (clicksInQueue == 1)
		{
			if (isOrthoOn)
			{
				worldCoord temp = mouseBeforeZoom - rotationCentre;
				this->alignToAxis(&temp);
				mouseBeforeZoom = rotationCentre + temp;
			}
			Screen->drawLine(world2screen(rotationCentre), world2screen(mouseBeforeZoom), 2, DARK_GRAY);
		}
		else if (clicksInQueue == 2)
		{
			if (isOrthoOn)
			{
				worldCoord temp = mouseBeforeZoom - rotationCentre;
				this->alignToAxis(&temp);
				mouseBeforeZoom = rotationCentre + temp;
			}
			Screen->drawLine(world2screen(rotationCentre), world2screen(rotationStart), 2, DARK_GRAY);
			Screen->drawLine(world2screen(rotationCentre), world2screen(mouseBeforeZoom), 2, DARK_GRAY);
		}
	}

	if (currentMode == LineDrawing)								//Hint line of movement
	{
		if (clicksInQueue == 1)
		{
			worldCoord tempMouse = screen2world(mousePosition);
			worldCoord tempMove = tempMouse - movementStart;
			if (isOrthoOn) { this->alignToAxis(&tempMove); }
			worldCoord tempEnd;
			tempEnd = movementStart + tempMove;
			Screen->drawLine(world2screen(movementStart), world2screen(tempEnd), 1, BLUE);
		}
	}
}


void Editor::updateScreen()
{
	this->updateUtilities();

	this->updateLines();

	this->updateVertices();

	this->hintResult();

	this->drawIcons();
}


void Editor::drawIcons()
{
	arrowButton.displayIcon();
	crossButton.displayIcon();
	lineButton.displayIcon();
	moveButton.displayIcon();
	rotateButton.displayIcon();

	topViewButton.displayIcon();
	frontViewButton.displayIcon();
	sideViewButton.displayIcon();

	objSnapButton.displayIcon();
	grdSnapButton.displayIcon();
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
	worldCoord P = screen2world(X);
	if (isObjectSnapOn) { this->snapToVert(&P); }
	if (X.y > 31)
	{
		if (currentMode == Selection)
		{
			for (auto i = 0; i < Model->getVertex3BufferSize(); i++)
			{
				if (!Model->isVertex3Deleted(i))
				{
					vertex3		tempVert = Model->getVertex3(i);
					worldCoord	tempCoordW = tempVert.pos;
					screenCoord tempCoordS = world2screen(tempCoordW);
					if (abs(tempCoordS.x - X.x) <= 10 && abs(tempCoordS.y - X.y) <= 10)
					{
						Model->selectVertex3byIndex(i);
					}
				}
			}
			for (auto i = 0; i < Model->getLine3BufferSize(); i++)
			{
				if (!Model->isLine3Deleted(i))
				{
					if (!Model->isLine3Selected(i))
					{
						line3		tempLine = Model->getLine3(i);
						worldCoord	currentP = P;
						float		dist = distPoint2Line(currentP, currentView, tempLine);
						if (pointIsAroundLine(currentP, currentView, tempLine) && (int)(dist * scale) < 5)
						{
							Model->selectLine3byIndex(i);
						}
					}
					else
					{
						line3 		tempLine = Model->getLine3(i);
						screenCoord tempStart = world2screen(tempLine.vert[0]);
						screenCoord tempEnd = world2screen(tempLine.vert[1]);

						if (clicksInQueue > 0 && tempLine.id == currentEdit)
						{
							movementEnd = P;
							worldCoord move = movementEnd - movementStart;
							if (isOrthoOn) { this->alignToAxis(&move); }
							if (startVertMoving) { Model->moveLine3EndPoint(i, 0, currentView, move); }
							if (endVertMoving) { Model->moveLine3EndPoint(i, 1, currentView, move); }
							startVertMoving = 0;
							endVertMoving = 0;
							currentEdit = 0;
							clicksInQueue = 0;
						}
						else if (clicksInQueue == 0)
						{
							if (abs(tempStart.x - mousePosition.x) <= 5 &&
								abs(tempStart.y - mousePosition.y) <= 5)
							{
								movementStart = tempLine.vert[0];
								startVertMoving = 1;
								endVertMoving = 0;
								currentEdit = tempLine.id;
								clicksInQueue++;
							}
							if (abs(tempEnd.x - mousePosition.x) <= 5 &&
								abs(tempEnd.y - mousePosition.y) <= 5)
							{
								movementStart = tempLine.vert[1];
								startVertMoving = 0;
								endVertMoving = 1;
								currentEdit = tempLine.id;
								clicksInQueue++;
							}
						}
					}
				}
			}
		}
		if (currentMode == Placement)
		{
			worldCoord temp = P;
			vertex3 tempVert = { currentID, temp, false, false };
			Model->addVertex3(tempVert);
			currentID++;
		}
		if (currentMode == LineDrawing)
		{
			if (clicksInQueue == 0)
			{
				movementStart = P;
				clicksInQueue++;
			}
			else
			{
				movementEnd = P;
				this->drawLine();
				currentID++;
				clicksInQueue = 0;
			}
		}
		if (currentMode == Relocation || currentMode == CopyRelocation)
		{
			if (clicksInQueue == 0)
			{
				movementStart = P;
				clicksInQueue++;
			}
			else
			{
				movementEnd = P;
				if (currentMode == Relocation)
				{
					this->moveSelected();
				}
				else if (currentMode == CopyRelocation)
				{
					this->copyMoveSelected();
				}
				clicksInQueue = 0;
			}
		}
		if (currentMode == Rotation || currentMode == CopyRotation)
		{
			if (clicksInQueue == 0)
			{
				rotationCentre = P;
				clicksInQueue++;
			}
			else if (clicksInQueue == 1)
			{
				rotationStart = P;
				if (isOrthoOn)
				{
					worldCoord temp = rotationStart - rotationCentre;
					this->alignToAxis(&temp);
					rotationStart = rotationCentre + temp;
				}
				clicksInQueue++;
			}
			else if (clicksInQueue == 2)
			{
				rotationEnd = P;
				if (isOrthoOn)
				{
					worldCoord temp = rotationEnd - rotationCentre;
					this->alignToAxis(&temp);
					rotationEnd = rotationCentre + temp;
				}
				if (currentMode == Rotation)
				{
					this->rotateSelected();
				}
				else if (currentMode == CopyRotation)
				{
					this->copyRotateSelected();
				}
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
			this->activateLineDrawing();
			break;
		case 3:
			this->activateRelocation();
			break;
		case 4:
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

		case 8:
			this->toggleObjectSnap();
			break;
		case 9:
			this->toggleGridSnap();
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
	lineButton.turnOff();
	moveButton.turnOff();
	rotateButton.turnOff();
}


void Editor::activatePlacement()
{
	currentTool = cross;
	currentMode = Placement;
	arrowButton.turnOff();
	crossButton.turnOn();
	lineButton.turnOff();
	moveButton.turnOff();
	rotateButton.turnOff();
}


void Editor::activateLineDrawing()
{
	currentTool = line;
	currentMode = LineDrawing;
	arrowButton.turnOff();
	crossButton.turnOff();
	lineButton.turnOn();
	moveButton.turnOff();
	rotateButton.turnOff();
}


void Editor::activateRelocation()
{
	currentTool = move;
	currentMode = Relocation;
	arrowButton.turnOff();
	crossButton.turnOff();
	lineButton.turnOff();
	moveButton.turnOn();
	rotateButton.turnOff();
}


void Editor::activateRotation()
{
	currentTool = rotate;
	currentMode = Rotation;
	arrowButton.turnOff();
	crossButton.turnOff();
	lineButton.turnOff();
	moveButton.turnOff();
	rotateButton.turnOn();
}


void Editor::activateCopyRelocation()
{
	currentTool = copy_move;
	currentMode = CopyRelocation;
	arrowButton.turnOff();
	crossButton.turnOff();
	lineButton.turnOff();
	moveButton.turnOff();
	rotateButton.turnOff();
}


void Editor::activateCopyRotation()
{
	currentTool = copy_rotate;
	currentMode = CopyRotation;
	arrowButton.turnOff();
	crossButton.turnOff();
	lineButton.turnOff();
	moveButton.turnOff();
	rotateButton.turnOff();
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


void Editor::toggleObjectSnap()
{
	isObjectSnapOn = isObjectSnapOn ? false : true;
	if (isObjectSnapOn)
	{
		objSnapButton.turnOn();
	}
	else
	{
		objSnapButton.turnOff();
	}
}


void Editor::toggleGridSnap()
{
	isGridSnapOn = isGridSnapOn ? false : true;
	if (isGridSnapOn)
	{
		grdSnapButton.turnOn();
	}
	else
	{
		grdSnapButton.turnOff();
	}
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


bool Editor::snapToVert(worldCoord* P)
{
	bool snapped = false;
	screenCoord tempP = world2screen(*P);
	for (auto i = 0; i < Model->getVertex3BufferSize(); i++)
	{
		vertex3 tempW = Model->getVertex3(i);
		screenCoord tempS = world2screen(tempW.pos);
		if (abs(tempP.x - tempS.x) < 5 &&
			abs(tempP.y - tempS.y) < 5)
		{
			*P = tempW.pos;
			snapped = true;
		}
	}
	for (auto i = 0; i < Model->getLine3BufferSize(); i++)
	{
		worldCoord	tempStart = Model->getLine3(i).vert[0];
		worldCoord	tempEnd = Model->getLine3(i).vert[1];
		screenCoord tempStartS = world2screen(tempStart);
		screenCoord tempEndS = world2screen(tempEnd);
		if (abs(tempP.x - tempStartS.x) < 5 &&
			abs(tempP.y - tempStartS.y) < 5)
		{
			*P = tempStart;
			snapped = true;
		}
		if (abs(tempP.x - tempEndS.x) < 5 &&
			abs(tempP.y - tempEndS.y) < 5)
		{
			*P = tempEnd;
			snapped = true;
		}
	}
	return snapped;
}


void Editor::alignToAxis(worldCoord* V)
{
	char greatest = ' ';
	float maxExtent;

	switch (currentView)
	{
	case Top:
	{
		maxExtent = abs(V->x); greatest = 'x';
		if (abs(V->y) > maxExtent) { maxExtent = abs(V->y); greatest = 'y'; }
		if (greatest == 'x') { V->y = 0.0f; }
		else if (greatest == 'y') { V->x = 0.0f; }
	}
	break;
	case Front:
	{
		maxExtent = abs(V->x); greatest = 'x';
		if (abs(V->z) > maxExtent) { maxExtent = abs(V->z); greatest = 'z'; }
		if (greatest == 'x') { V->z = 0.0f; }
		else if (greatest == 'z') { V->x = 0.0f; }
	}
	break;
	case Right:
	{
		maxExtent = abs(V->y); greatest = 'y';
		if (abs(V->z) > maxExtent) { maxExtent = abs(V->z); greatest = 'z'; }
		if (greatest == 'y') { V->z = 0.0f; }
		else if (greatest == 'z') { V->y = 0.0f; }
	}
	break;
	}
}


void Editor::flattenVector(worldCoord* V)
{
	switch (currentView)
	{
	case Top:
		V->z = 0.0f;
		break;
	case Front:
		V->y = 0.0f;
		break;
	case Right:
		V->x = 0.0f;
		break;
	}
}


float Editor::calculateAngle(worldCoord rotStart, worldCoord rotEnd)
{
	float rotAngle = 0.0f;
	worldCoord cp = rotStart ^ rotEnd;
	switch (currentView)
	{
	case Top:
		rotAngle = atan2(cp.z, rotStart * rotEnd);
		break;
	case Front:
		rotAngle = -atan2(cp.y, rotStart * rotEnd);
		break;
	case Right:
		rotAngle = atan2(cp.x, rotStart * rotEnd);
		break;
	}
	return rotAngle;
}


void Editor::selectAll()
{
	for (auto i = 0; i < Model->getVertex3BufferSize(); i++)
	{
		Model->selectVertex3byIndex(i);
	}
	for (auto i = 0; i < Model->getLine3BufferSize(); i++)
	{
		Model->selectLine3byIndex(i);
	}
	clicksInQueue = 0;
}


void Editor::deselectAll()
{
	for (auto i = 0; i < Model->getVertex3BufferSize(); i++)
	{
		Model->deselectVertex3byIndex(i);
	}
	for (auto i = 0; i < Model->getLine3BufferSize(); i++)
	{
		Model->deselectLine3byIndex(i);
	}
	clicksInQueue = 0;
}


unsigned int Editor::nVertSelected()
{
	auto nVert = Model->getVertex3BufferSize();
	unsigned int nSelectedVert = 0;
	for (auto i = 0; i < nVert; i++)
		if (!Model->isVertex3Deleted(i) && Model->isVertex3Selected(i))
			nSelectedVert++;
	return nSelectedVert;
}


unsigned int Editor::nLineSelected()
{
	auto nLine = Model->getLine3BufferSize();
	unsigned int nSelectedLine = 0;
	for (auto i = 0; i < nLine; i++)
		if (!Model->isLine3Deleted(i) && Model->isLine3Selected(i))
			nSelectedLine++;
	return nSelectedLine;
}


void Editor::drawLine()
{
	worldCoord stretch = movementEnd - movementStart;
	if (this->isOrthoOn) { this->alignToAxis(&stretch); }
	line3 tempLine = { currentID, {movementStart, movementStart + stretch}, false, false };
	Model->addLine3(tempLine);
}


void Editor::deleteSelected()
{
	for (auto i = 0; i < Model->getVertex3BufferSize(); i++)
	{
		if (!Model->isVertex3Deleted(i) && Model->isVertex3Selected(i))
		{
			Model->deleteVertex3byIndex(i);
		}
	}
	for (auto i = 0; i < Model->getLine3BufferSize(); i++)
	{
		if (!Model->isLine3Deleted(i) && Model->isLine3Selected(i))
		{
			Model->deleteLine3byIndex(i);
		}
	}
}


void Editor::moveSelected()
{
	worldCoord move = movementEnd - movementStart;
	if (isOrthoOn) { this->alignToAxis(&move); }
	for (auto i = 0; i < Model->getVertex3BufferSize(); i++)
	{
		if (!Model->isVertex3Deleted(i) && Model->isVertex3Selected(i))
		{
			Model->moveVertex3byIndex(i, move);
		}
	}
	for (auto i = 0; i < Model->getLine3BufferSize(); i++)
	{
		if (!Model->isLine3Deleted(i) && Model->isLine3Selected(i))
		{
			Model->moveLine3byIndex(i, move);
		}
	}
}


void Editor::copyMoveSelected()
{
	worldCoord move = movementEnd - movementStart;
	if (isOrthoOn) { this->alignToAxis(&move); }

	unsigned int nSelectedVert = this->nVertSelected();
	std::unique_ptr<vertex3[]> tempVerts(new vertex3[nSelectedVert]);
	unsigned int vCount = 0;
	for (auto i = 0; i < Model->getVertex3BufferSize(); i++)
	{
		if (!Model->isVertex3Deleted(i) && Model->isVertex3Selected(i))
		{
			tempVerts[vCount].deleted = false;
			tempVerts[vCount].selected = true;
			tempVerts[vCount].id = currentID;
			tempVerts[vCount].pos = Model->getVertex3(i).pos;

			tempVerts[vCount].pos += move;

			vCount++;
		}
	}
	for (unsigned int i = 0; i < nSelectedVert; i++)
		Model->addVertex3(tempVerts[i]);

	unsigned int nSelectedLine = this->nLineSelected();
	std::unique_ptr<line3[]> tempLines(new line3[nSelectedLine]);
	unsigned int lCount = 0;
	for (auto i = 0; i < Model->getLine3BufferSize(); i++)
	{
		if (!Model->isLine3Deleted(i) && Model->isLine3Selected(i))
		{
			tempLines[lCount].deleted = false;
			tempLines[lCount].selected = true;
			tempLines[lCount].id = currentID;
			tempLines[lCount].vert[0] = Model->getLine3(i).vert[0];
			tempLines[lCount].vert[1] = Model->getLine3(i).vert[1];

			tempLines[lCount].vert[0] += move;
			tempLines[lCount].vert[1] += move;

			lCount++;
		}
	}
	for (unsigned int i = 0; i < nSelectedLine; i++)
		Model->addLine3(tempLines[i]);
}


void Editor::rotateSelected()
{
	worldCoord startVect = unitVector2(rotationStart - rotationCentre);
	worldCoord endVect = unitVector2(rotationEnd - rotationCentre);
	rotationAngle = calculateAngle(startVect, endVect);
	for (auto i = 0; i < Model->getVertex3BufferSize(); i++)
	{
		if (!Model->isVertex3Deleted(i) && Model->isVertex3Selected(i))
		{
			Model->rotVertex3byIndex(i, currentView, rotationCentre, rotationAngle);
		}
	}
	for (auto i = 0; i < Model->getLine3BufferSize(); i++)
	{
		if (!Model->isLine3Deleted(i) && Model->isLine3Selected(i))
		{
			Model->rotLine3byIndex(i, currentView, rotationCentre, rotationAngle);
		}
	}
}


void Editor::copyRotateSelected()
{
	worldCoord startVect = unitVector2(rotationStart - rotationCentre);
	worldCoord endVect = unitVector2(rotationEnd - rotationCentre);
	rotationAngle = calculateAngle(startVect, endVect);

	unsigned int nSelectedVert = this->nVertSelected();
	std::unique_ptr<vertex3[]> tempVerts(new vertex3[nSelectedVert]);
	unsigned int vCount = 0;
	for (auto i = 0; i < Model->getVertex3BufferSize(); i++)
	{
		if (!Model->isVertex3Deleted(i) && Model->isVertex3Selected(i))
		{
			tempVerts[vCount].deleted = false;
			tempVerts[vCount].selected = true;
			tempVerts[vCount].id = currentID;
			tempVerts[vCount].pos = rotate2(Model->getVertex3(i).pos, currentView, rotationCentre, rotationAngle);

			vCount++;
		}
	}
	for (unsigned int i = 0; i < nSelectedVert; i++)
		Model->addVertex3(tempVerts[i]);

	unsigned int nSelectedLine = this->nLineSelected();
	std::unique_ptr<line3[]> tempLines(new line3[nSelectedLine]);
	unsigned int lCount = 0;
	for (auto i = 0; i < Model->getLine3BufferSize(); i++)
	{
		if (!Model->isLine3Deleted(i) && Model->isLine3Selected(i))
		{
			tempLines[lCount].deleted = false;
			tempLines[lCount].selected = true;
			tempLines[lCount].id = currentID;
			tempLines[lCount].vert[0] = rotate2(Model->getLine3(i).vert[0], currentView, rotationCentre, rotationAngle);
			tempLines[lCount].vert[1] = rotate2(Model->getLine3(i).vert[1], currentView, rotationCentre, rotationAngle);

			lCount++;
		}
	}
	for (unsigned int i = 0; i < nSelectedLine; i++)
		Model->addLine3(tempLines[i]);
}


void Editor::quickSave()
{
	Model->exportTextFile();
}