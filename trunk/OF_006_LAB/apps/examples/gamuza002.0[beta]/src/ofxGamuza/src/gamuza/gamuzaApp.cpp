#include "gamuzaApp.h"


//--------------------------------------------------------------
void gamuzaApp::setup(){

	//////////////////////////////////////////////
	// init general settings
	ofSeedRandom();
	ofSetFrameRate(FPS);
	ofSetVerticalSync(true);
	ofEnableSmoothing();
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// init openGL settings
	glDepthMask(false);						// disable writing to depth buffer
	glDisable(GL_DEPTH_TEST);				// disable sorting
	glEnable(GL_BLEND);						// enable blending
	glBlendFunc(GL_ONE, GL_ONE);			// set to additive (ignoring all alpha values)
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// get host machine data for OSC sending data
	// (could be localhost or another machine connected to the same ethernet)
	getIPfromXML();
	sender.setup(host_number.c_str(),atoi(host_port.c_str()));
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// init true type font
	automatFont.loadFont("automat.ttf",6, true, true);
	//////////////////////////////////////////////

	workingW = 320;
	workingH = 240;
	totPixels = workingW*workingH;

	#ifdef TARGET_LINUX
		linuxFullScreen = false;
    #endif

	frameCounter = 0;

	firstScreenW = FSWIDTH;
	firstScreenH = FSHEIGHT;
	secondScreenW = SSWIDTH;
	secondScreenH = SSHEIGHT;


    vidGrabber.setVerbose(true);
    vidGrabber.listDevices();
    vidGrabber.setDeviceID(0);
    vidGrabber.initGrabber(workingW,workingH);


    colorImg.allocate(workingW,workingH);						// Live Cam as input source
	grayImg.allocate(workingW,workingH);						// B&W lve Cam
	colorImgWarped.allocate(workingW,workingH);					// warping input for adjust tracking area

	colorImgHSV.allocate(workingW,workingH);					// HSV image
	hueImg.allocate(workingW,workingH);							// Hue Image
    satImg.allocate(workingW,workingH);							// Saturation Image
    valImg.allocate(workingW,workingH);							// value Image
	hsvTrackedPixels = new unsigned char[totPixels];			// Hue pixels
	huePixels = new unsigned char[totPixels];					// Sat pixels
	satPixels = new unsigned char[totPixels];					// Val pixels
	valPixels = new unsigned char[totPixels];					// HSV tracked image
	hsvTracking.allocate(workingW,workingH);					// resulting texture from HSV tracking

	colorBg.allocate(workingW,workingH);						// background color image
	savedBG.allocate(workingW,workingH,OF_IMAGE_COLOR);
	colorDiff.allocate(workingW,workingH);						// background subtraction over RGB input image
	resPixels = new unsigned char[totPixels];					// single channel - b & w
	pixels1 = new unsigned char[totPixels];
	pixels2 = new unsigned char[totPixels];
	resPixels = new unsigned char[totPixels];

	grayBg.allocate(workingW,workingH);							// background b&w image
	savedGrayBG.allocate(workingW,workingH,OF_IMAGE_GRAYSCALE);
	grayThresh.allocate(workingW,workingH);

	balancedTracking.allocate(workingW,workingH);				// balanced tracking from back subtraction over color & HSV tracking together
	alg1 = new unsigned char[totPixels];
	alg2 = new unsigned char[totPixels];
	balPixels = new unsigned char[totPixels];

	grayPrev.allocate(workingW,workingH);						// previous frame for motion detection
	grayNow.allocate(workingW,workingH);						// actual frame for motion detection
	motionImg.allocate(workingW,workingH);						// motion detection image
	motion = new unsigned char[totPixels];

	mhiImageFloat = new float[totPixels];
	mhiImageByte = new unsigned char[totPixels];
	historyImg.allocate(workingW,workingH);						// history image

	opticalFlow.allocate(workingW,workingH);

	blackPixels = new unsigned char[totPixels];					// black image
	for(int b=0;b<workingW*workingH;b++){
		blackPixels[b] = 0;
	}

	//////////////////////////////////////////////
	// contour finder
	contourReg = new vector<ofxPoint2f>[MAX_NUM_CONTOURS_TO_FIND];
	contourSmooth = new vector<ofxPoint2f>[MAX_NUM_CONTOURS_TO_FIND];
	contourSimple = new vector<ofxPoint2f>[MAX_NUM_CONTOURS_TO_FIND];
	contourHull = new vector<ofxPoint2f>[MAX_NUM_CONTOURS_TO_FIND];
	box = new CvBox2D32f[MAX_NUM_CONTOURS_TO_FIND];
	blobAngle = new float[MAX_NUM_CONTOURS_TO_FIND];
	_s_blobInfo = new CvBox2D32f[MAX_NUM_CONTOURS_TO_FIND];
	_osc_blobInfo = new CvBox2D32f[MAX_NUM_CONTOURS_TO_FIND];
	smoothPct = 0.13f;
	tolerance = 20.0f;

	// contour viewer
    bShowEllipse				= true;
    bShowAngle					= true;
	bShowLines					= true;
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// recursive centroids
	depth = CENTROID_DEPTH;
	recursiveCentroid = new ofRecursiveCentroids(depth,workingW,workingH);
	centroidNum = pow(2,depth-1); // 16 with depth = 5
	for(int c=0;c<centroidNum;c++){
		_s_centPos.push_back(ofPoint(0.0f,0.0f,0.0f));
		_osc_centPos.push_back(ofPoint(0.0f,0.0f,0.0f));
	}
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// optical flow
	_s_opfVel = new ofxPoint2f[OPTICAL_FLOW_GRID_X*OPTICAL_FLOW_GRID_Y];
	_osc_opfVel = new ofxPoint2f[OPTICAL_FLOW_GRID_X*OPTICAL_FLOW_GRID_Y];
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// face tracking
	//load in our face xml file
	haarFinder.setup("haarXML/haarcascade_frontalface_alt2.xml");
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// finger/hands tracking
	fFound						= false;
	hFound						= false;
	runningBlobs				= 0;
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// end texture/projection
	finalTexture.allocate(secondScreenW,secondScreenH, false);		// allocate memory for deforming final texture
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// set gui vars values
	sourcePoints  = new ofxPoint2f[4];
	sourcePoints[0] = ofxPoint2f(0.0f, 0.0f);
	sourcePoints[1] = ofxPoint2f(1.0f, 0.0f);
	sourcePoints[2] = ofxPoint2f(1.0f, 1.0f);
	sourcePoints[3] = ofxPoint2f(1.0f, 0.0f);

	warpMult = new ofxPoint2f[4];
	warpMult[0] = ofxPoint2f(0.0f,0.0f);
	warpMult[1] = ofxPoint2f(340.0f,0.0f);
	warpMult[2] = ofxPoint2f(340.0f,260.0f);
	warpMult[3] = ofxPoint2f(0.0f,260.0f);

	destPoints  = new ofxPoint2f[4];
	destPoints[0] = ofxPoint2f(20.0f, 20.0f);
	destPoints[1] = ofxPoint2f(340.0f, 20.0f);
	destPoints[2] = ofxPoint2f(340.0f, 260.0f);
	destPoints[3] = ofxPoint2f(20.0f, 260.0f);

	inputPoints  = new ofxPoint2f[4];
	inputPoints[0] = ofxPoint2f(0.0f, 0.0f);
	inputPoints[1] = ofxPoint2f(1.0, 0.0f);
	inputPoints[2] = ofxPoint2f(1.0, 1.0);
	inputPoints[3] = ofxPoint2f(0.0f, 1.0);

	cropPoints  = new ofxPoint2f[4];
	cropPoints[0] = ofxPoint2f(0.0f, 0.0f);
	cropPoints[1] = ofxPoint2f(1.0, 0.0f);
	cropPoints[2] = ofxPoint2f(1.0, 1.0);
	cropPoints[3] = ofxPoint2f(0.0f, 1.0);

	cropMult  = new ofxPoint2f[4];
	cropMult[0] = ofxPoint2f(0.0f, 0.0f);
	cropMult[1] = ofxPoint2f(secondScreenW*1.0f, 0.0f);
	cropMult[2] = ofxPoint2f(secondScreenW*1.0f, secondScreenH*1.0f);
	cropMult[3] = ofxPoint2f(0.0f, secondScreenH*1.0);

	saveXML = false;
	showGui = true;
	lightGui = false;
	sendingSocketReady = false;

	bLearnBackground = false;
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// background gamuza graphics
	backgroundGraphics.loadImage("background.png");
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// loading tracking backgrounds (color and b&w)
	loadBG.loadImage(TRACKING_BG);
	colorBg.setFromPixels(loadBG.getPixels(),workingW,workingH);
	loadGrayBG.loadImage(TRACKING_BW_BG);
	grayBg.setFromPixels(loadGrayBG.getPixels(),workingW,workingH);
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// create ofxGui instance
	gui = ofxGui::Instance(this);

	// load gui from xml
	gui->buildFromXml(OFXGUI_XML);
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// get info from gui xml for warping points
	getXmlInfo();
	//////////////////////////////////////////////

	///////////////////////////////////////////////////////////////
	// FRAME GRABBER
	frameGrab = new FrameGrabber("frames/export", 25, false, firstScreenW, 0, secondScreenW, secondScreenH);
	startGrab = false;
	///////////////////////////////////////////////////////////////

}

//--------------------------------------------------------------
void gamuzaApp::update(){


	if(showGui){
		ofBackground(20,20,20);
		ofShowCursor();
	}else{
		ofBackground(0,0,0);
		ofHideCursor();
	}
	ofSetWindowTitle("GAMUZA[0.2]   <http://gamuza.d3cod3.org>");

	//////////////////////////////////////////////
	// scale warping&crop points from GUI
	for (int j = 0; j < 4; j++){
		warpMult[j].x = sourcePoints[j].x*320.0f + 20.0;
		warpMult[j].y = sourcePoints[j].y*240.0f + 20.0;
		cropMult[j].x = cropPoints[j].x*secondScreenW;
		cropMult[j].y = cropPoints[j].y*secondScreenH;
	}
	//////////////////////////////////////////////

    bool bNewFrame = false;

    vidGrabber.grabFrame();
    bNewFrame = vidGrabber.isFrameNew();

	if(bNewFrame){

        colorImg.setFromPixels(vidGrabber.getPixels(), workingW,workingH);

		colorImg.mirror(false,true); // invert horizontally source image

		colorImgWarped = colorImg;
		colorImgWarped.warpIntoMe(colorImg, warpMult, destPoints);

		grayImg = colorImgWarped;

		//////////////////////////////////////////////
		// background learning
		if (bLearnBackground == true){
			colorBg = colorImgWarped;
			grayBg = grayImg;
			savedBG.setFromPixels(colorBg.getPixels(),workingW,workingH,OF_IMAGE_COLOR);
			savedBG.saveImage(TRACKING_BG);
			savedGrayBG.setFromPixels(grayBg.getPixels(),workingW,workingH,OF_IMAGE_GRAYSCALE);
			savedGrayBG.saveImage(TRACKING_BW_BG);
			bLearnBackground = false;
		}
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		// face tracking
		if(computeFaceTracking){
			haarFinder.findHaarObjects(grayImg, 100, 99999999, MAX_NUM_CONTOURS_TO_FIND);
			_s_ftInfo.clear();
			_s_ftInfo.assign(haarFinder.blobs.size(), ofxPoint4f());
			_osc_ftInfo.clear();
			_osc_ftInfo.assign(haarFinder.blobs.size(), ofxPoint4f());
		}
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		// background subtraction
		if(bgSubTech == 0){
			calculateColorDifference();
		}else{
			calculateGrayscaleDifference();
		}
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		// tracking color difference over time using HSV color information
		calculateColorDifferenceHSV();
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		// calculate hsv color range
		hsvViewer.calcColorRange(hue, hueWidth, sat, satWidth, val);
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		// filter information from the two different algorithms
		balanceTracking();
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		// motion detection over background subtraction
		calculateMotion();
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		// draw image history from motion detection
		if(computeMotionHistory){
			generateHistory(motionImg.getPixels(), dRate);
		}
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		// calculate recursive centroids
		if(computeRecursiveCentroid){
			recursiveCentroid->computeCentroidThreaded(balancedTracking.getPixels());
		}
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		// find contours
		if(computeContourFinder){
			runningBlobs = contourFinder.findContours(balancedTracking, 100, 9999999, MAX_NUM_CONTOURS_TO_FIND, false, false);
			if(runningBlobs > 0){
				computeContourAnalysis();
			}
			// fingers/hands tracking (depends from contour analysis)
			if(computeFingerTracking && runningBlobs > 0){
				fFound		= false;
				hFound		= false;
				fFound		= fingerFinder.findFingers(contourFinder.blobs[0]);
				hFound		= fingerFinder.findHands(contourFinder.blobs[0]);
				/////////////////////////////////////////////////////////////////////////////////
				///////////////////////////
				/////////////////////////// something wrong here with finger positions
				fingerPos.clear();
				fingerPos.assign(fingerFinder.ppico.size(),ofxPoint2f());
				fingerPos = fingerFinder.ppico;
				_s_fingerPos.clear();
				_s_fingerPos.assign(fingerFinder.ppico.size(),ofxPoint2f());
				_osc_fingerPos.clear();
				_osc_fingerPos.assign(fingerFinder.ppico.size(),ofxPoint2f());
				///////////////////////////
				///////////////////////////
				/////////////////////////////////////////////////////////////////////////////////
			}
		}
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		// smoothing & normalize numerical variables(prepare it for sending via OSC)
		smoothingValues();
		normalizeValues();
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		// send OSC data
		if(sendingSocketReady){
			sendOscData();
		}
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		// Grab frame sequence of final projection
		if(startGrab){
			frameGrab->grabFrame();
		}
		//////////////////////////////////////////////

	}

	//printf("%f \n", _s_opfVel[10].x);
}

//--------------------------------------------------------------
void gamuzaApp::draw(){

	ofSetColor(0xffffff);

	if(showGui){
		colorImg.draw(20,20);
		colorBg.draw(20,280, 160, 120);
		colorImgWarped.draw(180,280,160,120);
		if(!lightGui){
			if(bgSubTech == 0){
				colorDiff.draw(360,20,160,120);
			}else{
				grayThresh.draw(360,20,160,120);
			}
			colorImgHSV.draw(520,20,160,120);
			balancedTracking.draw(360,150,160,120);
			hsvTracking.draw(520,150,160,120);
			if(sendingSocketReady){
				motionImg.draw(20,410, 160, 120); // motion detection from balanced tracking
			}
		}
		if(computeMotionHistory && sendingSocketReady){
			historyImg.draw(180,410,160,120); // image history
		}

		//////////////////////////////////////////////////
		// recursive centroid
		if(computeRecursiveCentroid && sendingSocketReady){
			glPushMatrix();
				glTranslatef(520, 280, 0);
				glScalef(0.5,0.5,0);

				ofFill();
				ofSetColor(0x000000);
				ofRect(0,0,320,240);
				ofSetColor(0xffffff);

				recursiveCentroid->draw();
			glPopMatrix();
		}
		//////////////////////////////////////////////////

		//////////////////////////////////////////////////
		// face tracking
		if(computeFaceTracking && sendingSocketReady){
			numFace = haarFinder.blobs.size();
			glPushMatrix();
				glTranslatef(180, 280, 0);
				glScalef(0.5,0.5,0);
				for(int i = 0; i < numFace; i++){
					float x = haarFinder.blobs[i].boundingRect.x;
					float y = haarFinder.blobs[i].boundingRect.y;
					float w = haarFinder.blobs[i].boundingRect.width;
					float h = haarFinder.blobs[i].boundingRect.height;

					//float cx = haarFinder.blobs[i].centroid.x;
					//float cy = haarFinder.blobs[i].centroid.y;

					ofNoFill();
					ofSetColor(0xFF0000);
					ofRect(x, y, w, h);
				}
			glPopMatrix();
		}
		//////////////////////////////////////////////////

		// Draw lines between warping points (input source)
		ofSetColor(0x333333);
		ofNoFill();
		ofBeginShape();
		for (int j = 0; j < 4; j++){
			ofVertex(warpMult[j].x, warpMult[j].y);
		}
		ofEndShape(true);

		// Draw warping points (input source)
		ofFill();
		ofSetColor(0xFF0000);
		for (int j = 0; j < 4; j++){
			ofCircle(warpMult[j].x, warpMult[j].y, 3);
		}

		// draw motion detection centers mass
		if(!lightGui && sendingSocketReady){
			glPushMatrix();
				glTranslatef(20, 410, 0);
				glScalef(0.5,0.5,0);
				ofSetColor(0xFF0000);
				ofNoFill();
				ofRect(_s_MDCX1 - (_s_MDQ*0.01),_s_MDCY1 - (_s_MDQ*0.01),_s_MDQ*0.02,_s_MDQ*0.02);
				ofSetColor(0x8aff00);
				ofNoFill();
				ofRect(_s_MDCX2 - (_s_MDQ*0.01),_s_MDCY2 - (_s_MDQ*0.01),_s_MDQ*0.02,_s_MDQ*0.02);
				ofSetColor(0xFFFFFF);
				ofLine(_s_MDCX1,_s_MDCY1,_s_MDCX2,_s_MDCY2);
			glPopMatrix();
		}

		// blob detection + contours
		if(computeContourFinder && sendingSocketReady){
			drawContourAnalysis();
		}

		// optical flow LK
		if(computeOpticalFlow && sendingSocketReady){
			glPushMatrix();
				glTranslatef(520, 410, 0);
				glScalef(0.5,0.5,0);
				ofFill();
				ofSetColor(0x333333);
				ofRect(0,0,320,240);
				ofSetColor(0xffffff);
				opticalFlow.draw();
			glPopMatrix();
		}

		// finger/hand tracking
		if(computeFingerTracking && sendingSocketReady){
			glPushMatrix();
				glTranslatef(180,280, 0);
				glScalef(0.5,0.5,0);
				if(fFound){
					fingerFinder.draw(0,0);
				}
				if(hFound){
					fingerFinder.drawhands(0,0);
				}
			glPopMatrix();
		}

	}

	//////////////////////////////////////////////////
	// Draw video into the finalTexture
    finalTexture.swapIn();
    finalTexture.setupScreenForMe();

	ofSetColor(255, 255, 255, 255);

	drawIntoFBO();

    finalTexture.swapOut();
    finalTexture.setupScreenForThem();

	ofSetColor(255, 255, 255, 255);

	glPushMatrix();
    glTranslatef(firstScreenW, 0, 0);
		finalTexture.setPoints(inputPoints,cropMult);
		finalTexture.draw();
	glPopMatrix();
	//////////////////////////////////////////////////


	if(showGui){
		// GUI texts
		char reportStr[1024];
		if(!sendingSocketReady){
			ofSetColor(0xff0000);
			sprintf(reportStr, "Preparing OSC Sending Socket %i%% ",(int)(ofNormalize(_s_MDCX1,-80000000.0f,0.0f)*99));
			automatFont.drawString(reportStr, 360,749);
			ofSetColor(0xffffff);
			ofLine(361,753,361 + (ofNormalize(_s_MDCX1,-80000000.0f,0.0f)*156),753);
		}else{
			ofSetColor(0xffffff);
			sprintf(reportStr, "OSC Sending Socket Ready");
			automatFont.drawString(reportStr, 360,749);
			ofSetColor(0xff0000);
			sprintf(reportStr, "Sending OSC data to host %s Port %s", host_number.c_str(),host_port.c_str());
			automatFont.drawString(reportStr, 360,761);
		}

		// draw background image
		ofSetColor(0xFFFFFF);
		ofEnableAlphaBlending();
		backgroundGraphics.draw(0,0);
		ofDisableAlphaBlending();

		// Graphical user interface draw
		gui->draw();

		// draw hsv viewer
		hsvViewer.drawColorRange(984, 47, 120, 20);
	}

}


//--------------------------------------------------------------
void gamuzaApp::keyPressed  (int key){

	switch (key){
		case ' ':
			bLearnBackground = true;
			break;

		case 's':
			vidGrabber.videoSettings();
			break;

		case 'f':
            //--------------------------------------------------------------------
			#ifdef TARGET_OSX
			//--------------------------------------------------------------------
			ofToggleFullscreen();
			printf("fullscreen OSX\n");
			//--------------------------------------------------------------------
			#endif
			//--------------------------------------------------------------------

            ////////////////////////////////////////////////////////////////////////////////////
            /////////FULLSCREEN LINUX IS NOT WORKING WITH COMPIZ FUSION///////////////////////// linux fullscreen BUG
			//--------------------------------------------------------------------
			#ifdef TARGET_LINUX
			//--------------------------------------------------------------------
			linuxFullScreen = !linuxFullScreen;

			if(linuxFullScreen){
                ofSetWindowPosition(0,0);
                ofSetWindowShape(firstScreenW+secondScreenW,secondScreenH);
			}else{
                ofSetWindowPosition(20,20);
                ofSetWindowShape(1130,780);
			}

			//--------------------------------------------------------------------
			#endif
			//--------------------------------------------------------------------
			////////////////////////////////////////////////////////////////////////////////////linux fullscreen BUG
			////////////////////////////////////////////////////////////////////////////////////
			break;

		case 'g':
			showGui = !showGui;
			break;

		case 'p':
			startGrab = !startGrab;
			break;
	}
}

//--------------------------------------------------------------
void gamuzaApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void gamuzaApp::mouseDragged(int x, int y, int button){
	gui->mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void gamuzaApp::mousePressed(int x, int y, int button){
	gui->mousePressed(x, y, button);
}

//--------------------------------------------------------------
void gamuzaApp::mouseReleased(int x, int y, int button){
	gui->mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void gamuzaApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void gamuzaApp::calculateColorDifference(){

	int bpp = 3;
	pixels1 = colorImgWarped.getPixels();
	pixels2 = colorBg.getPixels();
	bool pixelDiff;

	for(int i=0; i < workingH; i++){
      for(int j=0; j < workingW; j++){
         pixelDiff = false;

         for(int b=0; b < bpp; b++){
            int diff = pixels1[(i*workingW+j)*bpp+b] - pixels2[(i*workingW+j)*bpp+b];
            diff = (diff < 0)?-diff:diff;
            if(diff > (int)threshold){
               pixelDiff = true;
               break;
            }
         }
         resPixels[i*workingW+j] = (pixelDiff)?255:0;
      }
   }
   colorDiff.setFromPixels(resPixels, workingW, workingH);
   colorDiff.blur(blur1);

}

//--------------------------------------------------------------
void gamuzaApp::calculateGrayscaleDifference(){

	if(bgSubTech == 1){ // absolute difference
		grayThresh = grayImg;
		grayThresh.absDiff(grayBg);
	}else if(bgSubTech == 2){ // lighter than
		grayThresh = grayImg;
		grayThresh -= grayBg;
	}else if(bgSubTech == 3){ // darker than
		grayThresh = grayBg;
		grayThresh -= grayImg;
	}

	grayThresh.threshold(threshold);
	grayThresh.blur(blur1);
}

//--------------------------------------------------------------
void gamuzaApp::calculateColorDifferenceHSV(){

	//Calculate min and max thersholds values
	minHue = max((hue - hueWidth*0.5) * 255, 0.0);
	maxHue = min((hue + hueWidth*0.5) * 255, 255.0);
	minSat = max((sat - satWidth*0.5) * 255, 0.0);
	maxSat = min((sat + satWidth*0.5) * 255, 255.0);
	minVal = max((val - valWidth*0.5) * 255, 0.0);
	maxVal = min((val + valWidth*0.5) * 255, 255.0);

	// HSV
	colorImgHSV = colorImgWarped;
	if(rgbORhsv == 1){
		colorImgHSV.convertRgbToHsv();
	}
	colorImgHSV.blur(HsvBlur);
	colorImgHSV.convertToGrayscalePlanarImages(hueImg, satImg, valImg);

	// Perform tracking calculations
	huePixels = hueImg.getPixels();
	satPixels = satImg.getPixels();
	valPixels = valImg.getPixels();

	for (int i = 0; i < totPixels; i++){
		if((huePixels[i] >= minHue && huePixels[i] <= maxHue) &&
			(satPixels[i] >= minSat && satPixels[i] <= maxSat) &&
			(valPixels[i] >= minVal && valPixels[i] <= maxVal)){
				hsvTrackedPixels[i] = 255;
		}else{
				hsvTrackedPixels[i] = 0;
		}
	}

	hsvTracking.setFromPixels(hsvTrackedPixels, workingW, workingH);
	hsvTracking.blur(blur2);

}

//--------------------------------------------------------------
void gamuzaApp::balanceTracking(){

	if(bgSubTech == 0){
		 alg1 = colorDiff.getPixels();
	}else{
		 alg1 = grayThresh.getPixels();
	}
	alg2 = hsvTracking.getPixels();

	for (int i = 0; i < totPixels; i++){
		if(alg1[i] >= (int)balance1 || alg2[i] >= (int)balance2){
			balPixels[i] = 255;
		}else{
			balPixels[i] = 0;
		}
	}

	balancedTracking.setFromPixels(balPixels, workingW, workingH);
}

//--------------------------------------------------------------
void gamuzaApp::calculateMotion(){

	if(frameCounter > 5){// dont do anything until we have enough in history

		grayNow = balancedTracking;

		if(computeOpticalFlow){
			opticalFlow.calc(grayPrev,grayNow,11); // optical flow analysis based on frame difference
			cvSmooth(opticalFlow.getVelX(), opticalFlow.getVelX(), CV_BLUR , CAM_VELOCITY_BLUR);
			cvSmooth(opticalFlow.getVelY(), opticalFlow.getVelY(), CV_BLUR , CAM_VELOCITY_BLUR);
		}

		motionImg.absDiff(grayPrev, grayNow);   // motionImg is the difference between current and previous frame
		cvThreshold(motionImg.getCvImage(), motionImg.getCvImage(), (int)mThreshold, 255, CV_THRESH_TOZERO); // anything below diffThreshold, drop to zero (compensate for noise)
		numPixelsChanged = motionImg.countNonZeroInRegion(0, 0, workingW, workingH); // this is how many pixels have changed that are above diffThreshold

		if(numPixelsChanged >= (int)mNoiseComp){ // noise compensation
			grayPrev = grayNow; // save current frame for next loop

			cvThreshold(motionImg.getCvImage(), motionImg.getCvImage(), (int)mThreshold, 255, CV_THRESH_TOZERO);// chop dark areas

			meanX = (_osc_MDCX1 + _osc_MDCX2) / 2;
			meanY = (_osc_MDCY1 + _osc_MDCY2) / 2;

		}else{
			motionImg.setFromPixels(blackPixels, workingW, workingH);
		}

		motion = motionImg.getPixels();

		//////////////////////////////////////////////////
		// get upper and lower middle points from motion detection
		for(int i = 0; i < totPixels; i++){
			if (motion[i] > (int)mThreshold){
				mCY = (int)i/workingW;//y position from [i] position in image vector
				mCX = i - mCY*workingW;// x position from [i] position in image vector
			}
		}
		for(int i = totPixels - 1; i > 0; i--){
			if (motion[i] > (int)mThreshold){
				mCY2 = (int)i/workingW;//y position from [i] position in image vector
				mCX2 = i - mCY2*workingW;// x position from [i] position in image vector
			}
		}

		//////////////////////////////////////////////////

	}else{
		frameCounter ++;
	}

}

//--------------------------------------------------------------
void gamuzaApp::generateHistory(unsigned char * input,float decreaseRate){

	for (int i = 0; i < totPixels; i++){
		mhiImageFloat[i] += input[i];

		if (mhiImageFloat[i] > 255){
			mhiImageFloat[i] = 255;
		}

		mhiImageFloat[i] *= decreaseRate;
		mhiImageByte[i] = (unsigned char)(mhiImageFloat[i]);
	}

	historyImg.setFromPixels(mhiImageByte,workingW,workingH);

}

//--------------------------------------------------------------
void gamuzaApp::computeContourAnalysis(){
	for (int i = 0; i < contourFinder.nBlobs; i++){

		int length_of_contour = contourFinder.blobs[i].pts.size();

		// ellipse
		fitEllipse(contourFinder.blobs[i].pts, box[i]);

		// angles
		blobAngle[i] =  getOrientation(contourFinder.blobs[i].pts);

		// lines
		findLines(contourFinder.blobs[i].pts,geomLines,30,40,30);

		// assign smoothed and normalized blobs geometry vectors vars
		_s_blobGeom.clear();
		_s_blobGeom.assign(geomLines.size(), ofxPoint4f());
		_osc_blobGeom.clear();
		_osc_blobGeom.assign(geomLines.size(), ofxPoint4f());

		// smoothed, simple and convexHull contours computing
		contourReg[i].clear();
		contourReg[i].assign(length_of_contour, ofxPoint2f());
		contourSmooth[i].clear();
		contourSmooth[i].assign(length_of_contour, ofxPoint2f());

		for(int j = 0; j < length_of_contour; j++){
			contourReg[i].at(j) = contourFinder.blobs[i].pts[j];
		}

		contourS.smooth(contourReg[i], contourSmooth[i], smoothPct);
		contourSimple[i].clear();

		contourS.simplify(contourSmooth[i], contourSimple[i], tolerance);
		contourS.convexHull(contourSimple[i], contourHull[i]);

	}
}

//--------------------------------------------------------------
void gamuzaApp::drawContourAnalysis(){
	glPushMatrix();
		glTranslatef(360, 280, 0);
		glScalef(0.5,0.5,0.0);

		ofFill();
		ofSetColor(0x333333);
		ofRect(0,0,320,240);
		ofSetColor(0xffffff);

		for (int i = 0; i < contourFinder.nBlobs; i++){

			//-------------------  draw the contours
			if(cfDetail == 0){
				ofSetColor(0xff0000);
				ofNoFill();
				drawBlob(0,0,contourFinder.blobs[i]);
			}else if(cfDetail == 1){
				ofSetColor(0xff0000);
				ofNoFill();
				ofBeginShape();
				for(unsigned int j = 0; j < contourSmooth[i].size(); j++){
					ofVertex(contourSmooth[i].at(j).x, contourSmooth[i].at(j).y);
				}
				ofEndShape(true);
			}else if(cfDetail == 2){
				ofSetColor(0xff0000);
				ofNoFill();
				ofBeginShape();
				for(unsigned int k = 0; k < contourSimple[i].size(); k++){
					ofVertex(contourSimple[i].at(k).x, contourSimple[i].at(k).y);
				}
				ofEndShape(true);
			}else if(cfDetail == 3){
				ofSetColor(0x220000);
				ofNoFill();
				ofBeginShape();
				for(unsigned int z = 0; z < contourHull[i].size(); z++){
					ofVertex(contourHull[i].at(z).x, contourHull[i].at(z).y);
				}
				ofEndShape(true);
				for(unsigned int w = 0; w < contourHull[i].size(); w++){
					ofCircle(contourHull[i].at(w).x, contourHull[i].at(w).y, 2);
				}
			}
			//------------------- fit ellipse to that blob and draw it
			if(bShowEllipse){

				ofNoFill();
				ofSetColor(0xffffff);

				glPushMatrix();
					glTranslatef(box[i].center.x, box[i].center.y,0);
					glRotatef(box[i].angle,0,0,1);
					ofEllipse( 0,0, box[i].size.width*.5,box[i].size.height*.5);
				glPopMatrix();
			}
			//------------------- fit angle of orientation
			if(bShowAngle){
				ofSetColor(0xffffff);

				float x1,y1,x2,y2;

				x1 = contourFinder.blobs[i].centroid.x + 25 * cos(blobAngle[i]);
				y1 = contourFinder.blobs[i].centroid.y + 25 * sin(blobAngle[i]);
				x2 = contourFinder.blobs[i].centroid.x - 25 * cos(blobAngle[i]);
				y2 = contourFinder.blobs[i].centroid.y - 25 * sin(blobAngle[i]);

				glPushMatrix();
					glScalef(0.5,0.5,0.0);
					ofLine(x1*2,y1*2,x2*2,y2*2);
				glPopMatrix();

				x1 = contourFinder.blobs[i].centroid.x + 10 * cos(blobAngle[i]+HALF_PI);
				y1 = contourFinder.blobs[i].centroid.y + 10 * sin(blobAngle[i]+HALF_PI);
				x2 = contourFinder.blobs[i].centroid.x - 10 * cos(blobAngle[i]+HALF_PI);
				y2 = contourFinder.blobs[i].centroid.y - 10 * sin(blobAngle[i]+HALF_PI);
				glPushMatrix();
					glScalef(0.5,0.5,0.0);
					ofLine(x1*2,y1*2,x2*2,y2*2);
				glPopMatrix();
			}
			//------------------- fit geometry lines on countour
			if(bShowLines){
				ofSetColor(0x8aff00);
				ofNoFill();
				for(unsigned int j = 0; j < geomLines.size(); j++){
					ofLine(geomLines[j].x,geomLines[j].y,geomLines[j].z,geomLines[j].w);
					ofCircle(geomLines[j].x,geomLines[j].y,3);
					ofCircle(geomLines[j].z,geomLines[j].w,3);
				}
			}
		}
	glPopMatrix();
}

//--------------------------------------------------------------
void gamuzaApp::drawIntoFBO(){
	glDisable(GL_BLEND);
	//..................
	// draw things that needs blending
	//..................
	glEnable(GL_BLEND);

	ofFill();
	ofSetColor(0xFF0000);

	glPushMatrix();
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        grayImg.draw(0,0,secondScreenW,secondScreenH);
	glPopMatrix();
}

//--------------------------------------------------------------
void gamuzaApp::getVelAtPixel(int x, int y, float *u, float *v) {
	*u = cvGetReal2D( opticalFlow.getVelX(), y, x );
	*v = cvGetReal2D( opticalFlow.getVelY(), y, x );
}

//--------------------------------------------------------------
void gamuzaApp::getVelAtNorm(float x, float y, float *u, float *v) {
	int ix = x * workingW;
	int iy = y * workingH;
	if(ix<0) ix = 0; else if(ix>=workingW) ix = workingW - 1;
	if(iy<0) iy = 0; else if(iy>=workingH) iy = workingH - 1;
	*u = cvGetReal2D( opticalFlow.getVelX(), iy, ix );
	*v = cvGetReal2D( opticalFlow.getVelY(), iy, ix );
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// In a race, the quickest runner can never overtake the slowest,
// since the pursuer must first reach the point whence the pursued started,
// so that the slower must always hold a lead.
//
//—Aristotle, Physics VI:9, 239b15
// http://en.wikipedia.org/wiki/Zeno%27s_paradoxes#Achilles_and_the_tortoise
//--------------------------------------------------------------
void gamuzaApp::smoothingValues(){

	// motion detection centers coords.
	_s_MDCX1 = _s_MDCX1*0.95f + 0.05f*(mCX/1.0f);
 	_s_MDCX2 = _s_MDCX2*0.95f + 0.05f*(mCX2/1.0f);
	_s_MDCY1 = _s_MDCY1*0.95f + 0.05f*(mCY/1.0f);
	_s_MDCY2 = _s_MDCY2*0.95f + 0.05f*(mCY2/1.0f);

	// motion quantity
	_s_MDQ = _s_MDQ*0.95f + 0.05f*numPixelsChanged;

	// blobs info from contour finder
	for(int b=0;b<contourFinder.nBlobs;b++){
		_s_blobInfo[b].center.x		= _s_blobInfo[b].center.x*0.95 + box[b].center.x*0.05;
		_s_blobInfo[b].center.y		= _s_blobInfo[b].center.y*0.95 + box[b].center.y*0.05;
		_s_blobInfo[b].angle		= _s_blobInfo[b].angle*0.95 + box[b].angle*0.05;
		_s_blobInfo[b].size.width	= _s_blobInfo[b].size.width*0.95 + box[b].size.width*0.05;
		_s_blobInfo[b].size.height	= _s_blobInfo[b].size.height*0.95 + box[b].size.height*0.05;
	}
	for(unsigned int g = 0; g < geomLines.size(); g++){
		_s_blobGeom[g].x = _s_blobGeom[g].x*0.95 + geomLines[g].x*0.05;
		_s_blobGeom[g].y = _s_blobGeom[g].y*0.95 + geomLines[g].y*0.05;
		_s_blobGeom[g].z = _s_blobGeom[g].z*0.95 + geomLines[g].z*0.05;
		_s_blobGeom[g].w = _s_blobGeom[g].w*0.95 + geomLines[g].w*0.05;
	}

	// recursive centroid positions
	for(int c=0;c<centroidNum;c++){
		_s_centPos[c].x = _s_centPos[c].x*0.95 + recursiveCentroid->cent[c].posx*0.05;
		_s_centPos[c].y = _s_centPos[c].y*0.95 + recursiveCentroid->cent[c].posy*0.05;
	}

	// optical flow pixels velocity
	for(int o=0;o<OPTICAL_FLOW_GRID_Y;o++){
		for(int oo=0;oo<OPTICAL_FLOW_GRID_X;oo++){
		    float _sdx,_sdy;
		    getVelAtNorm(oo, o, &_sdx, &_sdy);
			_s_opfVel[oo+(OPTICAL_FLOW_GRID_X*o)].x = _s_opfVel[oo+(OPTICAL_FLOW_GRID_X*o)].x*0.95 + _sdx*0.05;
			_s_opfVel[oo+(OPTICAL_FLOW_GRID_X*o)].y = _s_opfVel[oo+(OPTICAL_FLOW_GRID_X*o)].y*0.95 + _sdy*0.05;
		}
	}

	// face tracking blobs positions
	for(int f = 0; f < numFace; f++){
		_s_ftInfo[f].x = _s_ftInfo[f].x*0.95 + haarFinder.blobs[f].centroid.x*0.05;
		_s_ftInfo[f].y = _s_ftInfo[f].y*0.95 + haarFinder.blobs[f].centroid.y*0.05;
		_s_ftInfo[f].z = _s_ftInfo[f].z*0.95 + haarFinder.blobs[f].boundingRect.width*0.05;
		_s_ftInfo[f].w = _s_ftInfo[f].w*0.95 + haarFinder.blobs[f].boundingRect.height*0.05;
	}

	// fingers tracking positions
	if(fFound){
		for(unsigned int h = 0; h < fingerPos.size(); h++){
			_s_fingerPos[h].x = _s_fingerPos[h].x*0.95 + fingerPos[h].x*0.05;
			_s_fingerPos[h].y = _s_fingerPos[h].y*0.95 + fingerPos[h].y*0.05;
		}
	}

}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------
void gamuzaApp::normalizeValues(){

	// motion detection centers coords.
	_osc_MDCX1 = ofNormalize(_s_MDCX1,0.0f,320.0f);
	_osc_MDCX2 = ofNormalize(_s_MDCX2,0.0f,320.0f);
	_osc_MDCY1 = ofNormalize(_s_MDCY1,0.0f,240.0f);
	_osc_MDCY2 = ofNormalize(_s_MDCY2,0.0f,240.0f);

	// motion quantity
	_osc_MDQ = ofNormalize(_s_MDQ,mNoiseComp,totPixels);

	// blobs info from contour finder
	for(int b=0;b<contourFinder.nBlobs;b++){
		_osc_blobInfo[b].center.x = ofNormalize(_s_blobInfo[b].center.x,0.0f,320.0f);
		_osc_blobInfo[b].center.y = ofNormalize(_s_blobInfo[b].center.y,0.0f,240.0f);
		_osc_blobInfo[b].angle = ofNormalize(_s_blobInfo[b].angle,0.0f,360.0f);
		_osc_blobInfo[b].size.width = ofNormalize(_s_blobInfo[b].size.width,0.0f,320.0f);
		_osc_blobInfo[b].size.height = ofNormalize(_s_blobInfo[b].size.height,0.0f,240.0f);
	}
	for(unsigned int g = 0; g < geomLines.size(); g++){
		_osc_blobGeom[g].x = ofNormalize(_s_blobGeom[g].x,0.0f,320.0f);
		_osc_blobGeom[g].y = ofNormalize(_s_blobGeom[g].y,0.0f,240.0f);
		_osc_blobGeom[g].z = ofNormalize(_s_blobGeom[g].z,0.0f,320.0f);
		_osc_blobGeom[g].w = ofNormalize(_s_blobGeom[g].w,0.0f,240.0f);
	}

	// recursive centroid positions
	for(int c=0;c<centroidNum;c++){
		_osc_centPos[c].x = ofNormalize(_s_centPos[c].x,0.0f,320.0f);
		_osc_centPos[c].y = ofNormalize(_s_centPos[c].y,0.0f,240.0f);
	}

	// optical flow pixels velocity
	for(int o=0;o<OPTICAL_FLOW_GRID_X*OPTICAL_FLOW_GRID_Y;o++){
		_osc_opfVel[o] = _s_opfVel[o];
	}

	// face tracking blobs positions
	for(int f = 0; f < numFace; f++){
		_osc_ftInfo[f].x = ofNormalize(_s_ftInfo[f].x,0.0f,320.0f);
		_osc_ftInfo[f].y = ofNormalize(_s_ftInfo[f].y,0.0f,240.0f);
		_osc_ftInfo[f].z = ofNormalize(_s_ftInfo[f].z,0.0f,320.0f);
		_osc_ftInfo[f].w = ofNormalize(_s_ftInfo[f].w,0.0f,240.0f);
	}

	// fingers tracking positions
	for(unsigned int h = 0; h < fingerPos.size(); h++){
		_osc_fingerPos[h].x = ofNormalize(_s_fingerPos[h].x,0.0f,320.0f);
		_osc_fingerPos[h].y = ofNormalize(_s_fingerPos[h].y,0.0f,240.0f);
	}

	if(_osc_MDQ > 0.0){
		sendingSocketReady = true; // start sending osc vars
	}

}
//--------------------------------------------------------------
void gamuzaApp::getXmlInfo(){

	// parallel load of xml settings
	guiXml.loadFile(ofToDataPath(OFXGUI_XML));

	guiXml.pushTag("UI");
	int nOfTags = guiXml.getNumTags("OBJECT"); // panels
	for(int t = 0; t < nOfTags; t++){
		guiXml.pushTag("OBJECT",t); // move through the panels level
		int nOfObj = guiXml.getNumTags("OBJECT");
		for(int q = 0; q < nOfObj; q++){
			guiXml.pushTag("OBJECT",q);
			if(guiXml.getValue("ID", 0) == 19){
				sourcePoints[0].set(guiXml.getValue("VALUE_X", 0.0f), guiXml.getValue("VALUE_Y", 0.0f));
			}
			if(guiXml.getValue("ID", 0) == 20){
				sourcePoints[1].set(guiXml.getValue("VALUE_X", 0.0f), guiXml.getValue("VALUE_Y", 0.0f));
			}
			if(guiXml.getValue("ID", 0) == 21){
				sourcePoints[2].set(guiXml.getValue("VALUE_X", 0.0f), guiXml.getValue("VALUE_Y", 0.0f));
			}
			if(guiXml.getValue("ID", 0) == 22){
				sourcePoints[3].set(guiXml.getValue("VALUE_X", 0.0f), guiXml.getValue("VALUE_Y", 0.0f));
			}
			if(guiXml.getValue("ID", 0) == 23){
				cropPoints[0].set(guiXml.getValue("VALUE_X", 0.0f), guiXml.getValue("VALUE_Y", 0.0f));
			}
			if(guiXml.getValue("ID", 0) == 24){
				cropPoints[1].set(guiXml.getValue("VALUE_X", 0.0f), guiXml.getValue("VALUE_Y", 0.0f));
			}
			if(guiXml.getValue("ID", 0) == 25){
				cropPoints[2].set(guiXml.getValue("VALUE_X", 0.0f), guiXml.getValue("VALUE_Y", 0.0f));
			}
			if(guiXml.getValue("ID", 0) == 26){
				cropPoints[3].set(guiXml.getValue("VALUE_X", 0.0f), guiXml.getValue("VALUE_Y", 0.0f));
			}
			guiXml.popTag();
		}

		guiXml.popTag(); // pop panels
	}
	guiXml.popTag(); // pop UI
}

//--------------------------------------------------
void gamuzaApp::getIPfromXML(){
	////////////////////////////////// local xml capture
	host_data.loadFile("ip_host.xml");
	host_number = host_data.getValue("host_ip"," ",0);
	host_port = host_data.getValue("host_port"," ",0);
}

//--------------------------------------------------
void gamuzaApp::sendOscData(){
	ofxOscMessage m;
	ofxOscBundle b;

	// sending motion detection data
	if(sendOsc_MD){
		m.setAddress("/oscMDQ");
		m.addFloatArg(_osc_MDQ);
		b.addMessage(m);
		m.clear();
		m.setAddress("/oscMDCX1");
		m.addFloatArg(_osc_MDCX1);
		b.addMessage(m);
		m.clear();
		m.setAddress("/oscMDCX2");
		m.addFloatArg(_osc_MDCX2);
		b.addMessage(m);
		m.clear();
		m.setAddress("/oscMDCY1");
		m.addFloatArg(_osc_MDCY1);
		b.addMessage(m);
		m.clear();
		m.setAddress("/oscMDCY2");
		m.addFloatArg(_osc_MDCY2);
		b.addMessage(m);
	}

	// sending Contour&Blobs data
	if(sendOsc_CF && computeContourFinder){
		m.clear();

		m.setAddress("/oscCFB/BlobNUM"); // contour finder blobs number
		m.addIntArg((int)contourFinder.nBlobs);
		b.addMessage(m);

		for(int i=0;i<contourFinder.nBlobs;i++){
			m.clear();

			char temp[64];
			sprintf(temp,"/oscCFB/Blob%i",i); // contour finder blobs info

			m.setAddress(temp);
			m.addFloatArg(_osc_blobInfo[i].center.x);
			m.addFloatArg(_osc_blobInfo[i].center.y);
			m.addFloatArg(_osc_blobInfo[i].angle);
			m.addFloatArg(_osc_blobInfo[i].size.width);
			m.addFloatArg(_osc_blobInfo[i].size.height);
			b.addMessage(m);
		}
		for(unsigned int g = 0; g < geomLines.size(); g++){
			m.clear();

			char temp[64];
			sprintf(temp,"/oscCFG/Geom%i",g); // contour finder geometry

			m.setAddress(temp);
			m.addFloatArg(_osc_blobGeom[g].x);
			m.addFloatArg(_osc_blobGeom[g].y);
			m.addFloatArg(_osc_blobGeom[g].z);
			m.addFloatArg(_osc_blobGeom[g].w);
			b.addMessage(m);
		}
	}

	// sending Recursive Centroid data
	if(sendOsc_RC && computeRecursiveCentroid){
		for(int i=1;i<centroidNum;i++){
			m.clear();

			char temp[64];
			sprintf(temp,"/oscRCP/Pos%i",i);

			m.setAddress(temp);
			m.addFloatArg(_osc_centPos[i].x);
			m.addFloatArg(_osc_centPos[i].y);
			b.addMessage(m);
		}
	}

	// sending Optical Flow data
	if(sendOsc_OF && computeOpticalFlow){
		ofxOscBundle nested_b;
		for(int o=0;o<OPTICAL_FLOW_GRID_X*OPTICAL_FLOW_GRID_Y;o++){
			m.clear();

			char temp[64];
			sprintf(temp,"/oscOF/PixVel%i",o);

			m.setAddress(temp);
			m.addFloatArg(_osc_opfVel[o].x);
			m.addFloatArg(_osc_opfVel[o].y);
			nested_b.addMessage(m);
		}
		b.addBundle(nested_b);
		nested_b.clear();
	}

	// sending Face Tracking data
	if(sendOsc_FT && computeFaceTracking){
		for(int f = 0; f < numFace; f++){
			m.clear();

			char temp[64];
			sprintf(temp,"/oscFT/Face%i",f);

			m.setAddress(temp);
			m.addFloatArg(_osc_ftInfo[f].x);
			m.addFloatArg(_osc_ftInfo[f].y);
			m.addFloatArg(_osc_ftInfo[f].z);
			m.addFloatArg(_osc_ftInfo[f].w);
			b.addMessage(m);
		}
	}

	if(sendOsc_FHT && computeFingerTracking){
		for(unsigned int h = 0; h < fingerPos.size(); h++){
			m.clear();

			char temp[64];
			sprintf(temp,"/oscFHT/Finger%i",h);

			m.setAddress(temp);
			m.addFloatArg(_s_fingerPos[h].x);
			m.addFloatArg(_s_fingerPos[h].y);
			b.addMessage(m);
		}
	}

	sender.sendBundle(b);

	b.clear();
	m.clear();

}

/////////////////////////////HANDLE GUI/////////////////////////
//--------------------------------------------------------------

void gamuzaApp::handleGui(int parameterId, int task, void* data, int length)
{
	switch(parameterId)
	{
		case kParameter_Panel1:
		case kParameter_Panel2:
			break;

		//////////////////////////////////////////////PANEL 1
		//////////////ADJUST TRACKING/////////////////
		case kParameter_RGBhSV:
			if(task == kofxGui_Set_Int){
				rgbORhsv = *(int*)data;
			}
			break;

		case kParameter_Hue:
			if(task == kofxGui_Set_Float)
				hue = *(float*)data;
			break;

		case kParameter_HueWidth:
			if(task == kofxGui_Set_Float)
				hueWidth = *(float*)data;
			break;

		case kParameter_Sat:
			if(task == kofxGui_Set_Float)
				sat = *(float*)data;
			break;

		case kParameter_SatWidth:
			if(task == kofxGui_Set_Float)
				satWidth = *(float*)data;
			break;

		case kParameter_Val:
			if(task == kofxGui_Set_Float)
				val = *(float*)data;
			break;

		case kParameter_ValWidth:
			if(task == kofxGui_Set_Float)
				valWidth = *(float*)data;
			break;

		case kParameter_Threshold:
			if(task == kofxGui_Set_Float)
				threshold = *(float*)data;
			break;

		case kParameter_MotionThreshold:
			if(task == kofxGui_Set_Float)
				mThreshold = *(float*)data;
			break;

		case kParameter_MNoiseComp:
			if(task == kofxGui_Set_Float)
				mNoiseComp = *(float*)data;
			break;

		case kParameter_Blur1:
			if(task == kofxGui_Set_Float)
				blur1 = *(float*)data;
			break;

		case kParameter_Blur2:
			if(task == kofxGui_Set_Float)
				blur2 = *(float*)data;
			break;

		case kParameter_HSVBlur:
			if(task == kofxGui_Set_Float)
				HsvBlur = *(float*)data;
			break;

		case kParameter_Balance1:
			if(task == kofxGui_Set_Float)
				balance1 = *(float*)data;
			break;

		case kParameter_Balance2:
			if(task == kofxGui_Set_Float)
				balance2 = *(float*)data;
			break;

		case kParameter_DecreaseRate:
			if(task == kofxGui_Set_Float)
				dRate = *(float*)data;
			break;
		//////////////////////////////////////////////END PANEL 1
		//////////////////////////////////////////////

		//////////////////////////////////////////////PANEL 2
		//////////////SAVE SETTINGS///////////////////
		case kParameter_File:
			if(task == kofxGui_Set_String)
			{
				string file = *(string*)data;
				gui->buildFromXml(file);
			}
			break;

		case kParameter_SaveXml:
			if(task == kofxGui_Set_Bool){
				saveXML = *(bool*)data;
				gui->saveToXml(OFXGUI_XML);
			}
			break;
		//////////////////////////////////////////////END PANEL 2
		//////////////////////////////////////////////

		//////////////////////////////////////////////PANEL 3
		//////////////INPUT WARPING///////////////////
		case kParameter_Point1:
			if(task == kofxGui_Set_Point)
				sourcePoints[0] = *(ofxPoint2f*)data;
			break;

		case kParameter_Point2:
			if(task == kofxGui_Set_Point)
				sourcePoints[1] = *(ofxPoint2f*)data;
			break;

		case kParameter_Point3:
			if(task == kofxGui_Set_Point)
				sourcePoints[2] = *(ofxPoint2f*)data;
			break;

		case kParameter_Point4:
			if(task == kofxGui_Set_Point)
				sourcePoints[3] = *(ofxPoint2f*)data;
			break;
		//////////////////////////////////////////////END PANEL 3
		//////////////////////////////////////////////

		//////////////////////////////////////////////PANEL 4
		//////////////OUTPUT CROP/////////////////////
		case kParameter_CPoint1:
			if(task == kofxGui_Set_Point)
				cropPoints[0] = *(ofxPoint2f*)data;
			break;

		case kParameter_CPoint2:
			if(task == kofxGui_Set_Point)
				cropPoints[1] = *(ofxPoint2f*)data;
			break;

		case kParameter_CPoint3:
			if(task == kofxGui_Set_Point)
				cropPoints[2] = *(ofxPoint2f*)data;
			break;

		case kParameter_CPoint4:
			if(task == kofxGui_Set_Point)
				cropPoints[3] = *(ofxPoint2f*)data;
			break;
		//////////////////////////////////////////////END PANEL 4
		//////////////////////////////////////////////

		//////////////////////////////////////////////PANEL 5
		//////////////CONTOURS CONTROL////////////////
		case kParameter_ContoursDetail:
			if(task == kofxGui_Set_Int){
				cfDetail = *(bool*)data;
			}
			break;

		case kParameter_ShowEllipse:
			if(task == kofxGui_Set_Bool){
				bShowEllipse = *(bool*)data;
			}
			break;

		case kParameter_ShowAngle:
			if(task == kofxGui_Set_Bool){
				bShowAngle = *(bool*)data;
			}
			break;

		case kParameter_ShowLines:
			if(task == kofxGui_Set_Bool){
				bShowLines = *(bool*)data;
			}
			break;
		//////////////////////////////////////////////END PANEL 5
		//////////////////////////////////////////////

		//////////////////////////////////////////////PANEL 6
		//////////////COMPUTING ALGORITHMS////////////////
		case kParameter_computeMH:
			if(task == kofxGui_Set_Bool){
				computeMotionHistory = *(bool*)data;
			}
			break;

		case kParameter_computeCF:
			if(task == kofxGui_Set_Bool){
				computeContourFinder = *(bool*)data;
			}
			break;

		case kParameter_computeRC:
			if(task == kofxGui_Set_Bool){
				computeRecursiveCentroid = *(bool*)data;
			}
			break;

		case kParameter_computeOF:
			if(task == kofxGui_Set_Bool){
				computeOpticalFlow = *(bool*)data;
			}
			break;

		case kParameter_computeFT:
			if(task == kofxGui_Set_Bool){
				computeFaceTracking = *(bool*)data;
			}
			break;

		case kParameter_computeFHT:
			if(task == kofxGui_Set_Bool){
				computeFingerTracking = *(bool*)data;
			}
			break;

		case kParameter_lightGui:
			if(task == kofxGui_Set_Bool){
				lightGui = *(bool*)data;
			}
			break;
		//////////////////////////////////////////////END PANEL 6
		//////////////////////////////////////////////

		//////////////////////////////////////////////PANEL 7
		//////////////OSC CONTROL/////////////////////
		case kParameter_SendMD:
			if(task == kofxGui_Set_Bool){
				sendOsc_MD = *(bool*)data;
			}
			break;

		case kParameter_SendCF:
			if(task == kofxGui_Set_Bool){
				sendOsc_CF = *(bool*)data;
			}
			break;

		case kParameter_SendRC:
			if(task == kofxGui_Set_Bool){
				sendOsc_RC = *(bool*)data;
			}
			break;

		case kParameter_SendOF:
			if(task == kofxGui_Set_Bool){
				sendOsc_OF = *(bool*)data;
			}
			break;

		case kParameter_SendFT:
			if(task == kofxGui_Set_Bool){
				sendOsc_FT = *(bool*)data;
			}
			break;

		case kParameter_SendFHT:
			if(task == kofxGui_Set_Bool){
				sendOsc_FHT = *(bool*)data;
			}
			break;
		//////////////////////////////////////////////END PANEL 7
		//////////////////////////////////////////////

		//////////////////////////////////////////////PANEL 8
		/////////BACKGROUND SUBTRACTION TECHNIQUE/////
		case kParameter_BGSubT:
			if(task == kofxGui_Set_Int)
				bgSubTech = *(int*)data;
			break;
		//////////////////////////////////////////////END PANEL 8
		//////////////////////////////////////////////

	}
}
////////////////////////END//HANDLE GUI/////////////////////////
