#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxCYA.h"
//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
								// otherwise, we'll use a movie file


class testApp : public ofBaseApp, public ofxPersonListener {

	public:

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		void personEntered( ofxCYAPerson* newPerson, ofxCYAScene* scene );
		void personMoved( ofxCYAPerson* activePerson, ofxCYAScene* scene );
		void personWillLeave( ofxCYAPerson* leavingPerson, ofxCYAScene* scene );
		void personUpdated( ofxCYAPerson* updatedPerson, ofxCYAScene* scene );
	
        #ifdef _USE_LIVE_VIDEO
		  ofVideoGrabber 		vidGrabber;
		#else
		  ofVideoPlayer 		vidPlayer;
		#endif
		int camWidth, camHeight;

        ofxCvColorImage		colorImg;

//        ofxCvGrayscaleImage 	grayImage;
//		ofxCvGrayscaleImage 	grayBg;
//		ofxCvGrayscaleImage 	grayDiff;
//
//        ofxCvContourFinder 	contourFinder;
//
//		int 				threshold;
//		bool				bLearnBakground;


	ofxCYAPeopleTracker peopleTracker;
	
	//example test values
	int itestValue;
	float ftestValue;
	bool btestValue;
	
};

#endif
