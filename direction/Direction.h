#pragma once

/*
* An enum which defines the directions of the move
*/
enum Direction {
	//  1 row beneath and 1 col to the right
	TOP_RIGHT,

	// 1 row beneath and 1 col to the left
	TOP_LEFT,

	// 1 row udner and 1 col to the right
	BOTTOM_RIGHT, 

	//	1 row under and 1 col to the left
	BOTTOM_LEFT,

	// no movement
	NONE
};

/*
* Sets coordinates for variables based on the passed direction to achieve
* @param direction - direction of incrementation / decrementation
* @param row - a variable which will be incremented / decremented
* @param col - a variable which will be incremented / decremented
*/
void incrementDestinationCoordinates(Direction direction, int& row, int& col);


/*
* Returns a direction which is diagonally opposite to the one passed to the function
* for example: TOP_RIGHT -> BOTTOM_LEFT, BOTTOM_RIGHT -> TOP_LEFT
* @param direction - an actual direction which counter direction should be returned
* @return a counter direction
*/
Direction getCounterDirection(Direction direction);