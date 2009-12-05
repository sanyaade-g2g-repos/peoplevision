
#include "ofxCYAPeopleTracker.h"
#include "CPUImageFilter.h"

//scales down tracking images for improved performance
#define TRACKING_SCALE_FACTOR .5

#pragma mark Setup
void ofxCYAPeopleTracker::setup(int w, int h)
{
	width  = w;
	height = h;
	
	grayImage.allocate(width, height);
	grayBg.allocate(width, height);
	grayDiff.allocate(width, height);
	floatBgImg.allocate(width, height);
	graySmallImage.allocate( width*TRACKING_SCALE_FACTOR, height*TRACKING_SCALE_FACTOR );
	
	grayLastImage.allocate( width*TRACKING_SCALE_FACTOR, height*TRACKING_SCALE_FACTOR );
	grayBabyImage.allocate( width*TRACKING_SCALE_FACTOR, height*TRACKING_SCALE_FACTOR );
	
	//set up optical flow
	opticalFlow.allocate( width*TRACKING_SCALE_FACTOR, height*TRACKING_SCALE_FACTOR );
	opticalFlow.setCalcStep(5,5);
	
	//set tracker
	setHaarXMLFile("HS.xml");
	
	bLearnBackgroundProgressive = true;

	bTrackDark = true;
	bLearnBackground = true;
	threshold = 80;
	fLearnRate = 0.0f;
	highpassBlur = 0;
	highpassNoise = 0;
	highpassAmp = 0;
	smooth = 0; 
	
	bAmplify = bSmooth = bHighpass = false;
	bFindHoles = true;
	bCentroidDampen = true;
	
	minBlob = 30;
	maxBlob = width*height;
	
	drawMode = DRAW_MODE_NORMAL;
	bLearnBackground = true;
	
	//set gui
	//gui.addSlider(string name, int *value, int min, int max);
	gui.addToggle("learn background", &bLearnBackground);
	gui.addToggle("smart learn background", &bSmartLearnBackground);
	gui.addSlider("threshold", &threshold, 0, 255);
	gui.addSlider("minimum blob", &minBlob, 0.0f, 0.1f);
	gui.addSlider("maximum blob", &maxBlob, .2f, 1.0f);
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
	//not sure how to handle 'use haar as center'
	gui.addToggle("use haar for centroid", &bUseHaarAsCenter);
	gui.addSlider("minimum haar checkable blob", &minHaarArea, 0.0f, width);
	gui.addSlider("maximum haar checkable blob", &maxHaarArea, width, width*height);
	gui.addToggle("center dampen", &bCentroidDampen);
	
	gui.loadFromXML();	
	gui.setDraw(true);	
	
    persistentTracker.setListener( this );
}

void ofxCYAPeopleTracker::setHaarXMLFile(string haarFile)
{
	haarFinder.setup(haarFile);
	haarTracker.setup(&haarFinder);

}
void ofxCYAPeopleTracker::setupTuio(string ip, int port)
{
	bTuioEnabled = true;
	tuioClient.setup(ip, port);
}

void ofxCYAPeopleTracker::setListener(ofxPersonListener* listener)
{
	eventListener = listener;
}

#pragma mark Track People
void ofxCYAPeopleTracker::update(ofxCvColorImage image)
{
	grayImage = image;
	trackPeople();
}

void ofxCYAPeopleTracker::update(ofxCvGrayscaleImage image)
{
	grayImage = image;
	trackPeople();
}

/**
 * Core Method
 * Run every frame to update
 * the system to the current location
 * of people
 */
void ofxCYAPeopleTracker::trackPeople()
{
	graySmallImage.scaleIntoMe(grayImage);
	grayBabyImage.scaleIntoMe(grayImage);
	
	//-------------------
	//BACKGROUND
	//-------------------
	
	//force learn background if there are > 5 blobs (off by default)
	if (bSmartLearnBackground == true && contourFinder.nBlobs > 5){
		bLearnBackground = true;
	}
	//learn background (either in reset or additive)
	if (bLearnBackground){
		cout << "Learning Background" << endl;
		floatBgImg = grayImage;
		bLearnBackground = false;
	}
	else if (bLearnBackgroundProgressive){
		floatBgImg.addWeighted( grayImage, fLearnRate * .0001);
		cvConvertScale( floatBgImg.getCvImage(), grayBg.getCvImage(), 255.0f/65535.0f, 0 );       
		grayBg.flagImageChanged();			
	}
	
	grayDiff = grayImage;
	
	if(bTrackDark){
		cvSub(grayBg.getCvImage(), grayDiff.getCvImage(), grayDiff.getCvImage());
	}
	else { 
		cvSub(grayDiff.getCvImage(), grayBg.getCvImage(), grayDiff.getCvImage());
	}
	grayDiff.flagImageChanged();
	
	//-----------------------
	// IMAGE TREATMENT
	//-----------------------
	if(bSmooth){
		grayDiff.blur((smooth * 2) + 1); //needs to be an odd number
	}
	
	//highpass filter (see cpuimagefilter class)	
	if(bHighpass){
		grayDiff.highpass(highpassBlur, highpassNoise);
	}
	
	//amplify (see cpuimagefilter class)
	if(bAmplify){
		grayDiff.amplify(grayDiff, highpassAmp);
	}
	//threshold	
	grayDiff.threshold(threshold);
	
	//-----------------------
	// TRACKING
	//-----------------------	
	
	if (bTrackOpticalFlow){
		opticalFlow.calc(grayLastImage, graySmallImage, 11);
	}
	
	vector <ofxCYAPerson> stillLiving;
	contourFinder.findContours(grayDiff, minBlob*width*height, maxBlob*width*height, 50, bFindHoles);
	persistentTracker.trackBlobs(contourFinder.blobs);
	
	for(int i = 0; i < persistentTracker.blobs.size(); i++){
		ofxCvTrackedBlob blob = persistentTracker.blobs[i];
		//somehow we are not tracking this person, safeguard (shouldn't happen)
		if(!isTrackingPerson(blob.id)){
			printf("ofxPerson::warning. encountered persistent blob without a person behind them\n");
			continue;
		}
		
		//get the tracked person
		ofxCYAPerson p = getTrackedPerson(blob.id);
//		if(p.shouldRemove){
//			//this will stop them from being added to the set and therefore removing them
//			continue;
//		}
		
		//update this person with new blob info
		p.update(blob, bCentroidDampen);

		//sum optical flow for the person
		if(bTrackOpticalFlow){
			p.opticalFlowVectorAccumulation = opticalFlow.flowInRegion(p.boundingRect);
		}
		
		//detect haar patterns (faces, eyes, etc)
		if (bDetectHaar){
			int blobId, oldId;
			bool bHaarItemSet = false;
			if (p.area > minHaarArea && p.area < maxHaarArea){
				
				//find the region of interest, expanded by haarArea.
				//bound by the frame edge
				ofRectangle roi;
				roi.x		= fmax( (p.boundingRect.x - haarArea) * TRACKING_SCALE_FACTOR, 0.0f );
				roi.y		= fmax( (p.boundingRect.y - haarArea) * TRACKING_SCALE_FACTOR, 0.0f );
				roi.width	= fmin( (p.boundingRect.width  + haarArea*2) * TRACKING_SCALE_FACTOR, grayBabyImage.width - roi.x );
				roi.height	= fmin( (p.boundingRect.height + haarArea*2) * TRACKING_SCALE_FACTOR, grayBabyImage.width - roi.y );	
				
				haarTracker.findHaarObjects( grayBabyImage, roi );
				float x, y, w, h;
				while( haarTracker.hasNextHaarItem() ) {
					blobId = haarTracker.getHaarItemID();
					haarTracker.getHaarItemPropertiesEased( &x, &y, &w, &h );
					//strange bug where features in external ROI's are being detected. 
					//tried to call clearHaarItems() between blobs but this causes no detections at all
					//work around for now is ensure the rectangle is contained in the ROI
					if(roi.x < x && roi.y < y && roi.width > w && roi.height > h){
						//mult by two since haar finder is looking at a 320x240 sample
						x /= TRACKING_SCALE_FACTOR;
						y /= TRACKING_SCALE_FACTOR;
						w /= TRACKING_SCALE_FACTOR;
						h /= TRACKING_SCALE_FACTOR;
						p.setHaarRect(ofRectangle(x, y, w, h));
						bHaarItemSet = true;
						break;	//only allow first haar item
					}
				}
			}
			//flag that we missed a frame
			if(!bHaarItemSet){
				p.noHaarThisFrame();
			}
		}
		
		//communicate tuio changes
		if(bTuioEnabled){
			for (int i = 0; i < trackedPeople.size(); i++){
				ofxCYAPerson p = trackedPeople[i];
				if(bUseHaarAsCenter && p.hasHaarRect()){
					ofPoint tuioCursor = p.getHaarCentroidNormalized(width, height);
					tuioClient.cursorDragged( tuioCursor.x, tuioCursor.y, p.oid);
				}
				else{
					ofPoint tuioCursor = p.getCentroidNormalized(width, height);
					tuioClient.cursorDragged( tuioCursor.x, tuioCursor.y, p.oid);
				}
			}
		}
		
		if(eventListener != NULL && (p.velocity.x != 0 || p.velocity.y != 0) ){
			eventListener->personMoved(p.pid);
		}
		
		//update living set
		stillLiving.push_back( p );		
	}
	
	if(bTuioEnabled){
		tuioClient.update();		
	}
	
	//make this the updated set
	trackedPeople = stillLiving;
	//store the old image
	grayLastImage = graySmallImage;	
}

#pragma mark Person Management
void ofxCYAPeopleTracker::blobOn( int x, int y, int id, int order )
{
	ofxCvTrackedBlob blob = persistentTracker.getById( id );
	trackedPeople.push_back( ofxCYAPerson(id, order, blob) );
	if(eventListener != NULL){
		eventListener->personEntered(id);
	}
	if(bTuioEnabled){
		tuioClient.cursorPressed(1.0*x/width, 1.0*y/height, order);
	}
}

void ofxCYAPeopleTracker::blobMoved( int x, int y, int id, int order ){/*notused*/}

void ofxCYAPeopleTracker::blobOff( int x, int y, int id, int order )
{
	//ensure we are tracking
	if(!isTrackingPerson(id)){
		printf("ofxPerson::warning. encountered persistent blob without a person behind them\n");		
		return;
	}
	
	//flag them for removal
	ofxCYAPerson p = getTrackedPerson(id);
//	p.shouldRemove = true;
	
	//alert the delegate
	if(eventListener != NULL){
		eventListener->personWillLeave(id);
	}
	if (bTuioEnabled) {
		ofPoint cursor = p.getCentroidNormalized(width, height);
		tuioClient.cursorReleased(cursor.x, cursor.y, order);	
	}
}

bool ofxCYAPeopleTracker::isTrackingPerson( int pid )
{
    for( int i = 0; i < trackedPeople.size(); i++ ) {
        if( trackedPeople[i].pid == pid ) {
            return true;
        }
    }
	return false;
}

ofxCYAPerson ofxCYAPeopleTracker::getTrackedPerson( int pid )
{
    for( int i = 0; i < trackedPeople.size(); i++ ) {
        if( trackedPeople[i].pid == pid ) {
            return trackedPeople[i];
        }
    }
}

#pragma mark Draw
void ofxCYAPeopleTracker::draw()
{
	draw(0,0);
}

void ofxCYAPeopleTracker::draw(int x, int y)
{
	draw(x,y,drawMode);
}

void ofxCYAPeopleTracker::draw(int x, int y, int mode)
{
	ofPushMatrix();{
		ofTranslate(x, y, 0);
		switch (mode) {
			case DRAW_MODE_GUI:
				gui.draw();
			case DRAW_MODE_NORMAL:
				ofPushMatrix();{
					//translate to make room for the gui
					ofTranslate(500, 75, 0);
					// draw the incoming, the grayscale, the bg and the thresholded difference
					ofSetColor(0xffffff);
					grayImage.draw(20,20,320,240);
					grayDiff.draw(360,20,320,240);
					grayBg.draw(20,280,320,240);
										
					//individually draw blobs and report findings
					ofPushMatrix();{
						ofTranslate(360,280);
						
						ofFill();
						ofSetColor(0x333333);
						ofRect(0,0,320,240);
						ofSetColor(0xffffff);
						
						ofNoFill();
						if (bTrackOpticalFlow){
							ofSetColor(0x888888);
							opticalFlow.draw(width*TRACKING_SCALE_FACTOR, height*TRACKING_SCALE_FACTOR);
						}					
						
						ofScale(TRACKING_SCALE_FACTOR, TRACKING_SCALE_FACTOR);
						contourFinder.draw();
					
						for (int i=0; i < trackedPeople.size(); i++){
							
							//draw blobs				
							//if haarfinder is looking at these blobs, draw the area it's looking at
							ofxCYAPerson p = trackedPeople[i];
							
							if(bTrackOpticalFlow){
								//purple optical flow arrow
								ofSetColor(0xff00ff);
								ofLine(p.centroid.x, p.centroid.y, p.centroid.x + p.opticalFlowVectorAccumulation.x, p.centroid.y + p.opticalFlowVectorAccumulation.y);
							}
							
							ofSetColor(0xffffff);							
							if(bDetectHaar){
								//draw haar search area expanded 
								ofRect(p.boundingRect.x - haarArea, 
									   p.boundingRect.y - haarArea, 
									   p.boundingRect.width  + haarArea*2, 
									   p.boundingRect.height + haarArea*2);
							}
							
							if(p.hasHaarRect()){
								//draw the haar rect in green
								ofSetColor(0x00ff00);
								ofRect(p.getHaarRect().x, p.getHaarRect().y, p.getHaarRect().width, p.getHaarRect().height);
								//haar-detected people get a yellow square
								ofSetColor(0xffff00);
							}
							else {
								//no haar gets a blue square
								ofSetColor(0x0000ff);
							}
							
							//draw person
							ofRect(p.boundingRect.x, p.boundingRect.y, p.boundingRect.width, p.boundingRect.height);
							
							//draw centroid
							ofSetColor(0xff0000);
							ofCircle(p.centroid.x, p.centroid.y, 3);
														
							//draw id
							ofSetColor(0xffffff);
							char idstr[1024];
							sprintf(idstr, "pid: %d\noid: %d\nage: %d", p.pid, p.oid, p.age );
							ofDrawBitmapString(idstr, p.centroid.x+8, p.centroid.y);													
						}
					}ofPopMatrix();	//release video feedback drawing
				} ofPopMatrix(); //release gui drawing
				break;
			case DRAW_MODE_CAMERA_CALIBRATE:
				//TODO implement calibration mode
				//vidGrabber.draw(ofGetWidth()- camWidth,0);			
				break;
			case DRAW_MODE_FULLSCREEN:
				ofSetColor(0xffffff);
				grayImage.draw(0,0,ofGetWidth(),ofGetHeight());
		
				break;
			default:
				printf("undefined draw mode: %d", mode);
				break;
		}
	}ofPopMatrix();
}

#pragma mark accessors

/**
 * simple public getter for external classes
 */
ofxCYAPerson ofxCYAPeopleTracker::personAtIndex(int i)
{
	return trackedPeople[i];
}

int ofxCYAPeopleTracker::totalPeople()
{
	return trackedPeople.size();
}

void ofxCYAPeopleTracker::enableHaarFeatures(bool doHaar)
{
	bDetectHaar = doHaar;
}

void ofxCYAPeopleTracker::enableOpticalFlow(bool doOpticalFlow)
{
	bTrackOpticalFlow = doOpticalFlow;
}

#pragma mark background management
void ofxCYAPeopleTracker::relearnBackground()
{
	bLearnBackground = true;
}

void ofxCYAPeopleTracker::enableBackgroundRelearnSmart(bool doSmartLearn)//auto-relearns if there are too many blobs in the scene
{
	bSmartLearnBackground = doSmartLearn;
}

void ofxCYAPeopleTracker::enableBackgroundReleaernProgressive(bool doProgressive) //relearns over time using progessive frame averagering
{
	bLearnBackgroundProgressive = doProgressive;
}

void ofxCYAPeopleTracker::setRelearnRate(float relearnRate)
{
	fLearnRate = relearnRate;
}


#pragma mark image control
void ofxCYAPeopleTracker::setThreshold(float thresholdAmount)
{
	threshold = thresholdAmount;
}

void ofxCYAPeopleTracker::setMinBlobSize(float minBlobSize)
{
	minBlob = minBlobSize; 
}

void ofxCYAPeopleTracker::setMaxBlobSize(float maxBlobSize)
{
	maxBlob = maxBlobSize;
}

void ofxCYAPeopleTracker::enableSmooth(bool doSmooth)
{
	bSmooth = doSmooth;
}

void ofxCYAPeopleTracker::setSmoothAmount(int smoothAmount)
{
	smooth = smoothAmount;
}

void ofxCYAPeopleTracker::enableHighpass(bool doHighpass)
{
	bHighpass = doHighpass;
}

void ofxCYAPeopleTracker::setHighpassBlurAmount(int highpassBlurAmount)
{
	highpassBlur = highpassBlurAmount;
}

void ofxCYAPeopleTracker::setHighpassNoiseAmount(int highpassNoiseAmount)
{
	highpassNoise = highpassNoiseAmount;
}

void ofxCYAPeopleTracker::enableAmplify(bool doAmp)
{
	bAmplify = doAmp;
}

void ofxCYAPeopleTracker::setAmplifyAmount(int amplifyAmount)
{
	highpassAmp = amplifyAmount;
}

#pragma mark filter controls
//haar
void ofxCYAPeopleTracker::setHaarExpandArea(float haarExpandAmount) //makes the haar rect +area bigger
{
	haarArea = haarExpandAmount;
}

void ofxCYAPeopleTracker::setMinHaarArea(float minArea)
{
	minHaarArea = minArea;
}
void ofxCYAPeopleTracker::setMaxHaarArea(float maxArea)
{
	maxHaarArea = maxArea;
}

void ofxCYAPeopleTracker::useHaarAsCentroid(bool useHaarCenter)
{
	bUseHaarAsCenter = useHaarCenter;
}

//blobs
void ofxCYAPeopleTracker::enableFindHoles(bool findHoles)
{
	bFindHoles = findHoles;
}

void ofxCYAPeopleTracker::trackDarkBlobs()
{
	bTrackDark = true;
}

void ofxCYAPeopleTracker::trackLightBlobs()
{
	bTrackDark = false;	
}

void ofxCYAPeopleTracker::setDrawMode(int mode)
{
	drawMode = mode;
}

int ofxCYAPeopleTracker::getDrawMode() 
{
	return drawMode;
}
