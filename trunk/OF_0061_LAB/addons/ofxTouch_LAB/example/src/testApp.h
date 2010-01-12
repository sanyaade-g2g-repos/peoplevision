
#define OF_ADDON_USING_OFXTOUCH

#include "ofMain.h"
#include "ofAddons.h"


class testApp : public ofxTouchApp {

  public:
  
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
    
    void fingerDragged( const ofxTouchFinger& finger );
    void fingerPressed( const ofxTouchFinger& finger );    
    void fingerReleased( const ofxTouchFinger& finger );
    
};

