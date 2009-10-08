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
* The base class of all filters.
*/

#ifndef OFX_TOUCHAFILTER
#define OFX_TOUCHAFILTER

#include "ofMain.h"
#include "ofxCvMain.h"


class ofxTouchFilter {
  public:
  
    int cwidth;
    int cheight;
	int threshold;
	bool bLearnBackground;     

	ofxTouchFilter(){
		threshold = 60;
	}
	
    virtual void allocate( int w, int h ) = 0;        
    virtual void apply( ofxCvGrayscaleImage& img ) = 0;    
    virtual void draw( int x, int y ) = 0;
    virtual void draw( int x, int y, int w, int h ) = 0;  
};


#endif
