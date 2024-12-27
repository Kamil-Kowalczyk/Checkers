#include "Direction.h"

void incrementDestinationCoordinates(Direction direction, int& row, int& col) {
	switch (direction) {
	case TOP_LEFT:
		row -= 1;
		col -= 1;
		break;
	case TOP_RIGHT:
		row -= 1;
		col += 1;
		break;
	case BOTTOM_LEFT:
		row += 1;
		col -= 1;
		break;
	case BOTTOM_RIGHT:
		row += 1;
		col += 1;
		break;
	}
}

Direction getCounterDirection(Direction direction) {
	Direction counterDirection = NONE;
	switch (direction) {
	case TOP_LEFT:
		counterDirection = BOTTOM_RIGHT;
		break;
	case TOP_RIGHT:
		counterDirection = BOTTOM_LEFT;
		break;
	case BOTTOM_LEFT:
		counterDirection = TOP_RIGHT;
		break;
	case BOTTOM_RIGHT:
		counterDirection = TOP_LEFT;
		break;
	}
	return counterDirection;
}