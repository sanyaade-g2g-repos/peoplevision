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
	colorSmallImage.allocate( camWidth/2, camHeight/2 );
	graySmallImage.allocate( camWidth/2, camHeight/2 );
	
	bLearnBakgroundProgressive = true;
	bTrackDark = false;
	//bLearnBakground = true;
	threshold = 80;
	fLearnRate = 0.0f;
	highpassBlur = 0;
	highpassNoise = 0;
	highpassAmp = 0;
	smooth = 0;
	bAmplify = bSmooth = bHighpass = false;
	bFindHoles = true;
	
	minBlob = 30;
	maxBlob = 640*480;
	
	//set gui
	//gui.addSlider(string name, int *value, int min, int max);
	gui.addToggle("learn background", &bLearnBakground);
	gui.addSlider("threshold", &threshold, 0, 255);
	gui.addSlider("minimum blob", &minBlob, 0.0f, 640.0f*480.0f/10.0f);
	gui.addSlider("maximum blob", &maxBlob, 640.0f, 640.0f*480.0f*3);
	gui.addSlider("bg learn rate", &fLearnRate, 0.0f, 1000.0f);
	gui.addToggle("smoothing on", &bSmooth);
	gui.addSlider("smooth", &smooth, 0, 15);
	
	gui.addToggle("highpass on", &bHighpass);
	gui.addSlider("highpass blur", &highpassBlur, 0, 200);
	gui.addSlider("highpass noise", &highpassNoise, 0, 30);
	gui.addToggle("amplify on", &bAmplify);
	gui.addSlider("amplify", &highpassAmp, 0, 200);
	
	gui.addToggle("find holes", &bFindHoles);
	
	gui.addToggle("track dark blobs", &bTrackDark);
	gui.addToggle("track faces", &bDetectHaar);
	gui.loadFromXML();
	
	//set tracker
	haarFinder.setup( "HS.xml" );
	haarTracker.setup( &haarFinder );
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
		
		colorSmallImage.scaleIntoMe( colorImg );
		graySmallImage	= colorSmallImage;
		
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
		
		if(bSmooth){//Smooth
            grayDiff.blur((smooth * 2) + 1); //needs to be an odd number
        }
		
		if(bHighpass){//HighPass
			grayDiff.highpass(highpassBlur, highpassNoise);
        }
		
        if(bAmplify){//Amplify
			grayDiff.amplify(grayDiff, highpassAmp);
        }
		
		grayDiff.threshold(threshold);
		//grayDiff = grayImage;
		contourFinder.findContours(grayDiff, minBlob, maxBlob, 50, bFindHoles);
		
		//try to find faces?
		for (int i = 0; i < contourFinder.nBlobs; i++){
			ofxCvBlob blob = contourFinder.blobs[i];
			ofRectangle newrect;
			newrect.x		= blob.boundingRect.x/2.0f - 20.0f;
			newrect.y		= blob.boundingRect.y/2.0f - 20.0f;
			newrect.width	= blob.boundingRect.width/2.0f + 40.0f;
			newrect.height	= blob.boundingRect.height/2.0f + 40.0f;
			haarTracker.findHaarObjects( graySmallImage, newrect );
		}
		
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
	
	float x, y, w, h;
	int faceID;
	float faceMode;
	
	while( haarTracker.hasNextHaarItem() )
	{
		cout<<"FACE!"<<endl;
		
		faceID		= haarTracker.getHaarItemID();
		faceMode	= ( faceID % 10 ) / 10.0f;
		
		haarTracker.getHaarItemPropertiesEased( &x, &y, &w, &h );
		
		x	*= 2.0f;
		y	*= 2.0f;
		w	*= 2.0f;
		h	*= 2.0f;
		
		if( faceMode > 0.66 )
		{
			ofSetColor( 0xFF00FF );
		}
		else if( faceMode > 0.33 )
		{
			//ofSetColor( 0x00FF00 );
		}
		else
		{
			//ofSetColor( 0x0000FF );
		}
		
		//ofNoFill();
		ofRect( x, y, w, h );
	}
	
	ofPopMatrix();
	
	// finally, a report:
	
	ofSetColor(0xffffff);
	char reportStr[1024];
	sprintf(reportStr, "bg subtraction and blob detection\npress ' '\n num blobs found %i\n", threshold, contourFinder.nBlobs, bTrackDark, fLearnRate);
	ofDrawBitmapString(reportStr, 20, 600);
	
	gui.draw();
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
			gui.toggleDraw();
			break;
		case 'h':
			bHighpass = !bHighpass;
			break;
		case 'a':
			bAmplify = !bAmplify;
			break;
		case 's':
			bSmooth = !bSmooth;
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
