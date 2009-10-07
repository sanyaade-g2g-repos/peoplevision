#ifndef _TEST_APP
#define _TEST_APP

// 8/8/2008 Converted by Joel Gethin Lewis to OF 0.05 and OS X
// http://www.joelgethinlewis.com
// Based completely on the original code by Takashi Maekawa and Satoru Higa.
// Downloaded from Google Code via CVS:
// http://code.google.com/p/of-computervision/
// cvOpFlowBMExample by Takashi Maekawa and Satoru higa.
// An usage of example cvOpFlowBM class.

#include "ofMain.h"
#include "ofxCvMain.h"

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
		
		ofxCvColorImage cameraImg;
		ofxCvGrayscaleImage grayImg;
		ofxCvGrayscaleImage pastImg;
		ofxCvOpticalFlowBM	opticalFlow;
		
		int rows, cols;
		int block_size;
		int shift_size;
		int cw, ch;
		
		CvMat *velx, *vely;
		CvSize block;
		CvSize shift;
		CvSize max_range;
		
		ofVideoGrabber vidGrabber;
};

#endif
	
/* Original from Takashi

 //
 // cvOpFlowBMExample by Takashi Maekawa and Satoru higa.
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
 
 void keyPressed(int key);
 void keyReleased(int key);
 void mouseMoved(int x, int y );
 void mouseDragged(int x, int y, int button);
 void mousePressed(int x, int y, int button);
 void mouseReleased();
 
 ofCvColorImage cameraImg;
 ofCvGrayscaleImage grayImg;
 ofCvGrayscaleImage pastImg;
 ofCvOpticalFlowBM	opticalFlow;
 
 int rows, cols;
 int block_size;
 int shift_size;
 int cw, ch;
 
 CvMat *velx, *vely;
 CvSize block;
 CvSize shift;
 CvSize max_range;
 
 ofVideoGrabber vidGrabber;
 };
 
 #endif	

*/