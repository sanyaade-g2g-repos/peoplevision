/*
 *  ofxDspAmplifier.h
 *  ofxDsp
 *
 *  Created by Stefan Kirch on 22.07.08.
 *  Copyright 2008 alphakanal. All rights reserved.
 *
 */

//	----------------------------------------------------------------------------------------------------

#ifndef OFX_DSP_AMPLIFIER
#define OFX_DSP_AMPLIFIER

//	----------------------------------------------------------------------------------------------------

#include "ofxDspTypes.h"
#include "ofxDspObject.h"

//	----------------------------------------------------------------------------------------------------

class ofxDspAmplifier : public ofxDspObject
{
public:
	
	ofxDspAmplifier(int objectId);
	
	void	render(float* buffer, int bufferSize);

	bool	mActive;
	int		mType;
	float	mSpread, mPanorama, mLevel;
};

//	----------------------------------------------------------------------------------------------------

#endif

//	----------------------------------------------------------------------------------------------------
