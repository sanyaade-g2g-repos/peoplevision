#include "testApp.h"

class CYAPersonAttributes {
public:
	CYAPersonAttributes(){
		height = 0;
		hasBeard = false;
	}
	
	float height;
	bool hasBeard;
};

//--------------------------------------------------------------
void testApp::setup(){

	ofSetFrameRate(60);
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	camWidth = 640;
	camHeight = 480;

	#ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(camWidth,camHeight);
	#else
        vidPlayer.loadMovie("testmovie/testmovie2.mov");
        vidPlayer.play();
	#endif
	
	colorImg.allocate(camWidth, camHeight);
	
	peopleTracker.setup(camWidth, camHeight);
	peopleTracker.setListener( this );

	peopleTracker.addSlider("custom INTEGER", &itestValue, 0, ofGetWidth());
	peopleTracker.addSlider("custom FLOAT", &ftestValue, 0, ofGetHeight());
	peopleTracker.addToggle("custom BOOL", &btestValue);
	
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(100,100,100);

    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.grabFrame();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.idleMovie();
        bNewFrame = vidPlayer.isFrameNew();
	#endif

	if (bNewFrame){

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels(), camWidth,camHeight);
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels(), camWidth,camHeight);
        #endif
		peopleTracker.update(colorImg);

		//iterate through the people
		for(int i = 0; i < peopleTracker.totalPeople(); i++){
			ofxCYAPerson* p = peopleTracker.personAtIndex(i);
			//now make sweet interactivity with these people!
		}
	}
}

//delegate methods for people entering and exiting
void testApp::personEntered( ofxCYAPerson* newPerson, ofxCYAScene* scene )
{
	newPerson->customAttributes = new CYAPersonAttributes();
	
	//do something with them
	ofLog(OF_LOG_VERBOSE, "person %d of size %f entered!\n", newPerson->pid, newPerson->area);
}

void testApp::personMoved( ofxCYAPerson* activePerson, ofxCYAScene* scene )
{
	
	//do something with the moving person
	ofLog(OF_LOG_VERBOSE, "person %d of moved to (%f,%f)!\n", activePerson->pid, activePerson->boundingRect.x, activePerson->boundingRect.y);
}

void testApp::personWillLeave( ofxCYAPerson* leavingPerson, ofxCYAScene* scene )
{
	//do something to clean up
	ofLog(OF_LOG_VERBOSE, "person %d left after being %d frames in the system\n", leavingPerson->pid, leavingPerson->age);
}

void testApp::personUpdated( ofxCYAPerson* updatedPerson, ofxCYAScene* scene )
{
	CYAPersonAttributes* attrbs = (CYAPersonAttributes*)updatedPerson->customAttributes;
	attrbs->hasBeard = true;
	
	ofLog(OF_LOG_VERBOSE, "updated %d person\n", updatedPerson->pid);
}

//--------------------------------------------------------------
void testApp::draw(){
	peopleTracker.draw();

	if(btestValue){
		ofCircle(itestValue, ftestValue, 30);
	}	
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	switch (key){
		case ' ':{
			peopleTracker.relearnBackground();
		}break;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
}

