/*
 *  ofxBlob.h
 *  openFrameworks
 *
 *  Created by Brett "the Factor" Renfer on 7/22/09.
 *  Copyright 2009 Rockwell Group. All rights reserved.
 *
 */

//BASED ON ofxTouchBlob

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
 * A blob is a homogenous patch represented by a polygonal contour. 
 * Typically a blob tracker uses the contour to figure out the blob's
 * persistence and "upgrades" it with ids and other temporal 
 * information. 
 *
 */


#ifndef OFX_BLOB
#define OFX_BLOB

#include <vector>
#include "ofTypes.h"

class ofxBlob {
public:
	
    float area;
    float length;
    ofRectangle box;
    ofPoint center;
    
    bool hole;
    vector<ofPoint> contour;
	
     ofxBlob() {
        area = 0.0f;
        length = 0.0f;
        hole = false;        
    }
    
};


#endif


