#ifndef _TEST_APP
#define _TEST_APP

#define _USE_LIVE_VIDEO

/******************************************************************
	TRACKING + FILTER SCRIPT FROM NUI CCV
******************************************************************/

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCvHaarTracker.h"
#include "ofxSimpleGuiToo.h"
#include "CPUImageFilter.h"

#ifdef _USE_LIVE_VIDEO

#define USING_OFX_VIDEOGRABBER
#define BLOB_TRACKER_VIDEO_FORMAT VID_FORMAT_GREYSCALE
#define BLOB_TRACKER_COLOR_FORMAT VID_FORMAT_GREYSCALE

#ifdef USING_OFX_VIDEOGRABBER
	#include "ofxVideoGrabber.h"

	#ifndef BLOB_TRACKER_VIDEO_FORMAT
	#define BLOB_TRACKER_VIDEO_FORMAT VID_FORMAT_YUV422
	#endif

	#ifndef BLOB_TRACKER_COLOR_FORMAT
	#define BLOB_TRACKER_COLOR_FORMAT VID_FORMAT_RGB
	#endif
#endif

#endif

#define MODE_NORMAL				0
#define MODE_GUI				1
#define MODE_CAMERA_CALIBRATE	2
#define MODE_FULLSCREEN			3

class testApp : public ofBaseApp{
	public:

		void setup();
		void update();
		void draw();
		
		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
			
		#ifdef _USE_LIVE_VIDEO
			#ifdef USING_OFX_VIDEOGRABBER
				ofxVideoGrabber vidGrabber;
			#else
				ofVideoGrabber vidGrabber;
			#endif
		#else
			ofVideoPlayer 		vidPlayer;
		#endif
	
	
#ifdef USING_OFX_VIDEOGRABBER
	ofxCvGrayscaleImage		colorImg;
	ofxCvGrayscaleImage		colorSmallImage;
#else
	ofxCvColorImage		colorImg;
	ofxCvColorImage		colorSmallImage;
#endif
	
		ofxCvGrayscaleImage	grayImage;
		ofxCvGrayscaleImage	grayLastImage;
	
		ofxCvGrayscaleImage grayBg;
		ofxCvGrayscaleImage subtractBg;
		ofxCvGrayscaleImage graySmallImage;
		CPUImageFilter		grayDiff;
		ofxCvShortImage		floatBgImg;
	
		ofxCvContourFinder 	contourFinder;
	
		int mode;
	
		int smooth;
		int highpassBlur;
		int highpassNoise;
		int	highpassAmp;
		int threshold;	
		int minBlob;
		int maxBlob;
	
		float haarArea;
		float minHaarArea;
		float maxHaarArea;
	
		float fLearnRate;
		float camWidth, camHeight;
		
		bool bFindHoles;
		bool bLearnBackground;
		bool bSmartLearnBackground;
		bool bLearnBackgroundProgressive;
		bool bTrackDark;
		bool bHighpass;
		bool bAmplify;
		bool bSmooth;
		bool bDetectHaar;
		bool bTrackOpticalFlow;
	
		ofxCvHaarFinder			haarFinder;
		ofxCvHaarTracker		haarTracker;
	
		ofxCvOpticalFlowLK	opticalFlow;
	
		ofxSimpleGuiToo	gui;
};

#endif
	
