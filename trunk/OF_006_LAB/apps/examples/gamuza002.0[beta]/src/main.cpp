#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;
    bool            gMode = false; // for game_mode or window_mode

    if(gMode){
        ofSetupOpenGL(&window, 2464,900, OF_GAME_MODE);
    }else{
        // 1130x780 fit good starting from 1200x800(i.e. macbook)
        // or 1280x1024(normal 17 inch screens)
        ofSetupOpenGL(&window, 1130,780, OF_WINDOW);
    }

	ofRunApp(new testApp());

}
