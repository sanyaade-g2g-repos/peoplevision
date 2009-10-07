#include "testApp.h"

struct find_track_point {
	find_track_point() : gray(NULL), use_subpix(false) {}
	
	virtual ~find_track_point(){
		delete gray; gray = 0;
		cvReleaseImage(&eig);
		cvReleaseImage(&temp);
	}
	
	void allocate(int w, int h, int max_count, double quality, double min_distance){
		this->count = max_count;
		this->quality = quality;
		this->min_distance = min_distance;
		
		points.resize(count);
		eig = cvCreateImage( cvSize(w, h), 32, 1 );
		temp = cvCreateImage( cvSize(w, h), 32, 1 );
		
		gray = new ofxCvGrayscaleImage();
		gray->allocate(w, h);
	}
	
	void update(ofxCvColorImage& image){
		gray->setFromColorImage(image);
		
		cvGoodFeaturesToTrack( gray->getCvImage(), eig, temp, &points[0], &count,
							  quality, min_distance, NULL, 3, 0, 0.04 );
		
		if(use_subpix){
			cvFindCornerSubPix( gray->getCvImage(), &points[0], count, cvSize(10, 10), cvSize(-1,-1),
							   cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03));
		}
		
	}
	
	void draw(int x = 0, int y = 0){
		glPushMatrix();{
			glTranslatef(x, y, 0);
			for(int i = 0; i < count; i++){
				CvPoint2D32f &p = points[i];
				
				ofNoFill();
				ofSetColor(0xff0000);
				
				ofBeginShape();
				ofVertex(p.x, p.y);
				ofVertex(p.x + 3, p.y + 3);
				ofVertex(p.x, p.y);
				ofVertex(p.x, p.y + 4);
				
				ofEndShape();
			}
		}; glPopMatrix();
		
		ofSetColor(0xffffff);
		gray->draw(0, 240);
	}
	
	//std::vector<CvPoint2D32f> points;
	vector<CvPoint2D32f> points;
	double quality, min_distance;
	void findCornerSubPix(bool yn){ use_subpix = yn; }
	
private:
	
	ofxCvGrayscaleImage *gray;    
	IplImage *eig, *temp;
	int count;
	bool use_subpix;
};

find_track_point f;

//--------------------------------------------------------------
void testApp::setup(){	 
	ofBackground(255,255,255);	
	
	cout<<"I AM NEW"<<endl;
	cw = 320; ch = 240;
	
	vidGrabber.initGrabber(cw,ch);
	
	cameraImg.allocate(cw, ch);
	
	f.allocate(cw, ch, 150, 0.01, 3);
}

//--------------------------------------------------------------
void testApp::update(){
	
	vidGrabber.grabFrame();
	
	if(vidGrabber.isFrameNew()){        
		cameraImg.setFromPixels(vidGrabber.getPixels(), cw, ch);
		f.update(cameraImg);
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0xffffff);
	cameraImg.draw(0,0);
	f.draw();
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
 // cvFindTrackPointExample by Takashi Maekawa and Satoru Higa.
 // Based on code Good Features Track by masayashi.
 //    http://wikiwiki.jp/masayashi/?OpenCV%2FGood%20Features%20to%20Track
 // An usage of example cvFindTrackPoint class.
 //
 
 #include "testApp.h"
 
 struct find_track_point {
 
 find_track_point() : gray(NULL), use_subpix(false) {}
 
 virtual ~find_track_point(){
 delete gray; gray = 0;
 cvReleaseImage(&eig);
 cvReleaseImage(&temp);
 }
 
 void allocate(int w, int h, int max_count, double quality, double min_distance){
 this->count = max_count;
 this->quality = quality;
 this->min_distance = min_distance;
 
 points.resize(count);
 eig = cvCreateImage( cvSize(w, h), 32, 1 );
 temp = cvCreateImage( cvSize(w, h), 32, 1 );
 
 gray = new ofCvGrayscaleImage();
 gray->allocate(w, h);
 }
 
 void update(ofCvColorImage& image){
 gray->setFromColorImage(image);
 
 cvGoodFeaturesToTrack( gray->getCvImage(), eig, temp, &points[0], &count,
 quality, min_distance, NULL, 3, 0, 0.04 );
 
 if(use_subpix){
 cvFindCornerSubPix( gray->getCvImage(), &points[0], count, cvSize(10, 10), cvSize(-1,-1),
 cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03));
 }
 
 }
 
 void draw(int x = 0, int y = 0){
 glPushMatrix();{
 glTranslatef(x, y, 0);
 for(int i = 0; i < count; i++){
 CvPoint2D32f &p = points[i];
 
 ofNoFill();
 ofSetColor(0xff0000);
 
 ofBeginShape();
 ofVertex(p.x, p.y);
 ofVertex(p.x + 3, p.y + 3);
 ofVertex(p.x, p.y);
 ofVertex(p.x, p.y + 4);
 
 ofEndShape();
 }
 }; glPopMatrix();
 
 gray->draw(0, 240);
 }
 
 //std::vector<CvPoint2D32f> points;
 vector<CvPoint2D32f> points;
 double quality, min_distance;
 void findCornerSubPix(bool yn){ use_subpix = yn; }
 
 private:
 
 ofCvGrayscaleImage *gray;    
 IplImage *eig, *temp;
 int count;
 bool use_subpix;
 };
 
 find_track_point f;
 
 //--------------------------------------------------------------
 void testApp::setup(){	 
 ofBackground(255,255,255);	
 
 cw = 320; ch = 240;
 
 vidGrabber.initGrabber(cw,ch);
 
 cameraImg.allocate(cw, ch);
 
 f.allocate(cw, ch, 100, 0.01, 3);
 }
 
 //--------------------------------------------------------------
 void testApp::update(){
 
 vidGrabber.grabFrame();
 
 if(vidGrabber.isFrameNew()){        
 cameraImg.setFromPixels(vidGrabber.getPixels(), cw, ch);
 f.update(cameraImg);
 }
 
 }
 
 //--------------------------------------------------------------
 void testApp::draw(){
 ofSetColor(0xffffff);
 cameraImg.draw(0,0);
 f.draw();
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
