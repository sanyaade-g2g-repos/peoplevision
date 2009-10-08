#include "testApp.h"


void testApp::setup() {	
	//hook touchapp events
	
	ofAddListener( touchHandler.fingerDragged, this, &testApp::fingerDragged);
	ofAddListener( touchHandler.fingerPressed, this, &testApp::fingerPressed);
	ofAddListener( touchHandler.fingerReleased, this, &testApp::fingerReleased);
	
	//setSimulationMode( true );  //uncomment this to use mouse simulation
	//setVideoPlayerMode( true );   //comment this out to run live from cam
	//setVideoPlayerFile("touchkit-fingers.mov");
	bDragging = false;
	cout<<"SET"<<endl;
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
	
	for( int i=0; i<touchHandler.fingers.size(); i++ ) {
		ofSetColor( 180, 0, 0, 100 );
		ofCircle( touchHandler.fingers[i].x, touchHandler.fingers[i].y, 2*touchHandler.fingers[i].radius );
		ofSetColor( 200, 0, 0, 255 );        
		ofCircle( touchHandler.fingers[i].x, touchHandler.fingers[i].y, 0.5*touchHandler.fingers[i].radius );
	}
	
	touchHandler.draw();
}


void testApp::keyPressed( int key ) {}
void testApp::keyReleased( int key ) {}

void testApp::mouseMoved( int x, int y ) {}	
void testApp::mouseDragged( int x, int y, int button ) {}
void testApp::mousePressed( int x, int y, int button ) {}
void testApp::mouseReleased() {}

void testApp::fingerDragged( ofxTouchFinger & finger ) {
	bDragging = true;
}
void testApp::fingerPressed( ofxTouchFinger & finger ) {}
void testApp::fingerReleased( ofxTouchFinger & finger ) {}
	

