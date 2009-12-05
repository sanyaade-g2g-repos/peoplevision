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

/**
 * 11/23/2009
 * Refactored by James George (http://www.jamesgeorge.org)
 * integrated aspects of persistent person tracking and motion detection
 * from SNIFF project (http://www.gravitytrap.com/sniff/)
 * Added:
 *	added getters/setters for non-gui configuration
 *	instance persistent blob tracking
 *	delegate call backs
 *	finished tuio and optical flow implementation
 */

#ifndef _OFXPEOPLETRACKER_H
#define _OFXPEOPLETRACKER_H

#include "ofxCYAPerson.h"
#include "ofxOpenCv.h"
#include "CPUImageFilter.h"
#include "ofxCvOpticalFlowLK.h"

//haar setting
#define CV_HAAR_FIND_MODE CV_HAAR_FIND_BIGGEST_OBJECT

//dependent libraries
#include "ofxCvHaarTracker.h"
#include "ofxSimpleGuiToo.h"
#include "ofxCYATUIOSender.h"
#include "ofxCvBlobTracker.h"

#define DRAW_MODE_NORMAL				0
#define DRAW_MODE_GUI					1
#define DRAW_MODE_CAMERA_CALIBRATE		2
#define DRAW_MODE_FULLSCREEN			3

//interface for listener of people events
class ofxPersonListener {
public:
	//called when the person enters the system
    virtual void personEntered( int id ) = 0;
	//called each time the centroid moves (a lot)
	virtual void personMoved( int id ) = 0;
	//called one frame before the person is removed from the list to let you clean up
    virtual void personWillLeave(int id ) = 0;
};

class ofxCYAPeopleTracker : public ofxCvBlobListener {
	public:
		//set up and update
	
		void setup(int w, int h);				//Call during setup with camera width & height
		void update(ofxCvColorImage image);		//Call with sequential camera images
		void update(ofxCvGrayscaleImage image); //Call with sequential camera images
		
		//communication
		
		void setupTuio(string ip, int port);
		void setListener(ofxPersonListener* delegate);
	
		//tracking metrics
		
		void enableHaarFeatures(bool doHaar);
		void enableOpticalFlow(bool doOpticalFlow);
	
		// background learning and management
	
		void relearnBackground(); //one shot relearn trigger
		void enableBackgroundRelearnSmart(bool doSmartLearn);//auto-relearns if there are too many blobs in the scene
		void enableBackgroundReleaernProgressive(bool doProgressive); //relearns over time using progessive frame averagering
		void setRelearnRate(float relearnRate);
		
		//image control
	
		void setThreshold(float thresholdAmount);
		void setMinBlobSize(float minBlobSize); // % of screen
		void setMaxBlobSize(float maxBlobSize); // % of screen
	
		void enableSmooth(bool doSmooth);
		void setSmoothAmount(int smoothAmount);
	
		void enableHighpass(bool doHighpass);
		void setHighpassBlurAmount(int highpassBlurAmount);
		void setHighpassNoiseAmount(int highpassNoiseAmount);

		void enableAmplify(bool doAmp);
		void setAmplifyAmount(int amplifyAmount);
							   
		// filter controls

		//haar
		void setHaarXMLFile(string haarFile);
		void setHaarExpandArea(float haarExpandAmount); //makes the haar rect +area bigger
		void setMinHaarArea(float minArea);
		void setMaxHaarArea(float maxArea);
		void useHaarAsCentroid(bool useHaarCenter);
	
		//blobs
		void enableFindHoles(bool findHoles);
		void trackDarkBlobs();
		void trackLightBlobs();
		int getDrawMode();

		ofxCYAPerson personAtIndex(int i);
		ofxCYAPerson getTrackedPerson(int pid);
		int totalPeople();
	
	//drawing methods
	
		void setDrawMode(int mode);
		void draw();
		void draw(int x, int y);
		void draw(int x, int y, int mode);
	
	//callbacks for blob tracking
	
		void blobOn( int x, int y, int id, int order );
		void blobMoved( int x, int y, int id, int order );
		void blobOff( int x, int y, int id, int order );
	
	protected:
	
		void trackPeople();
		bool isTrackingPerson(int pid);

	
		vector <ofxCYAPerson> trackedPeople;
		
		ofxCvGrayscaleImage	grayImage;
		ofxCvGrayscaleImage	grayLastImage;
		
		ofxCvGrayscaleImage grayBg;
		ofxCvGrayscaleImage subtractBg;
	
		ofxCvGrayscaleImage graySmallImage;
		ofxCvGrayscaleImage grayBabyImage;
		
		//more specific CV images for processing
		
		CPUImageFilter		grayDiff;
		ofxCvShortImage		floatBgImg;
		
		// blob tracking things
		
		ofxCvContourFinder 	contourFinder;
		ofxCvBlobTracker persistentTracker;
		int drawMode;
		
		//filter variables
		
		float fLearnRate;
		float width, height;
		
		int smooth;
		int highpassBlur;
		int highpassNoise;
		int	highpassAmp;
		int threshold;	
		
		float minBlob;
		float maxBlob;
		
		// haar

		float haarArea;
		float minHaarArea;
		float maxHaarArea;
		ofxCvHaarFinder	 haarFinder;
		ofxCvHaarTracker haarTracker;
		
		// optical flow
		
		ofxCvOpticalFlowLK	opticalFlow;
		
		// switches for filters
		
		bool bFindHoles;
		bool bLearnBackground;
		bool bSmartLearnBackground;
		bool bLearnBackgroundProgressive;
		bool bHighpass;
		bool bAmplify;
		bool bSmooth;
		bool bDetectHaar;
		bool bUseHaarAsCenter;
		bool bTrackOpticalFlow;
		bool bTrackDark;
		bool bCentroidDampen;
	
		// communication
		
		ofxCYATUIOSender tuioClient;
		bool bTuioEnabled;
		ofxPersonListener* eventListener;
		
		//gui
		
		ofxSimpleGuiToo	gui;	
};
#endif
