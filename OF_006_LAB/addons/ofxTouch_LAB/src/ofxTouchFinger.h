/*
* Copyright 2008 NOR_/D <http://nortd.com>
*
* This file is part of ofxTouch.
* ofxTouch is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as 
* published by the Free Software Foundation, either version 3 of 
* the License, or (at your option) any later version. For details
* see <http://www.gnu.org/licenses/>.
*
* * *
* Stripped down ofxTouchTrackedBlob for use in user code.
*
*/


#ifndef OFX_TOUCH_FINGER
#define OFX_TOUCH_FINGER

#include "ofxTouchBlob.h"
#include "ofxTouchTrackedBlob.h"


class ofxTouchFinger {
  public:

    int id;
    int initialOrder;
    int x;
    int y;
	
	int lastX;
	int lastY;
	
    int width;
    int height;
	
	ofPoint scale;
	
    float radius;
	ofxTouchTrackedBlob blob;
    

    ofxTouchFinger() {
        id = -1;
        radius = 0.0f;
    }
    
    ofxTouchFinger( const ofxTouchBlob& b ) {
		blob = b;
        id = -1;
        initialOrder = -1;       
        x = b.center.x;
        y = b.center.y;
        width = b.box.width;
        height = b.box.height;
        if( b.box.width > b.box.height ) { radius = b.box.width/2.f; }
        else { radius = b.box.height/2.f; }
    }

    ofxTouchFinger( const ofxTouchTrackedBlob& b ) {
		blob = b;
        id = b.id;
        initialOrder = -1;
        set(b, 1.0f, 1.0f);
    }    

    void set( const ofxTouchTrackedBlob& b, float scaleMult=1.0f ) {
		blob = b;
		lastX = x;
		lastY = y;
		
        x = b.center.x*scaleMult;
        y = b.center.y*scaleMult;
        width = b.box.width*scaleMult;
        height = b.box.height*scaleMult;
        if( b.box.width > b.box.height ) { radius = (b.box.width/2.f)*scaleMult; }
        else { radius = (b.box.height/2.f)*scaleMult; }  
		
		scale.x = scaleMult;
		scale.y = scaleMult;
    }
	
	void set( const ofxTouchTrackedBlob& b, float xscale=1.0f, float yscale=1.0f ) {
		blob = b;
		lastX = x;
		lastY = y;
		
        x = b.center.x*xscale;
        y = b.center.y*yscale;
        width = b.box.width*xscale;
        height = b.box.height*yscale;
        if( b.box.width > b.box.height ) { radius = (b.box.width/2.f)*xscale; }
        else { radius = (b.box.height/2.f)*yscale; }  
		
		scale.x = xscale;
		scale.y = yscale;
    }
};


#endif


