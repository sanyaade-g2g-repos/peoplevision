#ifndef _TEST_APP
#define _TEST_APP

#define _USE_LIVE_VIDEO

#include "ofMain.h"
#include "ofxOpenCv.h"

#define CV_HAAR_FIND_MODE CV_HAAR_FIND_BIGGEST_OBJECT

#include "ofxCvHaarTracker.h"
#include "ofxSimpleGuiToo.h"
#include "CPUImageFilter.h"
#include "ofxPerson.h"

#ifdef _USE_LIVE_VIDEO

//	look in ofxVideoGrabber to find out about more formats
//	comment this out if you just want to use the normal quicktime stuff

//	NOTE: WE MAY NEED MORE STUFF TO MAKE THIS WIN COMPATIBLE
//	ofxVideoGrabber has some parts that will work, but if you want to
//	use the normal ofVideoGrabber with Win, we'll need to incorporate
//	utils from CCV for PS3 eye, etc

//#define USING_OFX_VIDEOGRABBER
//#define BLOB_TRACKER_VIDEO_FORMAT VID_FORMAT_GREYSCALE
//#define BLOB_TRACKER_COLOR_FORMAT VID_FORMAT_GREYSCALE

#ifdef USING_OFX_VIDEOGRABBER
	#include "ofxVideoGrabber.h"

	#ifndef BLOB_TRACKER_VIDEO_FORMAT
	#define BLOB_TRACKER_VIDEO_FORMAT VID_FORMAT_RGB 
	#endif

	#ifndef BLOB_TRACKER_COLOR_FORMAT
	#define BLOB_TRACKER_COLOR_FORMAT VID_FORMAT_RGB
	#endif
#endif

#endif

// display + calibration modes : press 'm' to cycle between them

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
	
	//CV images for detection + processing
	
	
	#ifdef USING_OFX_VIDEOGRABBER && #if BLOB_TRACKER_COLOR_FORMAT == VID_FORMAT_GREYSCALE
		
	// if you're capturing greyscale images from ofxVideoGrabber
	// you need to only look at greyscale images
	
		ofxCvGrayscaleImage		colorImg;
		ofxCvGrayscaleImage		colorSmallImage;
		ofxCvGrayscaleImage		colorBabyImage;
	
	#else
		ofxCvColorImage		colorImg;
		ofxCvColorImage		colorSmallImage;
		ofxCvColorImage		colorBabyImage;
	#endif
	
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
		int mode;
	
	//filter variables
	
		float fLearnRate;
		float camWidth, camHeight;
	
		int smooth;
		int highpassBlur;
		int highpassNoise;
		int	highpassAmp;
		int threshold;	
		int minBlob;
		int maxBlob;
	
	// haar
	
		float haarArea;
		float minHaarArea;
		float maxHaarArea;
		ofxCvHaarFinder			haarFinder;
		ofxCvHaarTracker		haarTracker;
	
	// optical flow
	
		ofxCvOpticalFlowLK	opticalFlow;
	
	// switches for filters
		
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
	
	// people vector
		
		vector <ofxPerson> peopleVector;

	//gui
	
		ofxSimpleGuiToo	gui;
};

#endif
	
