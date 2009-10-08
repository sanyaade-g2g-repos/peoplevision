#ifndef _TEST_APP
#define _TEST_APP

#define SAMPLE_WIDTH		640
#define SAMPLE_HEIGHT		480

#include "ofMain.h"
#include "PixelDifferencing.h"
//#include "ofxVideoGrabber.h"
#include "ofxOpenCv.h"

//comment this out to use a normal USB cam
//#define USING_OFX_VIDEOGRABBER

class testApp : public ofBaseApp{

	public:

		testApp();
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void resized(int w, int h);
	
		#ifdef USING_OFX_VIDEOGRABBER
			ofxVideoGrabber 		vidGrabber;
		#else
			ofVideoGrabber			vidGrabber;
		#endif
	
		int 				camWidth;
		int 				camHeight;
		int                 appWidth;
		int                 appHeight;
	
	ofxCvColorImage			colorLargeImage;
	ofxCvColorImage			colorSmallImage;
	ofxCvGrayscaleImage		grayLargeImage;
	ofxCvGrayscaleImage 	graySmallImage;
	ofxCvGrayscaleImage  bgImg;
	
	// images for frame differencing timer
	ofxCvGrayscaleImage grayPrev;
	ofxCvGrayscaleImage grayCurDiff;
	int diffThreshold;
		void checkDifferencing();
	float motionThreshold;
	bool learnedBackground; // whether it has grabbed a BG yet
	

};

#endif
