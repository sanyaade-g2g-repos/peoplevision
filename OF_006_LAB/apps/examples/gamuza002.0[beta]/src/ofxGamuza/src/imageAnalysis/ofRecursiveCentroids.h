#ifndef _OF_RECURSIVE_CENTROID
#define _OF_RECURSIVE_CENTROID

#include "ofMain.h"
#include "ofxThread.h"
#include <vector>


typedef struct{
	int 	posx;
	int 	posy;
	bool 	bOn;
	bool 	bSelected;
	int 	level;
} centroid;

typedef struct{
	int 	centa;
	int 	centb;
	bool 	bSelected;
} centLine;

#define MAX_NUM_CENTROID_LINES				20

//---------------------------------------------------------------
class ofRecursiveCentroids: public ofxThread{
	
	public:
		
		ofRecursiveCentroids(int _nLevels, int _width, int _height);
		void computeCentroid (unsigned char * data);
		void computeCentroid(int x, int y, int w, int h, unsigned char * data, bool splitOnUpDown, int level, int id, int addToId);
		
		void draw();
		
		void threadedFunction();
		void computeCentroidThreaded(unsigned char * data);
		
		int wid, hei;	
		
		unsigned char		*pixels;
		centroid *			cent;	
		int 				nCentroids;
		int 				nLevels;
		vector<ofPoint>		positions;

		
		bool 				bLinesVisible;
		bool 				bPointsVisible;
		
		
	
		
};
//---------------------------------------------------------------

#endif