/*
 *  ofxPersonTuioCursor.h
 *  peopleTracker_000
 *
 *  Created by Brett Renfer on 10/14/09.
 *  Copyright 2009 Rockwell Group. All rights reserved.
 *
 
 THIS NEEDS SOME WORK... TO INTEGRATE WITH & TUIO ISN'T 
 AS EASY AS IT SHOULD BE
 
 */

class ofxPersonTuioCursor {
public:
	float x, y, width, height, haarX, haarY, haarWidth, haarHeight;
	
	bool isAlive;      // is it alive this frame
	bool wasAlive;      // was it alive this frame
	bool moved;         // did it move this frame
	bool hasHaar;
	
	ofxPersonTuioCursor() {
		isAlive      = false;
		wasAlive   = false;
		moved      = false;
	}
};