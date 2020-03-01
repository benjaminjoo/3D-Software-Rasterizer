#include "Editor.h"


Editor::Editor(	float toler,
				std::shared_ptr<Camera> camera,
				std::shared_ptr < Canvas> screen,
				std::shared_ptr < ModelElementBuffer> buffer)
				:
				tolerance(toler),
				Cam(camera),
				Screen(screen),
				Model(buffer)
{
	std::cout << "Editor constructor called" << std::endl;
}


Editor::~Editor()
{
	std::cout << "Editor destructor called" << std::endl;
}


worldCoord Editor::screen2world(const screenCoord& Sc)
{
	worldCoord temp = { 0.0f, 0.0f, 0.0f };

	if (currentView == Side::Top)
	{
		temp.x = ((float)(Sc.x) / scale) + viewportCentre.x;
		temp.y = ((float)(Sc.y) / scale) + viewportCentre.y;
		temp.z = 0.0f;
	}
	else if (currentView == Side::Front)
	{
		temp.x = ((float)(Sc.x) / scale) + viewportCentre.x;
		temp.y = 0.0f;
		temp.z = ((float)(Sc.y) / scale) + viewportCentre.y;
	}
	else if (currentView == Side::Right)
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


screenCoord Editor::world2screen(const worldCoord& Wc)
{
	screenCoord temp = { 0, 0 };

	if (currentView == Side::Top)
	{
		temp.x = (int)(((Wc.x) - viewportCentre.x) * scale);
		temp.y = (int)(((Wc.y) - viewportCentre.y) * scale);
	}
	else if (currentView == Side::Front)
	{
		temp.x = (int)(((Wc.x) - viewportCentre.x) * scale);
		temp.y = (int)(((Wc.z) - viewportCentre.y) * scale);
	}
	else if (currentView == Side::Right)
	{
		temp.x = (int)(((Wc.y) - viewportCentre.x) * scale);
		temp.y = (int)(((Wc.z) - viewportCentre.y) * scale);
	}

	return temp;
}


void Editor::updateWorldPosition()
{
	if (currentView == Side::Top)
	{
		worldPosition.x = viewportCentre.x + (float)planPosition.x / scale;
		worldPosition.y = -(viewportCentre.y + (float)planPosition.y / scale);
		worldPosition.z = 0.0f;
	}
	else if (currentView == Side::Front)
	{
		worldPosition.x = viewportCentre.x + (float)frontPosition.x / scale;
		worldPosition.y = 0.0f;
		worldPosition.z = -(viewportCentre.y + (float)frontPosition.y / scale);
	}
	else if (currentView == Side::Right)
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

	if (currentView == Side::Top)
	{
		viewportCentre.x += (mouseBeforeZoom.x - mouseAfterZoom.x);
		viewportCentre.y += (mouseBeforeZoom.y - mouseAfterZoom.y);
	}
	else if (currentView == Side::Front)
	{
		viewportCentre.x += (mouseBeforeZoom.x - mouseAfterZoom.x);
		viewportCentre.y += (mouseBeforeZoom.z - mouseAfterZoom.z);
	}
	else if (currentView == Side::Right)
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

	screenCoord cursorPosition;
	switch (currentView)
	{
		case Side::Top:
			cursorPosition = planPosition;
		break;
		case Side::Front:
			cursorPosition = frontPosition;
		break;
		case Side::Right:
			cursorPosition = rightPosition;
		break;
	}

	Screen->drawMouseCursor(currentMode, cursorPosition, LIGHT_BLUE);
	
	for (auto i = 0; i < Model->getSpline3BufferSize(); i++)				//Highlight Spline control points
		if (!Model->isSpline3Deleted(i))
		{
			spline3 tempSpline = Model->getSpline3(i);
			for (auto& cp : tempSpline.ctrlPoints)
			{
				screenCoord tempCoordS = world2screen(cp.pos);
				if (abs(tempCoordS.x - cursorPosition.x) <= 10 && abs(tempCoordS.y - cursorPosition.y) <= 10)
					Screen->drawSpotHighlight(tempCoordS, YELLOW);
			}
		}

	if (isObjectSnapOn && this->snapToVert(&mouseBeforeZoom))
		Screen->drawSnapTarget(cursorPosition, WHITE);

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
				Screen->drawLine(tempStart, tempEnd, 1, BLUE);
		}
		if ((	currentMode == editingMode::Relocation ||
				currentMode == editingMode::CopyRelocation) &&
				clicksInQueue == 1)
		{
			if (Model->isLine3Selected(i) && !Model->isLine3Deleted(i))			//Draw all visible lines currently being moved
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
		if ((	currentMode == editingMode::Rotation ||
				currentMode == editingMode::CopyRotation) &&
				clicksInQueue == 2)
		{
			if (Model->isLine3Selected(i) && !Model->isLine3Deleted(i))			//Draw all visible lines currently being rotated
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


void Editor::updateTriangles()
{
	for (auto i = 0; i < Model->getTriangle3BufferSize(); i++)
	{
		if (!Model->isTriangle3Deleted(i))
		{
			triangle3 tempTriangle = Model->getTriangle3(i);
			screenCoord tempStart, tempEnd;

			if (currentMode == editingMode::TriangleDrawing)
			{
				for (int v = 0; v < 3; v++)
				{
					screenCoord vrt = world2screen(tempTriangle.vert[v].pos);
					if (tempTriangle.vert[v].selected)
						Screen->drawSpot(vrt, RED);
					else
						Screen->drawSpot(vrt, BLUE);
				}
			}
	
			if (tempTriangle.vertDrawn[0] &&
				tempTriangle.vertDrawn[1] &&
				!tempTriangle.vertDrawn[2])
			{
				tempStart = world2screen(tempTriangle.vert[0].pos);
				tempEnd = world2screen(tempTriangle.vert[1].pos);
				Screen->drawLine(tempStart, tempEnd, 1, BLUE);
			}

			if (tempTriangle.vertDrawn[0] &&
				tempTriangle.vertDrawn[1] &&
				tempTriangle.vertDrawn[2])
			{
				if ((currentMode == editingMode::Relocation ||
					currentMode == editingMode::CopyRelocation) &&
					clicksInQueue == 1)
				{
					worldCoord tempMove = movementStart - mouseBeforeZoom;
					if (isOrthoOn)
						this->alignToAxis(&tempMove);

					if (tempTriangle.vert[0].selected)
						tempTriangle.vert[0].pos -= tempMove;
					if (tempTriangle.vert[1].selected)
						tempTriangle.vert[1].pos -= tempMove;
					if (tempTriangle.vert[2].selected)
						tempTriangle.vert[2].pos -= tempMove;
				}
				for (int v = 0; v < 3; v++)
				{
					tempStart = world2screen(tempTriangle.vert[v].pos);
					if (v < 2)
						tempEnd = world2screen(tempTriangle.vert[v + 1].pos);
					else
						tempEnd = world2screen(tempTriangle.vert[0].pos);

					Screen->drawLine(tempStart, tempEnd, 1, BLUE);
				}
				for (int v = 0; v < 3; v++)
				{
					screenCoord vrt = world2screen(tempTriangle.vert[v].pos);
					if (tempTriangle.vert[v].selected)
						Screen->drawSpot(vrt, RED);
					else
						Screen->drawSpot(vrt, BLUE);
				}
			}
		}
	}
}


void Editor::updatePolylines()
{
	for (auto i = 0; i < Model->getPolyline3BufferSize(); i++)
	{
		if (!Model->isPolyline3Deleted(i))
		{
			polyline3 tempPolyline = Model->getPolyline3(i);

			for (auto& c : tempPolyline.ctrlPoints)
			{
				if (!c.deleted)
				{
					screenCoord cp = world2screen(c.pos);
					if (c.selected)
						Screen->drawSpot(cp, RED);
					else
						Screen->drawSpot(cp, BLUE);
				}

				if ((currentMode == editingMode::Relocation ||
					currentMode == editingMode::CopyRelocation) &&
					clicksInQueue == 1)
				{
					if (c.selected && !c.deleted)					//Draw all visible vertices currently being moved
					{
						worldCoord tempMove = movementStart - mouseBeforeZoom;
						if (isOrthoOn)
							this->alignToAxis(&tempMove);
						c.pos -= tempMove;
						screenCoord temp = world2screen(c.pos);
						Screen->drawSpot(temp, ORANGE);
					}
				}
			}

			if (tempPolyline.getSize() >= 2)
			{
				screenCoord tempStart;
				screenCoord tempEnd;
				bool startPointObtained;
				bool endPointObtained;
				int count = 0;
				for (unsigned lineCount = 0; lineCount < tempPolyline.ctrlPoints.size() - 1;)
				{
					startPointObtained = false;
					endPointObtained = false;

					for (int c = count; c < tempPolyline.ctrlPoints.size(); c++)
					{
						if (!tempPolyline.ctrlPoints[c].deleted)
						{
							tempStart = world2screen(tempPolyline.ctrlPoints[c].pos);
							startPointObtained = true;
							count++;
							break;
						}
					}

					for (int c = count; c < tempPolyline.ctrlPoints.size(); c++)
					{
						if (!tempPolyline.ctrlPoints[c].deleted)
						{
							tempEnd = world2screen(tempPolyline.ctrlPoints[c].pos);
							endPointObtained = true;
							count++;
							break;
						}
					}

					if (startPointObtained && endPointObtained)
					{
						Screen->drawLine(tempStart, tempEnd, 1, BLUE);
						lineCount++;
						count--;
					}
				}
			}
		}
	}
}


void Editor::updateSplines()
{
	for (auto i = 0; i < Model->getSpline3BufferSize(); i++)
	{
		if (!Model->isSpline3Deleted(i))
		{
			spline3 tempSpline = Model->getSpline3(i);

			for (auto& c : tempSpline.ctrlPoints)
			{
				if (!c.deleted)
				{
					screenCoord cp = world2screen(c.pos);
					if (c.selected)
						Screen->drawSpot(cp, RED);
					else
						Screen->drawSpot(cp, BLUE);
				}

				if ((currentMode == editingMode::Relocation ||
					currentMode == editingMode::CopyRelocation) &&
					clicksInQueue == 1)
				{
					if (c.selected && !c.deleted)					//Draw all visible control points currently being moved
					{
						worldCoord tempMove = movementStart - mouseBeforeZoom;
						if (isOrthoOn)
							this->alignToAxis(&tempMove);
						c.pos -= tempMove;
						screenCoord temp = world2screen(c.pos);
						Screen->drawSpot(temp, ORANGE);
					}
				}
			}

			if (tempSpline.getSize() > 3)
			{
				for (float t = 0.0f; t < static_cast<float>(tempSpline.getSize() - 3); t += 0.001f)
				{
					screenCoord spl = world2screen(tempSpline.getPoint(t));
					Screen->putPixel(spl.x, spl.y, WHITE);
				}

				//float step = 0.01f;
				//for (float t = 0.0f; t < static_cast<float>(tempSpline.getSize() - 3); t += step)
				//{
				//	screenCoord tempStart	= world2screen(tempSpline.getPoint(t));
				//	screenCoord tempEnd		= world2screen(tempSpline.getPoint(t + step));
				//	Screen->drawLine(tempStart, tempEnd, 1, WHITE);
				//}
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
				Screen->drawSpot(temp, RED);
			else
				Screen->drawSpot(temp, BLUE);
		}
		if ((	currentMode == editingMode::Relocation ||
				currentMode == editingMode::CopyRelocation) &&
				clicksInQueue == 1)
		{
			if (Model->isVertex3Selected(i) && !Model->isVertex3Deleted(i))					//Draw all visible vertices currently being moved
			{
				vertex3 tempVert = Model->getVertex3(i);
				worldCoord tempMove = movementStart - mouseBeforeZoom;
				if (isOrthoOn)
					this->alignToAxis(&tempMove);
				tempVert.pos -= tempMove;
				screenCoord temp = world2screen(tempVert.pos);
				Screen->drawSpot(temp, ORANGE);
			}
		}
		if ((	currentMode == editingMode::Rotation ||
				currentMode == editingMode::CopyRotation) &&
				clicksInQueue == 2)		//Draw all visible vertices currently being rotated
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
	if (currentMode == editingMode::Relocation ||
		currentMode == editingMode::CopyRelocation)								//Hint line of movement
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

	if (currentMode == editingMode::Rotation ||
		currentMode == editingMode::CopyRotation)								//Hint rotation angle
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

	if (currentMode == editingMode::LineDrawing ||
		currentMode == editingMode::TriangleDrawing ||
		currentMode == editingMode::PolylineDrawing)								//Hint line of movement
	{
		if (clicksInQueue > 0)
		{
			worldCoord tempMouse = screen2world(mousePosition);
			worldCoord tempMove	= tempMouse - movementStart;
			if (isOrthoOn)
				this->alignToAxis(&tempMove);
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

	this->updateTriangles();

	this->updatePolylines();

	this->updateSplines();

	this->updateVertices();

	this->hintResult();

	this->drawIcons();
}


void Editor::drawIcons()
{
	arrowButton.displayIcon();
	crossButton.displayIcon();
	lineButton.displayIcon();
	triangleButton.displayIcon();
	polylineButton.displayIcon();
	splineButton.displayIcon();
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
	if (currentView == Side::Top)
	{
		planPosition.x = mx;
		planPosition.y = my;
		if (planPosition.x < 0) { planPosition.x = 0; }
		if (planPosition.x > EDITOR_WIDTH - 1) { planPosition.x = EDITOR_WIDTH - 1; }
		if (planPosition.y < 0) { planPosition.y = 0; }
		if (planPosition.y > EDITOR_HEIGHT - 1) { planPosition.y = EDITOR_HEIGHT - 1; }
	}
	else if (currentView == Side::Front)
	{
		frontPosition.x = mx;
		frontPosition.y = my;
		if (frontPosition.x < 0) { frontPosition.x = 0; }
		if (frontPosition.x > EDITOR_WIDTH - 1) { frontPosition.x = EDITOR_WIDTH - 1; }
		if (frontPosition.y < 0) { frontPosition.y = 0; }
		if (frontPosition.y > EDITOR_HEIGHT - 1) { frontPosition.y = EDITOR_HEIGHT - 1; }
	}
	else if (currentView == Side::Right)
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
	if (isObjectSnapOn)
		this->snapToVert(&P);
	if (X.y > 31)
	{
		if (currentMode == editingMode::Selection)
			handleSelection(X, P);
		if (currentMode == editingMode::Placement)
			handlePlacement(P);
		if (currentMode == editingMode::LineDrawing)
			handleLineDrawing(P);
		if (currentMode == editingMode::TriangleDrawing)
			handleTriangleDrawing(P);
		if (currentMode == editingMode::PolylineDrawing)
			handlePolylineDrawing(P);
		if (currentMode == editingMode::SplineDrawing)
			handleSplineDrawing(P);
		if (currentMode == editingMode::Relocation ||
			currentMode == editingMode::CopyRelocation)
			handleRelocation(P);
		if (currentMode == editingMode::Rotation ||
			currentMode == editingMode::CopyRotation)
			handleRotation(P);
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
			this->activateTriangleDrawing();
			break;
		case 4:
			this->activatePolylineDrawing();
			break;
		case 5:
			this->activateSplineDrawing();
			break;
		case 6:
			this->activateRelocation();
			break;
		case 7:
			this->activateRotation();
			break;

		case 8:
			this->activateTopView();
			break;
		case 9:
			this->activateFrontView();
			break;
		case 10:
			this->activateRightView();
			break;

		case 11:
			this->toggleObjectSnap();
			break;
		case 12:
			this->toggleGridSnap();
			break;

		default:
			this->activateSelection();
			break;
		}
	}
}


void Editor::handleSelection(const screenCoord& X, const worldCoord& P)
{
	handleVertexSelection(X);

	handleLineSelection(P);

	handleTriangleSelection(X);

	handlePolylineSelection(X);

	handleSplineSelection(X);
}


void Editor::handleVertexSelection(const screenCoord& X)
{
	for (auto i = 0; i < Model->getVertex3BufferSize(); i++)
	{
		if (!Model->isVertex3Deleted(i))
		{
			vertex3		tempVert = Model->getVertex3(i);
			worldCoord	tempCoordW = tempVert.pos;
			screenCoord tempCoordS = world2screen(tempCoordW);
			if (abs(tempCoordS.x - X.x) <= 10 && abs(tempCoordS.y - X.y) <= 10)
				Model->selectVertex3byIndex(i);
		}
	}
}


void Editor::handleLineSelection(const worldCoord& P)
{
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


void Editor::handleTriangleSelection(const screenCoord& X)
{
	for (auto i = 0; i < Model->getTriangle3BufferSize(); i++)
	{
		if (!Model->isTriangle3Deleted(i))
		{
			triangle3 tempTriangle = Model->getTriangle3(i);
			for (int j = 0; j < 3; j++)
			{
				screenCoord tempCoordS = world2screen(tempTriangle.vert[j].pos);
				if (abs(tempCoordS.x - X.x) <= 10 && abs(tempCoordS.y - X.y) <= 10)
					Model->selectTriangleVertex3byIndex(i, j);			
			}
		}
	}
}


void Editor::handlePolylineSelection(const screenCoord& X)
{
	for (auto i = 0; i < Model->getPolyline3BufferSize(); i++)
	{
		if (!Model->isPolyline3Deleted(i))
		{
			polyline3 tempPolyline = Model->getPolyline3(i);
			for (auto j = 0; j < tempPolyline.ctrlPoints.size(); j++)
			{
				vertex3 tempVert = tempPolyline.ctrlPoints[j];
				worldCoord	tempCoordW = tempVert.pos;
				screenCoord tempCoordS = world2screen(tempCoordW);
				if (abs(tempCoordS.x - X.x) <= 10 && abs(tempCoordS.y - X.y) <= 10)
					Model->selectPolylineControlVertex3byIndex(i, j);
			}
		}
	}
}


void Editor::handleSplineSelection(const screenCoord& X)
{
	for (auto i = 0; i < Model->getSpline3BufferSize(); i++)
	{
		if (!Model->isSpline3Deleted(i))
		{
			spline3 tempSpline = Model->getSpline3(i);
			for (auto j = 0; j < tempSpline.ctrlPoints.size(); j++)
			{
				vertex3 tempVert = tempSpline.ctrlPoints[j];
				worldCoord	tempCoordW = tempVert.pos;
				screenCoord tempCoordS = world2screen(tempCoordW);
				if (abs(tempCoordS.x - X.x) <= 10 && abs(tempCoordS.y - X.y) <= 10)
					Model->selectSplineControlVertex3byIndex(i, j);
			}
		}
	}
}


void Editor::handlePlacement(const worldCoord& P)
{
	worldCoord temp = P;
	vertex3 tempVert = { currentID, temp, false, false };
	Model->addVertex3(tempVert);
	currentID++;
}


void Editor::handleLineDrawing(const worldCoord& P)
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


void Editor::handleTriangleDrawing(const worldCoord& P)
{
	if (clicksInQueue == 0)
	{
		triangle3 tempTriangle;
		currentTrigID = currentID;
		currentID++;
		tempTriangle.id = currentTrigID;

		vertex3 tempVert;
		tempVert.id = currentID;
		currentID++;
		tempVert.pos = P;

		tempTriangle.vert[0] = tempVert;
		tempTriangle.vertDrawn[0] = true;

		Model->addTriangle3(tempTriangle);
		movementStart = P;
		clicksInQueue++;
	}
	else
	{
		vertex3 tempVert;
		tempVert.id = currentID;
		currentID++;
		tempVert.pos = P;

		Model->addTriangle3Vert(currentTrigID, clicksInQueue, tempVert);
		movementStart = P;
		clicksInQueue++;

		if (clicksInQueue == 3)
			clicksInQueue = 0;
	}
}


void Editor::handlePolylineDrawing(const worldCoord& P)
{
	if (clicksInQueue == 0)
	{
		polyline3 tempPolyline;
		currentPlineID = currentID;
		currentID++;
		tempPolyline.id = currentPlineID;

		vertex3 tempVert;
		tempVert.id = currentID;
		currentID++;
		tempVert.pos = P;

		tempPolyline.ctrlPoints.push_back(tempVert);

		Model->addPolyline3(tempPolyline);
		movementStart = P;
		clicksInQueue++;
	}
	else
	{
		vertex3 tempVert;
		tempVert.id = currentID;
		currentID++;
		tempVert.pos = P;

		Model->addPolyline3ControlPoint(currentPlineID, tempVert);
		movementStart = P;
		clicksInQueue++;
	}
}


void Editor::handleSplineDrawing(const worldCoord& P)
{
	if (clicksInQueue == 0)
	{
		spline3 tempSpline;
		currentSplineID = currentID;
		currentID++;
		tempSpline.id = currentSplineID;

		vertex3 tempVert;
		tempVert.id = currentID;
		currentID++;
		tempVert.pos = P;

		tempSpline.ctrlPoints.push_back(tempVert);

		Model->addSpline3(tempSpline);
		clicksInQueue++;
	}
	else
	{
		vertex3 tempVert;
		tempVert.id = currentID;
		currentID++;
		tempVert.pos = P;

		Model->addSpline3ControlPoint(currentSplineID, tempVert);
		clicksInQueue++;
	}
}


void Editor::handleRelocation(const worldCoord& P)
{
	if (clicksInQueue == 0)
	{
		movementStart = P;
		clicksInQueue++;
	}
	else
	{
		movementEnd = P;
		if (currentMode == editingMode::Relocation)
		{
			this->moveSelected();
		}
		else if (currentMode == editingMode::CopyRelocation)
		{
			this->copyMoveSelected();
		}
		clicksInQueue = 0;
	}
}


void Editor::handleRotation(const worldCoord& P)
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
		if (currentMode == editingMode::Rotation)
		{
			this->rotateSelected();
		}
		else if (currentMode == editingMode::CopyRotation)
		{
			this->copyRotateSelected();
		}
		clicksInQueue = 0;
	}
}


void Editor::activateSelection()
{
	currentTool = tool::arrow;
	currentMode = editingMode::Selection;
	arrowButton.turnOn();
	crossButton.turnOff();
	lineButton.turnOff();
	triangleButton.turnOff();
	polylineButton.turnOff();
	splineButton.turnOff();
	moveButton.turnOff();
	rotateButton.turnOff();

	clicksInQueue = 0;
}


void Editor::activatePlacement()
{
	currentTool = tool::cross;
	currentMode = editingMode::Placement;
	arrowButton.turnOff();
	crossButton.turnOn();
	lineButton.turnOff();
	triangleButton.turnOff();
	polylineButton.turnOff();
	splineButton.turnOff();
	moveButton.turnOff();
	rotateButton.turnOff();

	clicksInQueue = 0;
}


void Editor::activateLineDrawing()
{
	currentTool = tool::line;
	currentMode = editingMode::LineDrawing;
	arrowButton.turnOff();
	crossButton.turnOff();
	lineButton.turnOn();
	triangleButton.turnOff();
	polylineButton.turnOff();
	splineButton.turnOff();
	moveButton.turnOff();
	rotateButton.turnOff();

	clicksInQueue = 0;
}


void Editor::activateTriangleDrawing()
{
	currentTool = tool::triangle;
	currentMode = editingMode::TriangleDrawing;
	arrowButton.turnOff();
	crossButton.turnOff();
	lineButton.turnOff();
	triangleButton.turnOn();
	polylineButton.turnOff();
	splineButton.turnOff();
	moveButton.turnOff();
	rotateButton.turnOff();

	clicksInQueue = 0;
}


void Editor::activatePolylineDrawing()
{
	currentTool = tool::polyline;
	currentMode = editingMode::PolylineDrawing;
	arrowButton.turnOff();
	crossButton.turnOff();
	lineButton.turnOff();
	triangleButton.turnOff();
	polylineButton.turnOn();
	splineButton.turnOff();
	moveButton.turnOff();
	rotateButton.turnOff();

	clicksInQueue = 0;
}


void Editor::activateSplineDrawing()
{
	currentTool = tool::spline;
	currentMode = editingMode::SplineDrawing;
	arrowButton.turnOff();
	crossButton.turnOff();
	lineButton.turnOff();
	triangleButton.turnOff();
	polylineButton.turnOff();
	splineButton.turnOn();
	moveButton.turnOff();
	rotateButton.turnOff();

	clicksInQueue = 0;
}


void Editor::activateRelocation()
{
	currentTool = tool::move;
	currentMode = editingMode::Relocation;
	arrowButton.turnOff();
	crossButton.turnOff();
	lineButton.turnOff();
	triangleButton.turnOff();
	polylineButton.turnOff();
	splineButton.turnOff();
	moveButton.turnOn();
	rotateButton.turnOff();

	clicksInQueue = 0;
}


void Editor::activateRotation()
{
	currentTool = tool::rotate;
	currentMode = editingMode::Rotation;
	arrowButton.turnOff();
	crossButton.turnOff();
	lineButton.turnOff();
	triangleButton.turnOff();
	polylineButton.turnOff();
	splineButton.turnOff();
	moveButton.turnOff();
	rotateButton.turnOn();

	clicksInQueue = 0;
}


void Editor::activateCopyRelocation()
{
	currentTool = tool::copy_move;
	currentMode = editingMode::CopyRelocation;
	arrowButton.turnOff();
	crossButton.turnOff();
	lineButton.turnOff();
	triangleButton.turnOff();
	polylineButton.turnOff();
	splineButton.turnOff();
	moveButton.turnOff();
	rotateButton.turnOff();

	clicksInQueue = 0;
}


void Editor::activateCopyRotation()
{
	currentTool = tool::copy_rotate;
	currentMode = editingMode::CopyRotation;
	arrowButton.turnOff();
	crossButton.turnOff();
	lineButton.turnOff();
	triangleButton.turnOff();
	polylineButton.turnOff();
	splineButton.turnOff();
	moveButton.turnOff();
	rotateButton.turnOff();

	clicksInQueue = 0;
}


void Editor::activateTopView()
{
	currentView = Side::Top;
	topViewButton.turnOn();
	frontViewButton.turnOff();
	sideViewButton.turnOff();
}


void Editor::activateFrontView()
{
	currentView = Side::Front;
	topViewButton.turnOff();
	frontViewButton.turnOn();
	sideViewButton.turnOff();
}


void Editor::activateRightView()
{
	currentView = Side::Right;
	topViewButton.turnOff();
	frontViewButton.turnOff();
	sideViewButton.turnOn();
}


void Editor::toggleObjectSnap()
{
	isObjectSnapOn = isObjectSnapOn ? false : true;
	if (isObjectSnapOn)
		objSnapButton.turnOn();
	else
		objSnapButton.turnOff();
}


void Editor::toggleGridSnap()
{
	isGridSnapOn = isGridSnapOn ? false : true;
	if (isGridSnapOn)
		grdSnapButton.turnOn();
	else
		grdSnapButton.turnOff();
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
	case Side::Top:
	{
		maxExtent = abs(V->x); greatest = 'x';
		if (abs(V->y) > maxExtent) { maxExtent = abs(V->y); greatest = 'y'; }
		if (greatest == 'x') { V->y = 0.0f; }
		else if (greatest == 'y') { V->x = 0.0f; }
	}
	break;
	case Side::Front:
	{
		maxExtent = abs(V->x); greatest = 'x';
		if (abs(V->z) > maxExtent) { maxExtent = abs(V->z); greatest = 'z'; }
		if (greatest == 'x') { V->z = 0.0f; }
		else if (greatest == 'z') { V->x = 0.0f; }
	}
	break;
	case Side::Right:
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
	case Side::Top:
		V->z = 0.0f;
		break;
	case Side::Front:
		V->y = 0.0f;
		break;
	case Side::Right:
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
	case Side::Top:
		rotAngle = atan2(cp.z, rotStart * rotEnd);
		break;
	case Side::Front:
		rotAngle = -atan2(cp.y, rotStart * rotEnd);
		break;
	case Side::Right:
		rotAngle = atan2(cp.x, rotStart * rotEnd);
		break;
	}
	return rotAngle;
}


void Editor::selectAll()
{
	for (auto i = 0; i < Model->getVertex3BufferSize(); i++)
		Model->selectVertex3byIndex(i);

	for (auto i = 0; i < Model->getLine3BufferSize(); i++)
		Model->selectLine3byIndex(i);

	for (auto i = 0; i < Model->getTriangle3BufferSize(); i++)
		if (!Model->isTriangle3Deleted(i))
			for (int j = 0; j < 3; j++)
				Model->selectTriangleVertex3byIndex(i, j);

	for (auto i = 0; i < Model->getPolyline3BufferSize(); i++)
		if (!Model->isPolyline3Deleted(i))
		{
			polyline3 tempPolyline = Model->getPolyline3(i);
			for (auto j = 0; j < tempPolyline.ctrlPoints.size(); j++)
				Model->selectPolylineControlVertex3byIndex(i, j);
		}

	for (auto i = 0; i < Model->getSpline3BufferSize(); i++)
		if (!Model->isSpline3Deleted(i))
		{
			spline3 tempSpline = Model->getSpline3(i);
			for (auto j = 0; j < tempSpline.ctrlPoints.size(); j++)
				Model->selectSplineControlVertex3byIndex(i, j);
		}

	clicksInQueue = 0;
}


void Editor::deselectAll()
{
	for (auto i = 0; i < Model->getVertex3BufferSize(); i++)
		Model->deselectVertex3byIndex(i);

	for (auto i = 0; i < Model->getLine3BufferSize(); i++)
		Model->deselectLine3byIndex(i);

	for (auto i = 0; i < Model->getTriangle3BufferSize(); i++)
		if (!Model->isTriangle3Deleted(i))
			for (int j = 0; j < 3; j++)
				Model->deselectTriangleVertex3byIndex(i, j);

	for (auto i = 0; i < Model->getPolyline3BufferSize(); i++)
		if (!Model->isPolyline3Deleted(i))
		{
			polyline3 tempPolyline = Model->getPolyline3(i);
			for (auto j = 0; j < tempPolyline.ctrlPoints.size(); j++)
				Model->deselectPolylineControlVertex3byIndex(i, j);
		}

	for (auto i = 0; i < Model->getSpline3BufferSize(); i++)
		if (!Model->isSpline3Deleted(i))
		{
			spline3 tempSpline = Model->getSpline3(i);
			for (auto j = 0; j < tempSpline.ctrlPoints.size(); j++)
				Model->deselectSplineControlVertex3byIndex(i, j);
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
	if (this->isOrthoOn)
		this->alignToAxis(&stretch);
	line3 tempLine = { currentID, {movementStart, movementStart + stretch}, false, false };
	Model->addLine3(tempLine);
}


void Editor::deleteSelected()
{
	for (auto i = 0; i < Model->getVertex3BufferSize(); i++)
		if (!Model->isVertex3Deleted(i) && Model->isVertex3Selected(i))
			Model->deleteVertex3byIndex(i);

	for (auto i = 0; i < Model->getLine3BufferSize(); i++)
		if (!Model->isLine3Deleted(i) && Model->isLine3Selected(i))
			Model->deleteLine3byIndex(i);

	for (auto i = 0; i < Model->getPolyline3BufferSize(); i++)
	{
		polyline3 tempPolyline = Model->getPolyline3(i);
		for (auto j = 0; j < tempPolyline.ctrlPoints.size(); j++)
			if (!tempPolyline.ctrlPoints[j].deleted && tempPolyline.ctrlPoints[j].selected)
				Model->deletePolylineControlVertex3byIndex(i, j);
	}

	for (auto i = 0; i < Model->getSpline3BufferSize(); i++)
	{
		spline3 tempSpline = Model->getSpline3(i);
		for (auto j = 0; j < tempSpline.ctrlPoints.size(); j++)
			if (!tempSpline.ctrlPoints[j].deleted && tempSpline.ctrlPoints[j].selected)
				Model->deleteSplineControlVertex3byIndex(i, j);
	}
}


void Editor::moveSelected()
{
	worldCoord move = movementEnd - movementStart;
	if (isOrthoOn)
		this->alignToAxis(&move);

	for (auto i = 0; i < Model->getVertex3BufferSize(); i++)
		if (!Model->isVertex3Deleted(i) && Model->isVertex3Selected(i))
			Model->moveVertex3byIndex(i, move);

	for (auto i = 0; i < Model->getLine3BufferSize(); i++)
		if (!Model->isLine3Deleted(i) && Model->isLine3Selected(i))
			Model->moveLine3byIndex(i, move);

	for (auto i = 0; i < Model->getTriangle3BufferSize(); i++)
		if (!Model->isTriangle3Deleted(i))
			for (int j = 0; j < 3; j++)
				if (Model->getTriangle3(i).vert[j].selected)
					Model->moveTriangleVertex3byIndex(i, j, move);

	for (auto i = 0; i < Model->getPolyline3BufferSize(); i++)
		if (!Model->isPolyline3Deleted(i))
		{
			polyline3 tempPolyline = Model->getPolyline3(i);
			for (auto j = 0; j < tempPolyline.ctrlPoints.size(); j++)
			{
				vertex3 tempVert = tempPolyline.ctrlPoints[j];
				if (!tempVert.deleted && tempVert.selected)
					Model->movePolylineControlVertex3byIndex(i, j, move);
			}
		}

	for (auto i = 0; i < Model->getSpline3BufferSize(); i++)
		if (!Model->isSpline3Deleted(i))
		{
			spline3 tempSpline = Model->getSpline3(i);
			for (auto j = 0; j < tempSpline.ctrlPoints.size(); j++)
			{
				vertex3 tempVert = tempSpline.ctrlPoints[j];
				if (!tempVert.deleted && tempVert.selected)
					Model->moveSplineControlVertex3byIndex(i, j, move);
			}
		}
}


void Editor::copyMoveSelected()
{
	worldCoord move = movementEnd - movementStart;
	if (isOrthoOn)
		this->alignToAxis(&move);

	unsigned int nSelectedVert = this->nVertSelected();
	std::unique_ptr<vertex3[]> tempVerts(new vertex3[nSelectedVert]);
	unsigned int vCount = 0;

	for (auto i = 0; i < Model->getVertex3BufferSize(); i++)
		if (!Model->isVertex3Deleted(i) && Model->isVertex3Selected(i))
		{
			tempVerts[vCount].deleted = false;
			tempVerts[vCount].selected = true;
			tempVerts[vCount].id = currentID;
			tempVerts[vCount].pos = Model->getVertex3(i).pos;

			tempVerts[vCount].pos += move;

			vCount++;
		}

	for (unsigned int i = 0; i < nSelectedVert; i++)
		Model->addVertex3(tempVerts[i]);

	unsigned int nSelectedLine = this->nLineSelected();
	std::unique_ptr<line3[]> tempLines(new line3[nSelectedLine]);
	unsigned int lCount = 0;

	for (auto i = 0; i < Model->getLine3BufferSize(); i++)
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

	for (unsigned int i = 0; i < nSelectedLine; i++)
		Model->addLine3(tempLines[i]);
}


void Editor::rotateSelected()
{
	worldCoord startVect = unitVector2(rotationStart - rotationCentre);
	worldCoord endVect = unitVector2(rotationEnd - rotationCentre);
	rotationAngle = calculateAngle(startVect, endVect);

	for (auto i = 0; i < Model->getVertex3BufferSize(); i++)
		if (!Model->isVertex3Deleted(i) && Model->isVertex3Selected(i))
			Model->rotVertex3byIndex(i, currentView, rotationCentre, rotationAngle);

	for (auto i = 0; i < Model->getLine3BufferSize(); i++)
		if (!Model->isLine3Deleted(i) && Model->isLine3Selected(i))
			Model->rotLine3byIndex(i, currentView, rotationCentre, rotationAngle);
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
		if (!Model->isVertex3Deleted(i) && Model->isVertex3Selected(i))
		{
			tempVerts[vCount].deleted = false;
			tempVerts[vCount].selected = true;
			tempVerts[vCount].id = currentID;
			tempVerts[vCount].pos = rotate2(Model->getVertex3(i).pos, currentView, rotationCentre, rotationAngle);

			vCount++;
		}

	for (unsigned int i = 0; i < nSelectedVert; i++)
		Model->addVertex3(tempVerts[i]);

	unsigned int nSelectedLine = this->nLineSelected();
	std::unique_ptr<line3[]> tempLines(new line3[nSelectedLine]);
	unsigned int lCount = 0;

	for (auto i = 0; i < Model->getLine3BufferSize(); i++)
		if (!Model->isLine3Deleted(i) && Model->isLine3Selected(i))
		{
			tempLines[lCount].deleted = false;
			tempLines[lCount].selected = true;
			tempLines[lCount].id = currentID;
			tempLines[lCount].vert[0] = rotate2(Model->getLine3(i).vert[0], currentView, rotationCentre, rotationAngle);
			tempLines[lCount].vert[1] = rotate2(Model->getLine3(i).vert[1], currentView, rotationCentre, rotationAngle);

			lCount++;
		}

	for (unsigned int i = 0; i < nSelectedLine; i++)
		Model->addLine3(tempLines[i]);
}


void Editor::quickSave()
{
	Model->exportTextFile();
}