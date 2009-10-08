/*
 *  PixelDifferencing.cpp
 *  openFrameworks
 *
 *  Created by Joshua Walton on 10/7/09.
 *  Copyright 2009 Lab at Rockwell Group. All rights reserved.
 *
 */

#include "PixelDifferencing.h"
#include "ofMain.h"

PixelDifferencing::PixelDifferencing() {

}

void PixelDifferencing::setVideo(ofVideoGrabber tGrabber) {
		//vGrabbber = tGrabber;
}

/*
void PixelDifferencing::checkDifferencing() {
		grayCurDiff.absDiff(grayPrev, grayLargeImage);   // curDiff is the difference between current and previous frame
		cvThreshold(grayCurDiff.getCvImage(), grayCurDiff.getCvImage(), diffThreshold, 255, CV_THRESH_TOZERO); // anything below diffThreshold, drop to zero (compensate for noise)
		int numPixelsChanged = grayCurDiff.countNonZeroInRegion(0, 0, WIDTH, HEIGHT); // this is how many pixels have changed that are above diffThreshold
			
		if (numPixelsChanged > motionThreshold && numPixelsChanged < motionThreshold + 10000) {
			overLayer->loadSet();
			currentState = 1;
			cout<<"CHANGED BECAUSE: "<<numPixelsChanged<<endl;
		}
		
		grayPrev = grayLargeImage; // save current frame for next loop
}
*/