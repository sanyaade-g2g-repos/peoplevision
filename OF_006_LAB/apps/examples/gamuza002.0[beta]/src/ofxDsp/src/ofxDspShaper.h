/*
 *  ofxDspShaper.h
 *  ofxDsp
 *
 *  Created by Stefan Kirch on 22.07.08.
 *  Copyright 2008 alphakanal. All rights reserved.
 *
 */

//	----------------------------------------------------------------------------------------------------

#ifndef OFX_DSP_SHAPER
#define OFX_DSP_SHAPER

//	----------------------------------------------------------------------------------------------------

#include "ofxDspTypes.h"
#include "ofxDspObject.h"

//	----------------------------------------------------------------------------------------------------

class ofxDspShaper : public ofxDspObject
{
public:
	
	ofxDspShaper(int objectId);
	
	void	render(float* buffer, int bufferSize);

	bool	mActive;
	int		mType;
	float	mDrive, mControl, mLevel;
};

//	----------------------------------------------------------------------------------------------------

#endif

//	----------------------------------------------------------------------------------------------------
