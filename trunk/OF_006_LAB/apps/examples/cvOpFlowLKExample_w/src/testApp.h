#ifndef _TEST_APP
#define _TEST_APP

// 8/8/2008 Converted by Joel Gethin Lewis to OF 0.05 and OS X.
// http://www.joelgethinlewis.com
// Based completely on the original code by Takashi Maekawa.
// Downloaded from Google Code via CVS:
// http://code.google.com/p/of-computervision/
// cvOpFlowLKExample by Takashi Maekawa.
// An usage of example cvOpFlowBM class.

#include "ofMain.h"
#include "ofxOpenCV.h"

class testApp : public ofSimpleApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed  (int key);
		void keyReleased (int key);
		
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();
		
		ofVideoGrabber 		vidGrabber;
		ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayLastImage;
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;
		ofxCvColorImage		colorImg;
		
		ofxCvOpticalFlowLK	opticalFlow;
		
		int 				threshold;
		bool				bLearnBakground;
};

#endif
	
/* Original from Takashi

 //
 // cvOpFlowLKExample by Takashi Maekawa.
 // An usage of example cvOpFlowBM class.
 //
 
 
 #ifndef _TEST_APP
 #define _TEST_APP
 
 #include "ofMain.h"
 #include "ofCvMain.h"
 
 class testApp : public ofSimpleApp{
 
 public:
 
 void setup();
 void update();
 void draw();
 
 void keyPressed  (int key);
 void mouseMoved(int x, int y );
 void mouseDragged(int x, int y, int button);
 void mousePressed(int x, int y, int button);
 void mouseReleased();
 
 ofVideoGrabber 		vidGrabber;
 ofCvGrayscaleImage 	grayImage;
 ofCvGrayscaleImage 	grayLastImage;
 ofCvGrayscaleImage 	grayBg;
 ofCvGrayscaleImage 	grayDiff;
 ofCvColorImage		colorImg;
 
 ofCvOpticalFlowLK	opticalFlow;
 
 int 				threshold;
 bool				bLearnBakground;
 
 
 };
 
 #endif
 
 
 */