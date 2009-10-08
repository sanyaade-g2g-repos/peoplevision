/*
 *  PixelDifferencing.h
 *  openFrameworks
 *
 *  Created by Joshua Walton on 10/7/09.
 *  Copyright 2009 Lab at Rockwell Group. All rights reserved.
 *
 */

#ifndef PIXEL_DIFFERENCING
#define PIXEL_DIFFERENCING

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxVideoGrabber.h"


class PixelDifferencing {
	
public:
	PixelDifferencing();
	void setVideo(ofVideoGrabber tGrabber);
	void checkDifferencing();
	
	ofxCvColorImage			colorLargeImage;
	ofxCvColorImage			colorSmallImage;
	ofxCvGrayscaleImage		grayLargeImage;
	ofxCvGrayscaleImage 	graySmallImage;
	ofxCvGrayscaleImage		bgImg;
	
	// images for frame differencing timer
	ofxCvGrayscaleImage grayPrev;
	ofxCvGrayscaleImage grayCurDiff;
	
	ofVideoGrabber vGrabbber;
	
};

#endif