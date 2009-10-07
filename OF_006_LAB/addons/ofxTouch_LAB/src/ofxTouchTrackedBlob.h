/*
* Copyright 2008 Stefan Hechenberger
*
* This file is part of ofxTouch.
* ofxTouch is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as 
* published by the Free Software Foundation, either version 3 of 
* the License, or (at your option) any later version. For details
* see <http://www.gnu.org/licenses/>.
*
* * * * * * * * *
* This class represents a blob with inter-frame information.
* This includes a persistent id to assume a persistent identity over 
* time.
*
*/


#ifndef OFX_TOUCH_TRACKEDBLOB
#define OFX_TOUCH_TRACKEDBLOB

#include "ofxTouchBlob.h"

class ofxTouchTrackedBlob : public ofxTouchBlob {
  public:

    int id;
    
    ofPoint deltaLoc;
    ofPoint deltaLocTotal;
    ofPoint predictedPos;
    float deltaArea;
    
    
    // Used only by BlobTracker
    //
    bool markedForDeletion;
    int framesLeft;
    vector<float> error;
    vector<int> closest;  // ids of the closest points, sorted
    
    

    ofxTouchTrackedBlob() {
        id = -1;
        area = 0.0f;
        length = 0.0f;
        deltaArea = 0.0f;
        markedForDeletion = false;
        framesLeft = 0;
    }
    
    ofxTouchTrackedBlob( const ofxTouchBlob& b ) {
        area = b.area;
        length = b.length;
        box = b.box;
        center = b.center;
        hole = b.hole;
        contour = b.contour;

        id = -1;        
        deltaArea = 0.0f;
        markedForDeletion = false;
        framesLeft = 0;
    }
    
    


    int getLowestError() {
        int best=-1;
        float best_v=99999.0f;
        for( unsigned int i=0; i<error.size(); i++ ) {
            if( error[i] < best_v ) {
                best = i;
                best_v = error[i];
            }
        }
        return best;
    }


};


#endif


