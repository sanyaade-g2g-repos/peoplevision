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

#pragma once

#include "ofMain.h"
#include "ofxCvConstants.h"
#include "ofxCvGrayscaleImage.h"

class ofxCvOpticalFlowLK
{
public:
	ofxCvOpticalFlowLK(void);
	~ofxCvOpticalFlowLK(void);
		
	void allocate(int _w, int _h);

	void calc( ofxCvGrayscaleImage & pastImage,
			   ofxCvGrayscaleImage & currentImage,
			  int size);

	void setCalcStep(int _cols, int _rows);
		
	void reset();
	void draw();
	void draw(int width, int height);
	
	vector <int> getFlowFromPoint(int x, int y);
	vector<vector <int> > getFlowVector();

private:

	static const int DEFAULT_CAPTURE_WIDTH = 320;
	static const int DEFAULT_CAPTURE_HEIGHT = 240;

	int captureWidth;
	int captureHeight;

	static const int DEFAULT_CAPTURE_COLS_STEP = 4;
	static const int DEFAULT_CAPTURE_ROWS_STEP = 4;

	int captureColsStep;
	int captureRowsStep;

	IplImage* vel_x;
	IplImage* vel_y;

};
