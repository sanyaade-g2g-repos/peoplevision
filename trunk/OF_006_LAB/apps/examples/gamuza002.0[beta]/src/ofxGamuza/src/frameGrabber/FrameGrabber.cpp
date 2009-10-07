#include "FrameGrabber.h"


FrameGrabber::FrameGrabber(string _fileName, int _numFrames, bool _close, int _x, int _y, int _w, int _h){
	fileName = _fileName; 
	maxFrames = _numFrames;
	close = _close;
	frameCounter = 0;
	minutes = seconds = 0;
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}

void FrameGrabber::grabFrame(){
	frameCounter++;
	if(frameCounter <= maxFrames){
		string file = fileName + ofToString(frameCounter, 0) + ".png";
		img.grabScreen(x,y,w,h);
		img.saveImage(file);
		float elapsedTime = ofGetElapsedTimef();
		seconds = (int)elapsedTime%60;
		minutes = (int)(elapsedTime/60);
		
		string ellapsed = " elapsed: " + ofToString(minutes, 0) + "min "+ ofToString(seconds, 0) + "sec\n";
		std::cout << ofToString(frameCounter, 0) + " of " + ofToString(maxFrames, 0) + " frames " + 
		"at " + ofToString(ofGetFrameRate(),2) + "fps" + ellapsed;
	}else if(close){
		OF_EXIT_APP(0);
	}
}

void FrameGrabber::threadedFunction(){
	if(lock()) {
		grabFrame();
		unlock();
	}else{
		printf("ofRecursiveCentroid - cannot compute RecursiveCentroid cos I'm locked");
	}
	stopThread();
}

void FrameGrabber::grabFrameThreaded(){
	startThread(false, false);   // blocking, verbose
}