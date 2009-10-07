#include "testApp.h"

//
// cvOpFlowBMExample by Takashi Maekawa and Satoru higa.
// An usage of example cvOpFlowBM class.
//

//--------------------------------------------------------------
void testApp::setup(){	 
    ofBackground(255,255,255);	
	
    cw = 320; ch = 240;
    
    vidGrabber.initGrabber(cw,ch);    
    cameraImg.allocate(cw, ch);
    grayImg.allocate(cw, ch);
    pastImg.allocate(cw, ch);
	
	opticalFlow.allocate(cw,ch);
}

//--------------------------------------------------------------
void testApp::update(){
    
    vidGrabber.grabFrame();
    
    if(vidGrabber.isFrameNew()){
        pastImg = grayImg;
        
        cameraImg.setFromPixels(vidGrabber.getPixels(), cw, ch);
        grayImg.setFromColorImage(cameraImg);
        
        grayImg.threshold(100);
        grayImg.blurHeavily();
		
        // printf("%p %p\n", pastImg.getCvImage(), grayImg.getCvImage());
		opticalFlow.calc(pastImg, grayImg,10);		
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    cameraImg.draw(0,0);
    grayImg.draw(320,0);
    pastImg.draw(0,240);
	opticalFlow.draw();       
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){ 
	if(key == 's'){
        vidGrabber.videoSettings();
    }
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
void testApp::mouseReleased(){
}

/* original test app.c from takashi

 //
 // cvOpFlowBMExample by Takashi Maekawa and Satoru higa.
 // An usage of example cvOpFlowBM class.
 //
 
 #include "testApp.h"
 
 //--------------------------------------------------------------
 void testApp::setup(){	 
 ofBackground(255,255,255);	
 
 cw = 320; ch = 240;
 
 vidGrabber.initGrabber(cw,ch);    
 cameraImg.allocate(cw, ch);
 grayImg.allocate(cw, ch);
 pastImg.allocate(cw, ch);
 
 opticalFlow.allocate(cw,ch);
 }
 
 //--------------------------------------------------------------
 void testApp::update(){
 
 vidGrabber.grabFrame();
 
 if(vidGrabber.isFrameNew()){
 pastImg = grayImg;
 
 cameraImg.setFromPixels(vidGrabber.getPixels(), cw, ch);
 grayImg.setFromColorImage(cameraImg);
 
 grayImg.threshold(100);
 grayImg.blurHeavily();
 
 // printf("%p %p\n", pastImg.getCvImage(), grayImg.getCvImage());
 opticalFlow.calc(pastImg, grayImg,10);		
 }
 
 }
 
 //--------------------------------------------------------------
 void testApp::draw(){
 cameraImg.draw(0,0);
 grayImg.draw(320,0);
 pastImg.draw(0,240);
 opticalFlow.draw();       
 }
 
 //--------------------------------------------------------------
 void testApp::keyPressed(int key){ 
 if(key == 's'){
 vidGrabber.videoSettings();
 }
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
 void testApp::mouseReleased(){
 }
 
 */
