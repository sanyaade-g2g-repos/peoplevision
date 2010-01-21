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
	ofBackground(100, 100, 100);
	//ofSetDataPathRoot("data/");

	camWidth = 640;
	camHeight = 480;
	
#ifdef _USE_LIVE_VIDEO
	 
	#ifdef USING_OFX_VIDEOGRABBER
		
	vidGrabber.setVerbose(true);
	vidGrabber.setDeviceID(0);
	
	Libdc1394Grabber *sdk = new Libdc1394Grabber;
	//sdk->setFormat7(VID_FORMAT7_0);
	sdk->listDevices();
	sdk->setDiscardFrames(true);
	//sdk->set1394bMode(true);
	//sdk->setROI(0,0,320,200);
	//sdk->setDeviceID("b09d01008bc69e:0");

	ofxIIDCSettings *settings = new ofxIIDCSettings;
	settings->setXMLFilename(ofToDataPath("settings.xml", true));
		
	bool result = vidGrabber.initGrabber( camWidth, camHeight, BLOB_TRACKER_VIDEO_FORMAT, BLOB_TRACKER_COLOR_FORMAT, 60, true, sdk, settings );
	
	if(result) {
		ofLog(OF_LOG_VERBOSE,"Camera successfully initialized.");
	} else {
		ofLog(OF_LOG_FATAL_ERROR,"Camera failed to initialize.");
	}
	
	vidGrabber.update();
	#else
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(camWidth, camHeight);  
	#endif
	
#else
	vidPlayer.loadMovie("testmovie/testmovie2.mov");
	vidPlayer.play();
#endif
    
	colorImg.allocate(camWidth, camHeight);
	peopleTracker.setup(camWidth, camHeight);
	//send tuio events
	//peopleTracker.setupTuio("localhost", 3333);
	//listen to any events from people entering or exiting
	peopleTracker.setListener( this );
	
	peopleTracker.addSlider("custom INTEGER", &itestValue, 0, ofGetWidth());
	peopleTracker.addSlider("custom FLOAT", &ftestValue, 0, ofGetHeight());
	peopleTracker.addToggle("custom BOOL", &btestValue);

}

//--------------------------------------------------------------
void testApp::update(){
    bool bNewFrame = false;	
#ifdef _USE_LIVE_VIDEO
	
	#ifdef USING_OFX_VIDEOGRABBER
	vidGrabber.update();
	#endif
	
	vidGrabber.grabFrame();
	
	#ifndef USING_OFX_VIDEOGRABBER
	//there is something wrong with this in ofxVideoGrabber
	bNewFrame = vidGrabber.isFrameNew();
	#else
	bNewFrame = true;
	#endif
#else
	vidPlayer.idleMovie();
	bNewFrame = vidPlayer.isFrameNew();
#endif
	
	if (bNewFrame){		
	#ifdef _USE_LIVE_VIDEO
		colorImg.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);				
	#else
		colorImg.setFromPixels(vidPlayer.getPixels(), camWidth, camHeight);
	#endif		
		
		//update all people in the system
		peopleTracker.update(colorImg);
		
		//iterate through the people to do interesting things
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
	printf("person %d of size %f entered!\n", newPerson->pid, newPerson->area);
}

void testApp::personMoved( ofxCYAPerson* activePerson, ofxCYAScene* scene )
{
	
	//do something with the moving person
	printf("person %d of moved to (%f,%f)!\n", activePerson->pid, activePerson->boundingRect.x, activePerson->boundingRect.y);
}

void testApp::personWillLeave( ofxCYAPerson* leavingPerson, ofxCYAScene* scene )
{
	//do something to clean up
	printf("person %d left after being %d frames in the system\n", leavingPerson->pid, leavingPerson->age);
}

void testApp::personUpdated( ofxCYAPerson* updatedPerson, ofxCYAScene* scene )
{
	CYAPersonAttributes* attrbs = (CYAPersonAttributes*)updatedPerson->customAttributes;
	attrbs->hasBeard = true;
	
	printf("updated %d person\n", updatedPerson->pid);
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
		case ' ':
			peopleTracker.relearnBackground();
			break;
		case 'm':
			//increment to the next draw mode
			peopleTracker.setDrawMode( (peopleTracker.getDrawMode()+1) % 4 );
			break;
		case 'f':
			ofToggleFullscreen();
			break;
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
