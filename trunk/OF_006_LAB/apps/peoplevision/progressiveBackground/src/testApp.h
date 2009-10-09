#ifndef _TEST_APP
#define _TEST_APP

#define _USE_LIVE_VIDEO

/******************************************************************
	TRACKING + FILTER SCRIPT FROM NUI CCV
******************************************************************/

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxSimpleGuiToo.h"
#include "CPUImageFilter.h"

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
	
		ofxCvGrayscaleImage	grayImage;
	
		ofxCvGrayscaleImage grayBg;
		ofxCvGrayscaleImage subtractBg;
		CPUImageFilter grayDiff;
		ofxCvShortImage		floatBgImg;
	
		ofxCvContourFinder 	contourFinder;
	
		int smooth;
		int highpassBlur;
		int highpassNoise;
		int	highpassAmp;
	
		int minBlob;
		int maxBlob;
		
		bool				bFindHoles;
		int 				threshold;
		bool				bLearnBakground;
		bool				bLearnBakgroundProgressive;
		bool				bTrackDark;
		bool				bHighpass;
		bool				bAmplify;
		bool				bSmooth;
		float				fLearnRate;
		float camWidth, camHeight;
		ofxSimpleGuiToo	gui;
};

#endif
	
