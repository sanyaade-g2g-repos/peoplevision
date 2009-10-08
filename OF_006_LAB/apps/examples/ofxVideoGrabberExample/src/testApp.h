#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxVideoGrabber.h"

//comment this out to use a normal USB cam
#define USING_OFX_VIDEOGRABBER

class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void calculateCaptureFramerate();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

	#ifdef USING_OFX_VIDEOGRABBER
		ofxVideoGrabber 		vidGrabber;
	#else
		ofVideoGrabber			vidGrabber;
	#endif
	
		int 				camWidth;
		int 				camHeight;
		int                 appWidth;
		int                 appHeight;

        /*framerate display*/
		char                buf[255];
		char                buf2[255];
		float 	            mytimeNow, mytimeThen, myframeRate;
		float 	            myfps;
		float                 myframes;
};

#endif
