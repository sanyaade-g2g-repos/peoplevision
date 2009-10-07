#ifndef _GUI_QUAD_
#define _GUI_QUAD_

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxVectorMath.h"
#include "ofxXmlSettings.h"

#include "baseGui.h"
//#include "miscUtils.h"

class guiQuad : public baseGui{
	
	public:
		guiQuad();

		void setup(string _quadName);
		void readFromFile(string filePath);
		void loadSettings();
		
		void releaseAllPoints();
		//these should be in the range x(0-maxX) y(0-maxH) 
		//with 	width  = maxW
		//		height = maxH 
		void setQuadPoints( ofxPoint2f * inPts );		
		
		bool selectPoint(float x, float y, float offsetX, float offsetY, float width, float height, float hitArea);
		bool updatePoint(float x, float y, float offsetX, float offsetY, float width, float height);
		
		//returns pts to width by height range 
		ofxPoint2f * getScaledQuadPoints(float width, float height);
		//returns pts in 0-1 scale
		ofxPoint2f * getQuadPoints();		
		void saveToFile(string filePath, string newQuadName);
		void saveToFile(string filePath);
		void draw(float x, float y, float width, float height, int red, int green, int blue, int thickness);
		void draw(float x, float y, float width, float height);

		
	protected:
		ofxXmlSettings	xml;
		ofxPoint2f srcZeroToOne[4];
		ofxPoint2f srcScaled[4];
		string quadName;
		int selected;
};

#endif	



