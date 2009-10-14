#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(200, 200, 200);
	ofSetVerticalSync(true);
	
	//sends to localhost, port: 3333
	server.start("default", 0);
	server.setVerbose(true);
	
	xmlServer.initialize();
	
	//deg = 45.0;
	
	//just an example marker TUIO object
	//object = server.addObject(0, 100, 100, deg);
	cursor  = NULL;
}

//--------------------------------------------------------------
void testApp::update(){
	//send the messages (this is a little weird because of the way OF handles
	//mouse and keyboard input)
	server.run();
	
	//update the example marker Object
	//deg+=0.1;
	//server.updateObject(object, 100, 100, deg);
}

//--------------------------------------------------------------
void testApp::draw(){
	//draw current cursors and object
	
	//this stuff is what is creating the representation on screen
	
	server.drawCursors();
	//server.drawObjects();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

/* right click for sticky cursor, right click on a sticky cursor to delete it */

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if (cursor==NULL) return;
	if (cursor->getTuioTime()==server.getCurrentTime()) return;
	//server.updateCursor(cursor,x,y);
	xmlServer.updateCursor(cursor,x,y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
	TuioCursor * match = NULL;
	for (std::list<TuioCursor*>::iterator tuioCursor = stickyCursorList.begin(); tuioCursor!=stickyCursorList.end(); tuioCursor++) {
		float dist = (*tuioCursor)->getDistance((float)(x)/ofGetWidth(), (float)(y)/ofGetHeight());
		if (dist < (10.0/ofGetHeight())) {
			match = (*tuioCursor);
			break;
		}
	}
	if(button ==  2){
		if (match!=NULL) {
			server.removeCursor(match);
			stickyCursorList.remove(match);
			cursor = NULL;
		} else {
			cursor = server.addCursor((float)(x)/ofGetWidth(), (float)(y)/ofGetHeight());
			stickyCursorList.push_back(cursor);
		}
	} else {
		if (match!=NULL) {
			cursor=match;
		} else {
			cursor = server.addCursor((float)(x)/ofGetWidth(), (float)(y)/ofGetHeight());
		}
	}	
	 
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
	if (cursor==NULL) return;
	
	TuioCursor *match = NULL;
	std::list<TuioCursor*> cursorList = server.tuioServer->getTuioCursors();
	for (std::list<TuioCursor*>::iterator tuioCursor = stickyCursorList.begin(); tuioCursor!=stickyCursorList.end(); tuioCursor++) {
		if ((*tuioCursor)->getDistance((float)(x)/ofGetWidth(), (float)(y)/ofGetHeight()) < (10.0f/ofGetWidth())) {
			match = (*tuioCursor);
			break;
		}
	}
	
	if (match==NULL) {
		server.removeCursor(cursor);
	}
	cursor = NULL;
	 
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

