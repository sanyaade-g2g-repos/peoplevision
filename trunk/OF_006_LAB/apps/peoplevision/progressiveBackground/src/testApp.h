#ifndef _TEST_APP
#define _TEST_APP

#define _USE_LIVE_VIDEO

/******************************************************************
	TRACKING + FILTER SCRIPT FROM NUI CCV
******************************************************************/

#include "ofMain.h"
#include "ofxOpenCv.h"

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
			ofVideoGrabber 		vidGrabber;
		#else
			ofVideoPlayer 		vidPlayer;
		#endif
	
		ofxCvColorImage		colorImg;
	
		ofxCvGrayscaleImage 	grayImage;
	
		ofxCvGrayscaleImage grayBg;
		ofxCvGrayscaleImage subtractBg;
		ofxCvGrayscaleImage grayDiff;
		ofxCvShortImage		floatBgImg;
	
		ofxCvContourFinder 	contourFinder;
	
		int 				threshold;
		bool				bLearnBakground;
		bool				bLearnBakgroundProgressive;
		bool				bTrackDark;
		float				fLearnRate;
		float camWidth, camHeight;
};

#endif
	
