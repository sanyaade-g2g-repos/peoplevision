/*
* Copyright 2008 NOR_/D <http://nortd.com>
*
* This file is part of ofxTouch.
* ofxTouch is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as 
* published by the Free Software Foundation, either version 3 of 
* the License, or (at your option) any later version. For details
* see <http://www.gnu.org/licenses/>.
*
* * *
* This class is currently used to relay events. Its main purpose
* is to make it possible to call touch specific code in ofxTouchApp
* before user code is called.
* 
* Once we add a proper (poco-based) event model to OF this
* class will be obsolete.
*/

#ifndef _OFX_TOUCHAPPRELAY
#define _OFX_TOUCHAPPRELAY


#include "ofBaseApp.h"
#include "ofxTouchApp.h"



class ofxTouchAppRelay : public ofSimpleApp {

  public:

    ofxTouchAppRelay(ofxTouchApp* _TouchAppPtr) : ofSimpleApp() {
        TouchAppPtr = _TouchAppPtr;
    }

    
    void setup(){ TouchAppPtr->_setup(); }
    void update(){ TouchAppPtr->_update(); }
    void draw(){ TouchAppPtr->_draw(); }
    void exit(){ TouchAppPtr->_exit(); }
    
    
    void keyPressed(int key ){ TouchAppPtr->_keyPressed(key); }
    void keyReleased(int key ){ TouchAppPtr->_keyReleased(key); }
    
    void mouseMoved( int x, int y ){ TouchAppPtr->_mouseMoved(x,y); }
    void mouseDragged( int x, int y, int button ){ TouchAppPtr->_mouseDragged(x,y,button); }
    void mousePressed( int x, int y, int button ){ TouchAppPtr->_mousePressed(x,y,button); }
    void mouseReleased(){ TouchAppPtr->_mouseReleased(); }

    //virtual void audioReceived 	(float * input, int bufferSize, int nChannels) {} 
    //virtual void audioRequested (float * output, int bufferSize, int nChannels) {}	

    
  private:
    ofxTouchApp* TouchAppPtr;

};

#endif
