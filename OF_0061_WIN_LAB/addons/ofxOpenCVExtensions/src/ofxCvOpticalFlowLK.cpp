//
// ofxCvOpticalFlowLK.h - a OpenCV cvOpticalFlowLK warpper for openFrameworks
//
// Copyright (C) 2008 Takashi Maekawa <takachin@generative.info> 
//     All rights reserved.
//     This is free software with ABSOLUTELY NO WARRANTY.
//
// You can redistribute it and/or modify it under the terms of 
// the GNU Lesser General Public License.
//

#include "ofxCvOpticalFlowLK.h"

ofxCvOpticalFlowLK::ofxCvOpticalFlowLK(void)
{
	captureWidth = DEFAULT_CAPTURE_WIDTH;
	captureHeight = DEFAULT_CAPTURE_HEIGHT;

	captureColsStep = DEFAULT_CAPTURE_COLS_STEP;
	captureRowsStep = DEFAULT_CAPTURE_ROWS_STEP;
}

ofxCvOpticalFlowLK::~ofxCvOpticalFlowLK(void)
{
	cvReleaseImage(&vel_x);
	cvReleaseImage(&vel_y);
}
	
void ofxCvOpticalFlowLK::allocate(int _w, int _h){
	captureWidth = _w;
	captureHeight = _h;

	vel_x = cvCreateImage( cvSize( captureWidth ,captureHeight ), IPL_DEPTH_32F, 1  );
	vel_y = cvCreateImage( cvSize( captureWidth ,captureHeight ), IPL_DEPTH_32F, 1  );
	
    cvSetZero(vel_x);
    cvSetZero(vel_y);
}

void ofxCvOpticalFlowLK::setCalcStep(int _cols, int _rows){
	captureColsStep = _cols;
	captureRowsStep = _rows;
}

void ofxCvOpticalFlowLK::calc( ofxCvGrayscaleImage & pastImage,
					   ofxCvGrayscaleImage & currentImage,					   
					   int size
					   )
{
	//cout<<"CALC-ING FLOW"<<endl;
	cvCalcOpticalFlowLK( pastImage.getCvImage(), currentImage.getCvImage(),
		cvSize( size, size), vel_x, vel_y );
}

void ofxCvOpticalFlowLK::draw(void){

	ofEnableAlphaBlending();
	ofSetColor(0xffffff);
	ofNoFill();

	int x, y, dx, dy;
	for ( y = 0; y < captureHeight; y+=captureRowsStep ){
		for ( x = 0; x < captureWidth; x+=captureColsStep ){

//			dx = (int)cvGetReal2D( vel_x, y, x );
//			dy = (int)cvGetReal2D( vel_y, y, x );

			//drawing reflects clamped values
			ofPoint vec = flowAtPoint(x, y);
			ofLine(x,y,x+vec.x,y+vec.y);

		}
	}
	ofDisableAlphaBlending();
}

void ofxCvOpticalFlowLK::draw(int width, int height){
	
	ofEnableAlphaBlending();
	ofSetColor(0xffffff);
	ofNoFill();
	
	float scalex = (float) width/captureWidth;
	float scaley = (float) height/captureHeight;
	
	int x, y, dx, dy;
	for ( y = 0; y < captureHeight; y+=captureRowsStep ){
		for ( x = 0; x < captureWidth; x+=captureColsStep ){
			
//			dx = (int)cvGetReal2D( vel_x, y, x );
//			dy = (int)cvGetReal2D( vel_y, y, x );
//			
			ofPoint vec = flowAtPoint(x, y);
			ofLine(x,y,x+vec.x,y+vec.y);
			
		}
	}
	ofDisableAlphaBlending();
}

ofPoint ofxCvOpticalFlowLK::flowAtPoint(int x, int y){
	if(x >= captureWidth || x < 0 || y >= captureHeight || y < 0){
		return 0.0f;
	}
	float fdx = cvGetReal2D( vel_x, y, x );
	float fdy = cvGetReal2D( vel_y, y, x );
	float mag2 = fdx*fdx + fdy*fdy;
	if(  mag2 > maxVector*maxVector){
		//return a normalized vector of the magnitude size
		return ofPoint(fdx,fdy)/mag2 * maxVector;
	}
	if( mag2 < minVector*minVector){
		//threhsold to 0
		return ofPoint(0,0);
	}
	return ofPoint(fdx, fdy);
}

ofPoint ofxCvOpticalFlowLK::flowInRegion(ofRectangle roi)
{
	return flowInRegion(roi.x, roi.y, roi.height, roi.width);
}

ofPoint ofxCvOpticalFlowLK::flowInRegion(int x, int y, int w, int h){
	w = fmin(x+w, x - captureWidth);
	h = fmin(y+h, y - captureHeight);
	x = fmax(0,x);
	y = fmax(0,y);
	ofPoint total(0,0);
	for(int i = 0; i < x; i++){
		for (int j = 0; j < y; j++) {
			total += flowAtPoint(x, y);
		}
	}
	return total / (w*h);
}

