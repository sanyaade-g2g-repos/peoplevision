/*
 *  ofxDspFilter.h
 *  ofxDsp
 *
 *  Created by Stefan Kirch on 21.07.08.
 *  Copyright 2008 alphakanal. All rights reserved.
 *
 */

//	----------------------------------------------------------------------------------------------------

#ifndef OFX_DSP_FILTER
#define OFX_DSP_FILTER

//	----------------------------------------------------------------------------------------------------

#include "ofxDspTypes.h"
#include "ofxDspObject.h"

//	----------------------------------------------------------------------------------------------------

class ofxDspFilter : public ofxDspObject
{
public:
	
	ofxDspFilter(int objectId);	

	void	update();
	void	render(float* buffer, int bufferSize);
	
	bool	mActive;
	int		mType;
	float	mCutOff, mResonance;
	
	float	svf[10];
};

//	----------------------------------------------------------------------------------------------------

#endif

//	----------------------------------------------------------------------------------------------------
