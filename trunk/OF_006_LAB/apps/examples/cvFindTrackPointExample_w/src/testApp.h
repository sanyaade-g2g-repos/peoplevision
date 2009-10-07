#ifndef _TEST_APP
#define _TEST_APP

// 8/8/2008 Converted by Joel Gethin Lewis to OF 0.05 and OS X
// http://www.joelgethinlewis.com
// Based completely on the original code by Takashi Maekawa and Satoru Higa.
// Downloaded from Google Code via CVS:
// http://code.google.com/p/of-computervision/
// Based on code Good Features Track by masayashi.
// http://wikiwiki.jp/masayashi/?OpenCV%2FGood%20Features%20to%20Track
// An usage of example cvFindTrackPoint class.

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
		
		int cw, ch;
		ofVideoGrabber vidGrabber;
};

#endif
	
/* Original from Takashi

 //
 // cvFindTrackPointExample by Takashi Maekawa and Satoru Higa.
 // Based on code Good Features Track by masayashi.
 //    http://wikiwiki.jp/masayashi/?OpenCV%2FGood%20Features%20to%20Track
 // An usage of example cvFindTrackPoint class.
 //
 
 #ifndef _TEST_APP
 #define _TEST_APP
 
 
 #include "ofMain.h"
 #include "ofCvMain.h"
 #include <vector>
 
 using namespace std;
 
 
 
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
 
 int cw, ch;
 ofVideoGrabber vidGrabber;
};

#endif	

*/