#include "testApp.h"
#include "stdio.h"

//--------------------------------------------------------------
testApp::testApp(){

}

//--------------------------------------------------------------
void testApp::setup(){
	camWidth 		= 640;	// try to grab at this size.
	camHeight 		= 480;
	appWidth        = ofGetWidth();
	appHeight       = ofGetHeight();
	
	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);
	
    vidGrabber.setDeviceID(0);
	
#ifdef USING_OFX_VIDEOGRABBER
	
    Libdc1394Grabber *sdk = new Libdc1394Grabber;
	//sdk->setFormat7(VID_FORMAT7_0);
	sdk->listDevices();
	sdk->setDiscardFrames(true);
	sdk->set1394bMode(true);
	//sdk->setROI(0,0,320,200);
	//sdk->setDeviceID("b09d01008bc69e:0");
	
	ofxIIDCSettings *settings = new ofxIIDCSettings;
	settings->setXMLFilename("mySettingsFile.xml");
#endif
	
	vidGrabber.setVerbose(true);
	
#ifdef USING_OFX_VIDEOGRABBER
	
    bool result = vidGrabber.initGrabber( camWidth, camHeight, VID_FORMAT_YUV422, VID_FORMAT_RGB, 30, true, sdk, settings );
	
    //bool result = vidGrabber.initGrabber( camWidth, camHeight, VID_FORMAT_YUV422, VID_FORMAT_RGB, 30 );
	// or like this:
	//bool result = vidGrabber.initGrabber( camWidth, camHeight, VID_FORMAT_GREYSCALE, VID_FORMAT_GREYSCALE, 30, true, new Libdc1394Grabber);
	// or like this:
	//bool result = vidGrabber.initGrabber( camWidth, camHeight, VID_FORMAT_YUV411, VID_FORMAT_RGB, 30, true, new Libdc1394Grabber, new ofxIIDCSettings);
#else
	
	bool result = vidGrabber.initGrabber( camWidth, camHeight );
	
#endif
	
	if(result) {
	    ofLog(OF_LOG_VERBOSE,"Camera successfully initialized.");
	} else {
	    ofLog(OF_LOG_FATAL_ERROR,"Camera failed to initialize.");
	}
	
	colorLargeImage.allocate( camWidth, camHeight );
	colorSmallImage.allocate( SAMPLE_WIDTH, SAMPLE_HEIGHT );
    grayLargeImage.allocate( camWidth, camHeight);
	grayPrev.allocate( camWidth, camHeight );
	grayCurDiff.allocate(camWidth, camHeight);
	graySmallImage.allocate( SAMPLE_WIDTH, SAMPLE_HEIGHT );
	bgImg.allocate( camWidth, camHeight ); 

	motionThreshold = 300000.0f;
}

//--------------------------------------------------------------
void testApp::update(){
	//vidGrabber.update();
	
	bool bNewFrame = false;
	vidGrabber.grabFrame();
	
	 bNewFrame = vidGrabber.isFrameNew();
	
	if( bNewFrame )
	{
	    colorLargeImage.setFromPixels( vidGrabber.getPixels(), camWidth, camHeight );
		 
		//colorLargeImage.mirror( false, true );
		colorSmallImage.scaleIntoMe( colorLargeImage );
		
		grayLargeImage	= colorLargeImage;
		graySmallImage	= colorSmallImage;
		
		if( learnedBackground == false ) {
			bgImg = grayLargeImage;
			learnedBackground = true;
		}
	
	}
	
	checkDifferencing();
	
}


//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0xffffff);
	vidGrabber.draw(appWidth - camWidth,0);
}


//--------------------------------------------------------------
void testApp::checkDifferencing() {
		grayCurDiff.absDiff(grayPrev, grayLargeImage);   // curDiff is the difference between current and previous frame
		cvThreshold(grayCurDiff.getCvImage(), grayCurDiff.getCvImage(), diffThreshold, 255, CV_THRESH_TOZERO); // anything below diffThreshold, drop to zero (compensate for noise)
		int numPixelsChanged = grayCurDiff.countNonZeroInRegion(0, 0, camWidth, camHeight); // this is how many pixels have changed that are above diffThreshold
		
		
		if (numPixelsChanged > motionThreshold) {
			cout<<"CHANGED BECAUSE: "<<numPixelsChanged<<" pixels changed "<<endl;
		}

	grayPrev = grayLargeImage; // save current frame for next loop
	
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

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
void testApp::resized(int w, int h){

}

