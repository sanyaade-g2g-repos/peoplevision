#include "testApp.h"




//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);
	ofBackground(255,255,255);
	
	labNet = ofxLABnet(2002);
	loadSettings();	
	
	buildCameras();
	
	ofxLABOSCSender * sender = labNet.addOscSender(oscHost, oscPort, "cvblobsender");
	
	//duplicate other senders
	oscSettings.pushTag("senders", 0);
	for (int i=0; i<oscSettings.getNumTags("sender"); i++){
		oscSettings.pushTag("sender", i);
		labNet.duplicateOscSender(sender, oscSettings.getValue("host","",0), oscSettings.getValue("port",0,0));
		oscSettings.popTag();
	}
	oscSettings.popTag();
	drawDebug = true;	
	newFrame = false;
}

//--------------------------------------------------------------
void testApp::buildCameras(){
	for(int i=0; i<numCameras; i++ ){
		camera[i].close();
		camera[i] = ofVideoGrabber();
		camera[i].initGrabber(cameraWidth[i], cameraHeight[i]);
		
		//check that we got the height we want
		//WIDTH = camera.width;
		//HEIGHT = camera.height;
			
		cvColor[i].clear();
		cvGray[i].clear();
		cvGrayWarped[i].clear();
		cvGrayBg[i].clear();
		cvGrayDiff[i].clear();
		
		//allocate our openCV images
		cvColor[i].allocate(cameraWidth[i], cameraHeight[i]);
		cvGray[i].allocate(cameraWidth[i], cameraHeight[i]);
		cvGrayWarped[i].allocate(cameraWidth[i], cameraHeight[i]);
		cvGrayBg[i].allocate(cameraWidth[i], cameraHeight[i]);
		cvGrayDiff[i].allocate(cameraWidth[i], cameraHeight[i]);
		
		//lets name our quad!
		cvWarpQuad[i].setup("QUAD_");
		
		//lets give the gui quad a default setting
		quadSrc[i][0].set(0, 0);
		quadSrc[i][1].set(cameraWidth[i], 0);
		quadSrc[i][2].set(cameraWidth[i], cameraHeight[i]);
		quadSrc[i][3].set(0, HEIGHT);
		
		quadDst[i][0].set(0, 0);
		quadDst[i][1].set(cameraWidth[i], 0);
		quadDst[i][2].set(cameraWidth[i], cameraHeight[i]);
		quadDst[i][3].set(0, cameraHeight[i]);
		
		//lets set the quad points then try and load our saved quad from file if it exists
		cvWarpQuad[i].setQuadPoints(quadSrc[i]);
		stringstream f;
		f<<"quad-settings_"<<i<<".xml";
		cvWarpQuad[i].readFromFile(f.str());
	}
	cout<<"CAMERAS LOADED"<<endl;
}

//--------------------------------------------------------------
void testApp::update(){
	
	labNet.update();
	for(int i=0; i<numCameras; i++ ) {
		camera[i].grabFrame();
		
		if( camera[i].isFrameNew() ){
			newFrame = true;
			//put the camera pixels into openCV color image
			cvColor[i].setFromPixels( camera[i].getPixels(), cameraWidth[i], cameraHeight[i]);
			cvColor[i].mirror(flipY[i], flipX[i]);
			cvColor[i].rotate(rotate[i]*90, cvColor[i].width/2, cvColor[i].height/2);
			cvGray[i] = cvColor[i];
			
			
			//we get back the warped coordinates - scaled to our camera size
			ofxPoint2f * warpedPts = cvWarpQuad[i].getScaledQuadPoints(cameraWidth[i], cameraHeight[i]);
			
			//lets warp with those cool coordinates!!!!!
			cvGrayWarped[i].warpIntoMe(cvGray[i], warpedPts, quadDst[i]);
			
			//lets calculate the openCV matrix for our coordWarping
			coordWarp[i].calculateMatrix(warpedPts, quadDst[i]);
			
			if (bLearnBakground[i] == true){
				cvGrayBg[i] = cvGrayWarped[i];
				bLearnBakground[i] = false;
			}
			
			//blob detection
			cvGrayDiff[i].absDiff(cvGrayWarped[i], cvGrayBg[i]);
			cvGrayDiff[i].threshold(threshold[i]);
			contourFinder[i].findContours(cvGrayDiff[i], minBlobSize[i], maxBlobSize[i], numBlobs[i], false);
		}
		
		//lets set our input points for coordWarping to be the mouse position
		inputPts[i].x = mouseX;
		inputPts[i].y = mouseY;
		
		//check to see if we are in the quad
		/*if( coordWarp[i].bInQuad(inputPts[i]) ){
			outputPts[i] = coordWarp[i].transform(inputPts[i].x, inputPts[i].y);
		}*/
		
		if(!mouseIsDown && gui->updated && oldCameraWidth[i] != cameraWidth[i]){
			cameraWidth[i] = oldCameraWidth[i];
			if (cameraWidth[i] <= 0) cameraWidth[i] = 1;
			cameraHeight[i] = (3*cameraWidth[i])/4;
			
			buildCameras();
		}
		
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	
	string str = "framerate is "; 						
	str += ofToString(ofGetFrameRate(), 2)+"fps"; 
	ofSetWindowTitle(str);
	
	//build bundle
	ofxOscBundle b;
	float targetWidth, targetHeight, wRatio;
	
	//loop through blobs + add them to bundle
	for(int i=0; i<numCameras; i++ ) {
	
		//if (drawCameras){
		
			targetWidth		= (float)(ofGetWidth())/(float)(4.0f);
			wRatio			= (float)targetWidth/(float)cameraWidth[i];
			targetHeight	= (float)(cameraHeight[i]*wRatio);
		
			ofSetColor(0xffffff);
			cvColor[i].draw(0,(i*targetHeight),targetWidth,targetHeight);	
			cvGrayWarped[i].draw(targetWidth,(i*targetHeight),targetWidth,targetHeight);
			cvGrayBg[i].draw(2*targetWidth,(i*targetHeight),targetWidth,targetHeight);
			cvGrayDiff[i].draw(3*targetWidth,(i*targetHeight),targetWidth,targetHeight);
			cvWarpQuad[i].draw(0, (i*targetHeight), targetWidth,targetHeight);
		
			//ofSetColor(0xffffff);
//			glPushMatrix();
//				glTranslated(3*targetWidth,(i*targetHeight), 0);
//				glScaled(wRatio, wRatio, 1);
//				contourFinder[i].draw(0,0);
//			glPopMatrix();
		//}
		
		//build nested bundle
		//ofxOscBundle camera;
		
		ofSetColor(0xffffff);
		glPushMatrix();
		glTranslated(3*targetWidth,(i*targetHeight), 0);
		glScaled(wRatio, wRatio, 1);
		
		for (int j = 0; j < contourFinder[i].nBlobs; j++){
			ofxCvBlob blob = contourFinder[i].blobs[j];		

			float blobX = (blob.boundingRect.x);
			float blobY = (blob.boundingRect.y);
			float blobW = (blob.boundingRect.width);
			float blobH = (blob.boundingRect.height);
			
			// make message
			ofxOscMessage m;
			char title[20];
			sprintf(title, "/camera%i/blob%i", i,j);
			m.setAddress( title );
			m.addIntArg( blobX );
			m.addIntArg( blobY );
			m.addIntArg( blobW );
			m.addIntArg( blobH );
			
			// add to bundle
			//camera.addMessage( m );
			for (int k=0; k<labNet.OSCSenders.size(); k++){
				labNet.OSCSenders[k]->send( m );
			}
			
			//send contours
			
			if (newFrame && sendContours){
				
				ofxOscMessage cMessage;
				char ctitle[25];
				sprintf(ctitle, "/camera%i/contourblob%i", i,j);
				cMessage.setAddress( ctitle );
				
				stringstream ss;
				
				int skipFrame = 0;
				int amountToSkip = 0;
				
				if (contourFinder[i].blobs[j].pts.size() > 100){
					amountToSkip = contourFinder[i].blobs[j].pts.size()/100.0f;
					//cout<<"SKIPPING "<<amountToSkip<<endl;
				}					
				
				for (int k=0; k<contourFinder[i].blobs[j].pts.size(); k++){
					if (skipFrame == 0) ss<<contourFinder[i].blobs[j].pts[k].x<<":"<<contourFinder[i].blobs[j].pts[k].y<<";";
					skipFrame++;
					if (skipFrame >= amountToSkip) skipFrame = 0;
				}
				cMessage.addStringArg(ss.str());
				
				for (int k=0; k<labNet.OSCSenders.size(); k++){
					labNet.OSCSenders[k]->send( cMessage );
				}
			}
			
			contourFinder[i].blobs[j].draw(0,0);
			
			char blobname[10];
			sprintf(blobname, "blob%i", j);
			ofFill();
			ofSetColor(255, 0, 0);
			ofDrawBitmapString(blobname, blobX+blobW/2, blobY+blobH/2);
		}
		
		glPopMatrix();
		//sender.sendMessage( b );
	
		// add to bundle
		//b.addBundle( camera );
	}
	
	// send that bundle
	//sender.sendBundle( b );
	
	if (drawDebug){
		glDisable(GL_DEPTH_TEST);
		ofSetColor(0xffffff);
		ofFill();
		ofShowCursor();
		ofPushMatrix();
		gui->draw();
		if(gui->updated){
			gui->updated = false;
			saveSettings();
		}
	}
	newFrame = false;
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	switch (key){
		case ' ':
			for(int i=0; i<numCameras; i++ ) {
				bLearnBakground[i] = true;
			}
			break;
		case 's':
			saveSettings();
		break;
		/*case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;*/
		case 'd':
		case 'D':
			drawDebug = !drawDebug;
			break;
		case 'f':
		case 'F':
			ofToggleFullscreen();
			break;
	}
	
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){ 
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

	for(int i=0; i<numCameras; i++ ) {
		float targetWidth = (float)(ofGetWidth())/(float)(4);
		float wRatio	  = (float)targetWidth/cameraWidth[i];
		float targetHeight = (float)cameraHeight[i]*wRatio;
		
		cvWarpQuad[i].updatePoint(x, y-targetHeight*i, 0, 0, targetWidth, targetHeight);
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

	for(int i=0; i<numCameras; i++ ) {
		float targetWidth = (float)(ofGetWidth())/(float)(4);
		float wRatio	  = (float)targetWidth/cameraWidth[i];
		float targetHeight = (float)cameraHeight[i]*wRatio;
		
		cvWarpQuad[i].selectPoint(x, y-targetHeight*i, 0, 0, targetWidth, targetHeight, 20);
	}
	mouseIsDown = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
	
	for(int i=0; i<numCameras; i++ ){
		stringstream f;
		f<<"quad-settings_"<<i<<".xml";
		cvWarpQuad[i].saveToFile(f.str());
	}
	mouseIsDown = false;
}

//--------------------------------------------------------------
void testApp::loadSettings() {
	
	//make gui
	gui = new GuiHandler;
	
	//load OSC JAMS
	oscSettings.loadFile("config.xml");
	oscHost		= oscSettings.getValue("oschost", "localhost", 0);
	oscPort		= oscSettings.getValue("oscport", 10000, 0);
	numCameras	= oscSettings.getValue("numCameras", 1, 0);
	sendContours = oscSettings.getValue("sendContours", sendContours, 0);
	
	//load camera stuffs
	for(int i=0; i<numCameras; i++ ){
	
		stringstream f;
		f<<"settings_"<<i<<".xml";
		settings[i].loadFile(f.str());
		cout<<"LOADING "<<f.str()<<endl;
		
		//gui.toggleRender();
		bool settingsFound = settings[i].pushTag("settings", 0);
		
		if (settingsFound){
			
			cameraWidth[i]	= oldCameraWidth[i]		= (settings[i].getValue("cameraWidth", 640));
			cameraHeight[i] = oldCameraHeight[i]	= (settings[i].getValue("cameraHeight", 480));
			
			threshold[i] = settings[i].getValue("threshold", threshold[i]);
			numBlobs[i] = settings[i].getValue("numblobs", numBlobs[i]);
			minBlobSize[i] = settings[i].getValue("minblobsize", minBlobSize[i]);			
			maxBlobSize[i] = settings[i].getValue("maxblobsize", maxBlobSize[i]);			
			bLearnBakground[i] = true;
			
			flipX[i] = settings[i].getValue("flipx", flipX[i]);
			//gui[i].addToggle("flip X", &flipX[i]);
			
			flipY[i] = settings[i].getValue("flipy", flipY[i]);
			rotate[i] = settings[i].getValue("rotate", rotate[i]);
			settings[i].popTag();
		} else {
			cameraWidth[i]	= oldCameraWidth[i] =  (640);
			cameraHeight[i] = oldCameraHeight[i] = (480);
			threshold[i]	= 80;
			numBlobs[i]		= 2;
			minBlobSize[i]	= 20;
			maxBlobSize[i]	= cameraWidth[i]*cameraHeight[i];
			bLearnBakground[i] = true;
			flipX[i] = false;			
			flipY[i] = false;
			rotate[i] = 0;
		}
		
		float targetWidth = (float)(ofGetWidth())/(float)(4);
		float wRatio	  = (float)targetWidth/cameraWidth[i];
		float targetHeight = (float)cameraHeight[i]*wRatio;
		
		char * name;
		sprintf(name,"Camera %i", i);
		Panel2 = gui->addPanel(name,5 + 200*i, targetHeight*numCameras + 5,600,200);
		
		Panel2->addIntSlider("CameraWidth", 580, 10, oldCameraWidth[i], 640, 0, &oldCameraWidth[i]);
		Panel2->addIntSlider("threshold", 580, 10, threshold[i], 255, 0, &threshold[i]);
		Panel2->addIntSlider("number of blobs", 580, 10, numBlobs[i], 20, 1, &numBlobs[i]);
		Panel2->addIntSlider("minimum blob size", 580, 10, minBlobSize[i], 600, 2, &minBlobSize[i]);
		Panel2->addIntSlider("maximum blob size", 580, 10, maxBlobSize[i], cameraWidth[i]*cameraHeight[i], 3, &maxBlobSize[i]);
		Panel2->addButton("learn background", 580, 10, "TRIGGER", true, 4, &bLearnBakground[i]);
		Panel2->addButton("flip X", 580, 10, "SWITCH", true, 5, &flipX[i]);
		Panel2->addButton("flip Y", 580, 10, "SWITCH", true, 6, &flipY[i]);		 		
		Panel2->addIntSlider("rotate", 580, 15, rotate[i], 4, 7, &rotate[i]);
		
		Panel2->addButton("send contours", 580, 10, "SWITCH", true, 4, &sendContours);
	}
}

//--------------------------------------------------------------
void testApp::saveSettings() {
	oscSettings.loadFile("config.xml");
	oscSettings.setValue("numCameras", numCameras);
	oscSettings.saveFile("config.xml");
	
	for(int i=0; i<numCameras; i++ ){
		settings[i].setValue("settings:threshold", threshold[i]);
		settings[i].setValue("settings:numblobs", numBlobs[i]);
		settings[i].setValue("settings:minblobsize", minBlobSize[i]);
		settings[i].setValue("settings:maxblobsize", maxBlobSize[i]);
		settings[i].setValue("settings:flipx", (int)flipX[i]);
		settings[i].setValue("settings:flipy", (int)flipY[i]);
		settings[i].setValue("settings:rotate", (int)rotate[i]);
		settings[i].setValue("settings:cameraWidth", (int)cameraWidth[i]);
		settings[i].setValue("settings:cameraHeight", (int)cameraHeight[i]);
		settings[i].setValue("sendContours", (int)sendContours);
						
		stringstream f;
		f<<"settings_"<<i<<".xml";
		settings[i].saveFile(f.str());
	}
}
