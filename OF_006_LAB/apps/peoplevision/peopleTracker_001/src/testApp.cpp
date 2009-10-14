#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetFrameRate(60);
	ofBackground(100, 100, 100);
	
	//ofSetLogLevel(OF_LOG_VERBOSE);
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
	#endif
	
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
	
	grayLastImage.allocate( camWidth/2, camHeight/2 );
	colorBabyImage.allocate( camWidth/2, camHeight/2 );
	grayBabyImage.allocate( camWidth/2, camHeight/2 );
	
	opticalFlow.allocate( camWidth/2, camHeight/2 );
	opticalFlow.setCalcStep(5,5);
	
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
	gui.addSlider("haar threshold add", &haarArea, 0.0f, 200.0f);
	gui.addToggle("send haar center as blob center", &bUseHaarAsCenter);
	gui.addSlider("minimum haar checkable blob", &minHaarArea, 0.0f, 640.0f);
	gui.addSlider("maximum haar checkable blob", &maxHaarArea, 640.0f, 640.0f*480.0f);
	
	gui.loadFromXML();
	
	bLearnBackground = true;	
	
	//set tracker
	haarFinder.setup( "HS.xml" );
	haarTracker.setup( &haarFinder );
	
	// connect TUIO
	
	tuioClient.setup("localhost", 3333);
	
	mode = 0;
}

//--------------------------------------------------------------
void testApp::update(){
		
	//check for new frame + do stuff
	
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
		
		//clear out vector of people
		
		peopleVector.clear();
		
	//get pixels
		
#ifdef _USE_LIVE_VIDEO
		colorImg.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);
		
		colorSmallImage.scaleIntoMe( colorImg );
		colorBabyImage.scaleIntoMe( colorImg );
		graySmallImage	= colorSmallImage;
		grayBabyImage	= colorBabyImage;
		
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
		//grayDiff.absDiff(grayBg, grayImage);		// normal way blob trackers do it
		
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
		contourFinder.findContours(grayDiff, minBlob, maxBlob, 50, bFindHoles);
		
	//detect haar patterns
	// NOTE: TO AVOID DOUBLE LOOPING THROUGH THE CONTOURS, THIS ALSO POPULATES THE PEOPLE VECTOR
		
		if (bDetectHaar){
		
			int blobId, oldId;
			oldId = -1;
			
			//try to find faces?
			for (int i = 0; i < contourFinder.nBlobs; i++){
				
				ofxPerson p = ofxPerson();
				
				ofxCvBlob blob = contourFinder.blobs[i];
				
				p.area		= (float) blob.area / (ofGetWidth()*ofGetHeight());
				p.x			= (float) blob.boundingRect.x / ofGetWidth();
				p.y			= (float) blob.boundingRect.y / ofGetHeight();
				p.width		= (float) blob.boundingRect.width / ofGetWidth();
				p.height	= (float) blob.boundingRect.height / ofGetHeight();
				p.velocity.x = (float) opticalFlow.getFlowFromPoint(p.x, p.y)[0] / ofGetWidth();
				p.velocity.y = (float) opticalFlow.getFlowFromPoint(p.x, p.y)[1] / ofGetHeight();
				
				// this creates an ofRectangle to pass as a ROI to the haartracker
				// the haarArea var adds to the area you're checking
				
				ofRectangle newrect;
				newrect.x		= fmax(0.0f, (float) blob.boundingRect.x/(grayImage.width/grayBabyImage.width) - haarArea/(grayImage.width/grayBabyImage.width));
				newrect.y		= fmax(0.0f, (float)blob.boundingRect.y/(grayImage.width/grayBabyImage.width) - haarArea/(grayImage.width/grayBabyImage.width));
				newrect.width	= fmin((float)blob.boundingRect.width/(grayImage.width/grayBabyImage.width) + haarArea*2/(grayImage.width/grayBabyImage.width), grayBabyImage.width);
				newrect.height	= fmin((float)blob.boundingRect.height/(grayImage.width/grayBabyImage.width) + haarArea*2/(grayImage.width/grayBabyImage.width),  grayBabyImage.height);
				
				if (blob.area > minHaarArea && blob.area < maxHaarArea){
					haarTracker.findHaarObjects( grayBabyImage, newrect );
					
					float x, y, w, h;
					
					while( haarTracker.hasNextHaarItem() )
					{
						haarTracker.getHaarItemPropertiesEased( &x, &y, &w, &h );
						
						blobId = haarTracker.getHaarItemID();
						
						if (blobId != oldId){
							
							//mult by two since haar finder is looking at a 320x240 sample
							
							x	*= (float)(grayImage.width/grayBabyImage.width);
							y	*= (float)(grayImage.width/grayBabyImage.width);
							w	*= (float)(grayImage.width/grayBabyImage.width);
							h	*= (float)(grayImage.width/grayBabyImage.width);
							
							p.hasHaar = true;
							p.haarRect.x = (float) x / ofGetWidth();
							p.haarRect.y = (float) y / ofGetHeight();
							p.haarRect.width = (float) w / ofGetWidth();
							p.haarRect.height = (float) h / ofGetHeight();
						}
						oldId = blobId;
					}
					
					//haarTracker.clearHaarItems();
				} else {
					p.hasHaar = false;
				}
				
				peopleVector.push_back(p);
			}
		}
		
	//detect optical flow
		
		if (bTrackOpticalFlow){
			opticalFlow.calc(grayLastImage,graySmallImage,11);
		}
		
		//make last image = small gray image for when you look at optical flow
		
		grayLastImage = graySmallImage;
		
	// create people objects
	// NOTE: THIS SHOULD PROBABLY BE INCORPORATED UP WITH THE HAAR LOOP SO YOU DON'T GO THROUGH
	// ALL THE BLOBS TWICE...
		
		// people are already in vector
		
		if (!bDetectHaar){
			for (int i = 0; i < contourFinder.nBlobs; i++){
				
				ofxPerson p = ofxPerson();				
				ofxCvBlob blob = contourFinder.blobs[i];
				
				p.area		= (float) blob.area / (ofGetWidth()*ofGetHeight());
				p.x			= (float) blob.boundingRect.x / ofGetWidth();
				p.y			= (float) blob.boundingRect.y / ofGetHeight();
				p.width		= (float) blob.boundingRect.width / ofGetWidth();
				p.height	= (float) blob.boundingRect.height / ofGetHeight();
				p.velocity.x = (float) opticalFlow.getFlowFromPoint(p.x, p.y)[0] / ofGetWidth();
				p.velocity.y = (float) opticalFlow.getFlowFromPoint(p.x, p.y)[1] / ofGetHeight();
				p.hasHaar	= false;
				
				peopleVector.push_back(p);
			}
		}
		
	//force learn background if there are > 5 blobs (off by default)
		
		if (bSmartLearnBackground == true && contourFinder.nBlobs > 5){
			bLearnBackground = true;
		}
		
	}
	
	//setup tuio
	
	for (int i=0; i<peopleVector.size(); i++){
		/*
		 NOT GIVING THE BEST RESULTS... COULD WORK IF WE IMPLEMENTED A SMARTER WAY FOR GETTING THE VECLOCITY FROM THE AREA
		 if ( peopleVector[i].velocity.x > 0 || peopleVector[i].velocity.y > 0){
			if (bUseHaarAsCenter && peopleVector[i].hasHaar) tuioClient.cursorDragged( peopleVector[i].haarRect.x, peopleVector[i].haarRect.y, i);
			else tuioClient.cursorDragged( peopleVector[i].x + peopleVector[i].width/2, peopleVector[i].y + peopleVector[i].height/2, i);
		} else {*/
			if (bUseHaarAsCenter && peopleVector[i].hasHaar) tuioClient.cursorDragged( peopleVector[i].haarRect.x + peopleVector[i].haarRect.width/2, peopleVector[i].haarRect.y + peopleVector[i].haarRect.height/2, i);
			else tuioClient.cursorDragged( peopleVector[i].x + peopleVector[i].width/2, peopleVector[i].y + peopleVector[i].width/2, i);
		//}
	}
	tuioClient.update();
}

//--------------------------------------------------------------
void testApp::draw(){
		
	if (mode == MODE_NORMAL || mode == MODE_GUI){
		
		// draw the incoming, the grayscale, the bg and the thresholded difference
		ofSetColor(0xffffff);
		colorImg.draw(20,20,320,240);
		grayDiff.draw(360,20,320,240);
		grayBg.draw(20,280,320,240);
		
		// draw optical flow
		
		if (bTrackOpticalFlow){
			ofPushMatrix();
			ofTranslate(680, 20);
			//ofScale(2, 2);		
			opticalFlow.draw();
			ofPopMatrix();
		}
				
		// draw the contours:
		
		ofFill();
		ofSetColor(0x333333);
		ofRect(360,280,320,240);
		ofSetColor(0xffffff);

		//individually draw blobs
		
		ofPushMatrix();
			ofTranslate(360,280);
			ofScale(0.5f,0.5f,0.5f);
			ofNoFill();
			
			for (int i=0; i<peopleVector.size(); i++){
				//draw blobs				
				//if haarfinder is looking at these blobs, draw the area it's looking at
				
				ofRectangle drawRect = peopleVector[i].getScaled(ofGetWidth(), ofGetHeight());
				
				if (peopleVector[i].getScaledArea(ofGetWidth(), ofGetHeight()) > minHaarArea && peopleVector[i].getScaledArea(ofGetWidth(), ofGetHeight()) < maxHaarArea){
					ofSetColor(0xffffff);
					
					
					ofRect(drawRect.x - haarArea/2, drawRect.y - haarArea/2, drawRect.width + haarArea, drawRect.height + haarArea);
					ofSetColor(0xffff00);
					
				//otherwise just draw a blue square
				} else {
					ofSetColor(0x0000ff);
				}
				ofRect(drawRect.x, drawRect.y, drawRect.width, drawRect.height);
				
				if (peopleVector[i].hasHaar){
					
					ofRectangle haarRect = peopleVector[i].getScaledHaar(ofGetWidth(), ofGetHeight());
					
					ofSetColor( 0xFF00FF );
					ofRect( haarRect.x, haarRect.y, haarRect.width, haarRect.height );
				}
			}
		
		/*
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
		 
		 */
		ofPopMatrix();
		
		// finally, a report:
		
		ofSetColor(0xffffff);		
		gui.draw();
		
// draw ofxVideoGrabber calibration mode
	} else if (mode == MODE_CAMERA_CALIBRATE){
		vidGrabber.draw(ofGetWidth()- camWidth,0);
		
// fullscreen mode : just draws the main gray image
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
