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
* This class is used in conjunction with the blob tracker.
* The blob tracker can delegete events to any class that inherits
* from this interface.
*
*/

#ifndef OFX_TOUCHBLOBLISTENER
#define OFX_TOUCHBLOBLISTENER

#include "ofxTouchTrackedBlob.h"


class ofxTouchBlobListener {
  public:
  
    virtual void blobMoved( const ofxTouchTrackedBlob& b ) = 0;    
    virtual void blobOn( const ofxTouchTrackedBlob& b ) = 0;
    virtual void blobOff( const ofxTouchTrackedBlob& b ) = 0;
    
};


#endif