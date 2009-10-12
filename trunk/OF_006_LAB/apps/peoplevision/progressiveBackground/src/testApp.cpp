#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetFrameRate(60);
	ofSetLogLevel(OF_LOG_VERBOSE);
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
	settings->setXMLFilename("settings.xml");
		
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
	vidGrabber.videoSettings();
	#endif
	
#else
	vidPlayer.loadMovie("fingers.mov");
	vidPlayer.play();
#endif
	
    colorImg.allocate(camWidth, camHeight);
	grayImage.allocate(camWidth, camHeight);
	grayLastImage.allocate(camWidth, camHeight);
	grayBg.allocate(camWidth, camHeight);
	grayDiff.allocate(camWidth, camHeight);
	floatBgImg.allocate(camWidth, camHeight);
	colorSmallImage.allocate( camWidth/2, camHeight/2 );
	graySmallImage.allocate( camWidth/2, camHeight/2 );
	opticalFlow.allocate(camWidth, camHeight);
	
	bLearnBackgroundProgressive = true;
	bTrackDark = false;
	bLearnBackground = true;
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
	gui.addToggle("learn background", &bLearnBackground);
	gui.addToggle("smart learn background", &bSmartLearnBackground);
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
	gui.addToggle("track optical flow", &bTrackOpticalFlow);
	gui.addToggle("track dark blobs", &bTrackDark);
	gui.addToggle("track faces", &bDetectHaar);
	gui.addSlider("haar threshold add", &haarArea, 0.0f, 100.0f);
	gui.addSlider("minimum haar checkable blob", &minHaarArea, 0.0f, 640.0f);
	gui.addSlider("maximum haar checkable blob", &maxHaarArea, 640.0f, 640.0f*480.0f);
	
	gui.loadFromXML();
	
	bLearnBackground = true;	
	
	//set tracker
	haarFinder.setup( "HS.xml" );
	haarTracker.setup( &haarFinder );
	
	mode = 0;
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(100,100,100);
	
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
		
		//get pixels
		
#ifdef _USE_LIVE_VIDEO
		colorImg.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);
		
		colorSmallImage.scaleIntoMe( colorImg );
		graySmallImage	= colorSmallImage;
		
#else
		colorImg.setFromPixels(vidPlayer.getPixels(), camWidth, camHeight);
#endif		
        grayImage = colorImg;
		
		//learn background (either in reset or additive)
		
		if (bLearnBackground == true){
			cout<<"DO IT"<<endl;
			floatBgImg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBackground = false;
		} else if (bLearnBackgroundProgressive){
			floatBgImg.addWeighted( grayImage, fLearnRate * .0001);
			//grayBg = floatBgImg;  // not yet implemented
			cvConvertScale( floatBgImg.getCvImage(), grayBg.getCvImage(), 255.0f/65535.0f, 0 );       
			grayBg.flagImageChanged();			
		}
		
		// take the abs value of the difference between background and incoming and then threshold:
		//grayDiff.absDiff(grayBg, grayImage);
		
		grayDiff = grayImage;
		
		//get diff of images either as lights or darks
		
		if(bTrackDark)
			cvSub(grayBg.getCvImage(), grayDiff.getCvImage(), grayDiff.getCvImage());
		else
			cvSub(grayDiff.getCvImage(), grayBg.getCvImage(), grayDiff.getCvImage());
		
		grayDiff.flagImageChanged();
		
		//smoothing
		
		if(bSmooth){//Smooth
            grayDiff.blur((smooth * 2) + 1); //needs to be an odd number
        }
		
		//highpass filter (see cpuimagefilter class)
		
		if(bHighpass){//HighPass
			grayDiff.highpass(highpassBlur, highpassNoise);
        }
		
		//amplify (see cpuimagefilter class)
		
        if(bAmplify){//Amplify
			grayDiff.amplify(grayDiff, highpassAmp);
        }
		
		//threshold + find blobs
		
		grayDiff.threshold(threshold);
		//grayDiff = grayImage;
		contourFinder.findContours(grayDiff, minBlob, maxBlob, 50, bFindHoles);
		
		//detect haar patterns
		
		if (bDetectHaar){
		
			//try to find faces?
			for (int i = 0; i < contourFinder.nBlobs; i++){
				ofxCvBlob blob = contourFinder.blobs[i];
				ofRectangle newrect;
				newrect.x		= blob.boundingRect.x/2.0f - haarArea;
				newrect.y		= blob.boundingRect.y/2.0f - haarArea;
				newrect.width	= blob.boundingRect.width/2.0f + haarArea*2;
				newrect.height	= blob.boundingRect.height/2.0f + haarArea*2;
				
				if (blob.area > minHaarArea && blob.area < maxHaarArea) haarTracker.findHaarObjects( graySmallImage, newrect );
			}
		}
		
		//detect optical flow
		
		if (bTrackOpticalFlow){
			opticalFlow.calc(grayLastImage,grayImage,11);
		}
		
		grayLastImage = grayImage;
		
		//force learn background if there are > 5 blobs (off by default)
		
		if (bSmartLearnBackground == true && contourFinder.nBlobs > 5){
			bLearnBackground = true;
		}
	}
		
	//printf("%f \n", ofGetFrameRate());
}

//--------------------------------------------------------------
void testApp::draw(){
	
	if (mode == MODE_NORMAL || mode == MODE_GUI){
		
		// draw the incoming, the grayscale, the bg and the thresholded difference
		ofSetColor(0xffffff);
		colorImg.draw(20,20,320,240);
		grayDiff.draw(360,20,320,240);
		grayBg.draw(20,280,320,240);
		
		if (bTrackOpticalFlow){
			ofPushMatrix();
			ofTranslate(680, 20);
			ofScale(.5, .5);		
			opticalFlow.draw();
			ofPopMatrix();
		}
		
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
		ofNoFill();
		
		for (int i = 0; i < contourFinder.nBlobs; i++){
			
			//if haarfinder is looking at these blobs, draw the area it's looking at
			
			if (contourFinder.blobs[i].area > minHaarArea && contourFinder.blobs[i].area < maxHaarArea){
				ofSetColor(0xffffff);
				ofRect(contourFinder.blobs[i].boundingRect.x - haarArea/2, contourFinder.blobs[i].boundingRect.y - haarArea/2, 
					   contourFinder.blobs[i].boundingRect.width + haarArea, contourFinder.blobs[i].boundingRect.height + haarArea);
				ofSetColor(0xffff00);
				
			//otherwise just draw a blue square
			} else {
				ofSetColor(0x0000ff);
			}
			ofRect(contourFinder.blobs[i].boundingRect.x, contourFinder.blobs[i].boundingRect.y, contourFinder.blobs[i].boundingRect.width, contourFinder.blobs[i].boundingRect.height);
		}
		
		//loop through + draw where faces are found (pink box)
		
		float x, y, w, h;
		int faceID;
		float faceMode;
		
		ofSetColor( 0xFF00FF );
		
		while( haarTracker.hasNextHaarItem() )
		{
			faceID		= haarTracker.getHaarItemID();
			
			haarTracker.getHaarItemPropertiesEased( &x, &y, &w, &h );
			
			//mult by two since haar finder is looking at a 320x240 sample
			
			x	*= 2.0f;
			y	*= 2.0f;
			w	*= 2.0f;
			h	*= 2.0f;
			
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
	} else if (mode == MODE_CAMERA_CALIBRATE){
		vidGrabber.draw(ofGetWidth()- camWidth,0);
	} else if (mode == MODE_FULLSCREEN){
		
		ofSetColor(0xffffff);
		
		float ratio = fmax((float)ofGetWidth()/colorImg.width, (float) ofGetHeight()/colorImg.height);
		//if (mode == MODE_CAMERA_CALIBRATE){	
			colorImg.draw(0,0,colorImg.width*ratio,colorImg.height*ratio);
		/*} else {
			colorImg.draw(0,0,colorImg.width*ratio,colorImg.height*ratio);
		}*/
	}
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	
	//most of this stuff is done in the gui now
	
	switch (key){
		case ' ':
			bLearnBackground = true;
			break;
		/*case '+':
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
		 */
		case 'm':
			mode++;
			if ( mode > 3) mode = 0;
			if ( mode == MODE_GUI || mode == MODE_CAMERA_CALIBRATE) gui.toggleDraw();
			if (mode == MODE_CAMERA_CALIBRATE){
			#ifdef USING_OFX_VIDEOGRABBER
				//vidGrabber.videoSettings();
			#else
				mode = MODE_FULLSCREEN;
			#endif
			}
			break;
		/*
		case 'h':
			bHighpass = !bHighpass;
			break;
		case 'a':
			bAmplify = !bAmplify;
			break;
		case 's':
			bSmooth = !bSmooth;
			break;
		 */
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
