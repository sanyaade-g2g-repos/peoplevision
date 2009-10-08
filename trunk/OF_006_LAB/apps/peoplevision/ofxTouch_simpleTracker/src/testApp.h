#define USING_OFX_VIDEOGRABBER
#define BLOB_TRACKER_VIDEO_FORMAT VID_FORMAT_GREYSCALE
#define BLOB_TRACKER_COLOR_FORMAT VID_FORMAT_GREYSCALE

#include "ofxTouchApp.h"
#include "ofMain.h"

class testApp : public ofBaseApp {

  public:
  
	ofxTouchApp touchHandler;
	
    bool bDragging;

    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased  (int key);

    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased();
    
    void fingerDragged( ofxTouchFinger & finger );
    void fingerPressed( ofxTouchFinger & finger );    
    void fingerReleased( ofxTouchFinger & finger );
    
};

