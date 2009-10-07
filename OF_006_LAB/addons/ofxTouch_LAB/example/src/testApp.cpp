#include "testApp.h"


void testApp::setup() {
    setSetupMode( true );
    //setSimulationMode( true );  //uncomment this to use mouse simulation
    setVideoPlayerMode( true );   //comment this out to run live from cam
    setVideoPlayerFile("touchkit-fingers.mov");
    bDragging = false;
}


void testApp::update(){
	ofBackground( 0,0,0 );
}


void testApp::draw() {
    ofEnableAlphaBlending();
    
    if( bDragging ) {
        ofFill();
        bDragging = false;
    } else {
        ofNoFill();
    }
    
	for( int i=0; i<fingers.size(); i++ ) {
        ofSetColor( 180, 0, 0, 100 );
        ofCircle( fingers[i].x, fingers[i].y, 2*fingers[i].radius );
        ofSetColor( 200, 0, 0, 255 );        
        ofCircle( fingers[i].x, fingers[i].y, 0.5*fingers[i].radius );
	}
        
}


void testApp::keyPressed( int key ) {}
void testApp::keyReleased( int key ) {}

void testApp::mouseMoved( int x, int y ) {}	
void testApp::mouseDragged( int x, int y, int button ) {}
void testApp::mousePressed( int x, int y, int button ) {}
void testApp::mouseReleased() {}

void testApp::fingerDragged( const ofxTouchFinger& finger ) {
    bDragging = true;
}
void testApp::fingerPressed( const ofxTouchFinger& finger ) {}
void testApp::fingerReleased( const ofxTouchFinger& finger ) {}
    

