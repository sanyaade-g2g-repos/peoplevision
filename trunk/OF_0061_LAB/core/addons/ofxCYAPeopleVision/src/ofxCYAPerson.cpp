/*
 *  ofxCYAPerson.cpp
 *  peopleTracker_000
 *
 *  Adapted by James George on 11/21/09.
 *  Copyright 2009 (http://www.jamesgeorge.org) All rights reserved.
 *
 */
#include "ofxCYAPerson.h"

#define MAX_HAAR_GHOSTFRAMES 15 //how long before we say it's really gone

ofxCYAPerson::ofxCYAPerson(int pid, int oid, ofxCvBlob blob)
: pid(pid),
  oid(oid),
  age(0),
  hasHaar(false),
  haarRect(ofRectangle(0,0,0,0)),
  opticalFlowVectorAccumulation(ofPoint(0,0)),
  centroid(blob.centroid),
  framesWithoutHaar(0),
  customAttributes(NULL)
{
	update(blob, false);
}

ofxCYAPerson::~ofxCYAPerson()
{
	if(customAttributes != NULL){
		free(customAttributes);
	}
}

void ofxCYAPerson::update(ofxCvBlob blob, bool dampen)
{
	if(dampen){
		centroid = (centroid * .7) + (blob.centroid * .3);
	}
	else{
		centroid = blob.centroid;
	}
	
	velocity	 = blob.centroid - centroid;
	area		 = blob.area;
	boundingRect = blob.boundingRect;
	contour		 = blob.pts; 
	age++;
}


void ofxCYAPerson::setHaarRect(ofRectangle _haarRect)
{
	haarRect = _haarRect;
	hasHaar = true;
	framesWithoutHaar = 0;
}

ofRectangle ofxCYAPerson::getHaarRect()
{
	if(!hasHaar){
		printf("ofxCYAPerson: accessing Haar rect without Haar!\n");
		return ofRectangle(0,0,0,0);
	}
	return haarRect;
}

bool ofxCYAPerson::hasHaarRect()
{
	return hasHaar;
}

void ofxCYAPerson::noHaarThisFrame()
{
	//temp remove keep haar
	hasHaar = false;
	return;
	
	//if we had a haar but it vanished move it by the movement of the blob
	if(framesWithoutHaar++ > MAX_HAAR_GHOSTFRAMES){
		hasHaar = false;
	}
	if(hasHaar){
		haarRect.x += velocity.x;
		haarRect.y += velocity.y;
	}
}

ofRectangle ofxCYAPerson::getBoundingRectNormalized(float videoWidth, float videoHeight)
{
		return ofRectangle(boundingRect.x/videoWidth,
						   boundingRect.y/videoHeight,
						   boundingRect.width/videoWidth,
						   boundingRect.height/videoHeight);
}

ofPoint ofxCYAPerson::getCentroidNormalized(float videoWidth, float videoHeight)
{
	return ofPoint(centroid.x / videoWidth, centroid.y / videoHeight);
}

ofPoint ofxCYAPerson::getHaarCentroidNormalized(float videoWidth, float videoHeight)
{
	ofRectangle haarRect = getHaarRect();
	return ofPoint( (haarRect.x + haarRect.width/2) / videoWidth, (haarRect.y + haarRect.height/2) / videoHeight );
}

