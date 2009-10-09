#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	
	camWidth = 640;
	camHeight = 480;
	
#ifdef _USE_LIVE_VIDEO
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(camWidth, camHeight);
#else
	vidPlayer.loadMovie("fingers.mov");
	vidPlayer.play();
#endif
	
    colorImg.allocate(camWidth, camHeight);
	grayImage.allocate(camWidth, camHeight);
	grayBg.allocate(camWidth, camHeight);
	grayDiff.allocate(camWidth, camHeight);
	floatBgImg.allocate(camWidth, camHeight);
	
	bLearnBakgroundProgressive = true;
	bTrackDark = false;
	//bLearnBakground = true;
	threshold = 80;
	fLearnRate = 0.0f;
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
		colorImg.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);
#else
		colorImg.setFromPixels(vidPlayer.getPixels(), camWidth, camHeight);
#endif
		
        grayImage = colorImg;
		if (bLearnBakground == true){
			floatBgImg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		} else if (bLearnBakgroundProgressive){
			floatBgImg.addWeighted( grayImage, fLearnRate * .0001);
			//grayBg = floatBgImg;  // not yet implemented
			cvConvertScale( floatBgImg.getCvImage(), grayBg.getCvImage(), 255.0f/65535.0f, 0 );       
			grayBg.flagImageChanged();			
		}
		
		// take the abs value of the difference between background and incoming and then threshold:
		//grayDiff.absDiff(grayBg, grayImage);
		grayDiff = grayImage;
		if(bTrackDark)
			cvSub(grayBg.getCvImage(), grayDiff.getCvImage(), grayDiff.getCvImage());
		else
			cvSub(grayDiff.getCvImage(), grayBg.getCvImage(), grayDiff.getCvImage());
		
		grayDiff.flagImageChanged();
		
		grayDiff.threshold(threshold);
		//grayDiff = grayImage;
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, true);	// find holes
	}
	
	//printf("%f \n", ofGetFrameRate());
}

//--------------------------------------------------------------
void testApp::draw(){
	
	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetColor(0xffffff);
	colorImg.draw(20,20,320,240);
	grayDiff.draw(360,20,320,240);
	grayBg.draw(20,280,320,240);
	//grayDiff.draw(360,280,320,240);
	
	// then draw the contours:
	
	ofFill();
	ofSetColor(0x333333);
	ofRect(360,280,320,240);
	ofSetColor(0xffffff);
	
	// we could draw the whole contour finder
	//contourFinder.draw(360,540);
	
	// or, instead we can draw each blob individually,
	// this is how to get access to them:
	ofPushMatrix();
	ofTranslate(360,280);
	ofScale(0.5f,0.5f,0.5f);
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(0,0);
    }
	ofPopMatrix();
	
	// finally, a report:
	
	ofSetColor(0xffffff);
	char reportStr[1024];
	sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\nthreshold %i (press: +/-)\nto change bg learn rate press: } or{ \n{ = +, { = -\n num blobs found %i\nlooking for dark blobs? %i\n learning rate = %f", threshold, contourFinder.nBlobs, bTrackDark, fLearnRate);
	ofDrawBitmapString(reportStr, 20, 600);
	
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	
	switch (key){
		case ' ':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
		case '}':
			fLearnRate++;
			break;
		case '{':
			fLearnRate--;
			if (fLearnRate < 0.0f) fLearnRate = 0.0f;
			break;
		case 'd':
			bTrackDark = !bTrackDark;
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
