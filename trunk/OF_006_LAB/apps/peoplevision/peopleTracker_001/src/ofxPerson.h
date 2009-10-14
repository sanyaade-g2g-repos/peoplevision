/***************************************************************************
 *
 *  ofxPerson.h
 *  Rockwell LAB + IDEO LAB peopleVision project
 * 
 *  Created by NUI Group Dev Team A on 3/1/09.
 *  Copyright 2009 NUI Group/Inc. All rights reserved.       
 *  Version : 1.20
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are 
 * met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer as
 * the first lines of this file unmodified.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY NUI GROUP ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL BEN WOODHOUSE BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 * Web: http://code.google.com/p/peoplevision/
 *
 * (...)
 *
 ***************************************************************************/

#ifndef OFX_PERSON_OBJECT
#define OFX_PERSON_OBJECT

#include "ofMain.h"

class ofxPerson : public ofRectangle
{
public: 
	float area;
	
	bool hasHaar;
	ofRectangle haarRect;
	/*float haarX;
	float haarY;
	float haarWidth;
	float haarHeight;
	*/
	
	bool hasOpticalFlow;
	ofPoint opticalFlowVelocity;

	ofPoint velocity;
	
	ofxPerson(){
		hasHaar = false;
		haarRect.x = haarRect.y = haarRect.width = haarRect.height = 0;
		hasOpticalFlow = false;
	};
	
	//get scaled points
	
	float getScaledX( int scaleWidth){
		return (float) x*scaleWidth;
	};
	
	float getScaledY( int scaleHeight){
		return (float) y*scaleHeight;
	};
	
	float getScaledWidth( int scaleWidth){
		return (float) width*scaleWidth;
	};
	
	float getScaledHeight( int scaleHeight ){
		return (float) height*scaleHeight;
	};
	
	float getScaledArea( int scaleWidth, int scaleHeight ){
		return (float) area*( scaleWidth * scaleHeight);
	};
	
	ofRectangle getScaled( int scaleWidth, int scaleHeight){
		ofRectangle scaled = ofRectangle(x,y,width,height);
		scaled.x		*= scaleWidth;
		scaled.y		*= scaleHeight;
		scaled.width	*= scaleWidth;
		scaled.height	*= scaleHeight;
		
		return scaled;
	};
	
	ofRectangle getScaledHaar( int scaleWidth, int scaleHeight){
		ofRectangle scaled = ofRectangle(haarRect.x,haarRect.y,haarRect.width,haarRect.height);
		scaled.x		*= scaleWidth;
		scaled.y		*= scaleHeight;
		scaled.width	*= scaleWidth;
		scaled.height	*= scaleHeight;
		
		return scaled;
	};
};

#endif