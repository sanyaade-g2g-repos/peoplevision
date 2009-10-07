/*
 *  ofxDspOscillator.h
 *  ofxDsp
 *
 *  Created by Stefan Kirch on 21.07.08.
 *  Copyright 2008 alphakanal. All rights reserved.
 *
 */

//	----------------------------------------------------------------------------------------------------

#ifndef OFX_DSP_OSCILLATOR
#define OFX_DSP_OSCILLATOR

//	----------------------------------------------------------------------------------------------------

#include "ofxDspTypes.h"
#include "ofxDspObject.h"

//	----------------------------------------------------------------------------------------------------

class ofxDspOscillator : public ofxDspObject
{
public:
	
	ofxDspOscillator(int objectId);
	
	void	render(float* buffer, int bufferSize);
	
	bool	mActive;
	int		mType;
	float	mPhase, mPhaseInc, mFrequency;	
};

//	----------------------------------------------------------------------------------------------------

#endif

//	----------------------------------------------------------------------------------------------------
