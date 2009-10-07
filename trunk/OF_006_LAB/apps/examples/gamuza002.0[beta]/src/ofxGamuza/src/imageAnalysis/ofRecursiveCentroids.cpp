#include "ofRecursiveCentroids.h"


//---------------------------------------------------------------
ofRecursiveCentroids::ofRecursiveCentroids(int _nLevels, int _width, int _height){

	nLevels				= _nLevels;
	nCentroids 			= pow(2, nLevels-1);
	cent 				= new centroid[ nCentroids ];
	wid					= _width;
	hei					= _height;

	for (int i = 0; i < nCentroids; i++){
		cent[i].bOn = false;
		positions.push_back(ofPoint(0.0f,0.0f,0.0f));
	}


}


//---------------------------------------------------------------
void ofRecursiveCentroids::draw(){

	ofFill();

	for (int i = 1; i < nCentroids; i++){
		if (cent[i].bOn){
			glColor3f(1,1,1);
		}else{
			glColor3f(1,1,1);
		}
		ofRect(cent[i].posx- 5,cent[i].posy-5,10,10);
		positions[i].set(cent[i].posx, cent[i].posy, 0.0f);
	}
}


//---------------------------------------------------------------
void ofRecursiveCentroids::computeCentroid (unsigned char * data){
	computeCentroid(0,0, wid, hei, data, false, 0, 1, 0);
}


//---------------------------------------------------------------
void ofRecursiveCentroids::computeCentroid(int x, int y, int w, int h, unsigned char * data, bool splitOnUpDown, int level, int id, int addToId){

	if (level == nLevels-1 ) return;

	int xPos = 0;
	int yPos = 0;
	int nPix = 0;
	for (int i = x; i < x+w; i++){
		for (int j = y; j < y+h; j++){
			int pos = j * wid + i;
			if (data[pos] == 255){
				xPos += i;
				yPos += j;
				nPix++;
			}
		}
	}

	if (nPix > 0){

		xPos = (int)((float)xPos / (float)nPix);
		yPos = (int)((float)yPos / (float)nPix);

		cent[id + addToId].posx 	= xPos;
		cent[id + addToId].posy 	= yPos;
		cent[id + addToId].level		= level;

		splitOnUpDown = !splitOnUpDown;
		level = level+1;
		if (splitOnUpDown){
			id *= 2;
			computeCentroid(x,y,w,(yPos - y), data,  splitOnUpDown, level, id, addToId);
			computeCentroid(x,yPos,w,h-(yPos-y),  data,  splitOnUpDown, level, id+1, addToId);
		} else {
			id *= 2;
			computeCentroid(x,y,(xPos-x),h,  data,  splitOnUpDown, level, id, addToId);
			computeCentroid(xPos,y,w - (xPos-x),h,   data,  splitOnUpDown, level, id+1, addToId);
		}
	}
}

//---------------------------------------------------------------
void ofRecursiveCentroids::threadedFunction(){
	if(lock()) {
		computeCentroid(0,0, wid, hei, pixels, false, 0, 1, 0);
		unlock();
	}else{
		printf("ofRecursiveCentroid - cannot compute RecursiveCentroid cos I'm locked");
	}
	stopThread();
}

//---------------------------------------------------------------
void ofRecursiveCentroids::computeCentroidThreaded(unsigned char * data){
	this->pixels = data;
	startThread(false, false);   // blocking, verbose
}
