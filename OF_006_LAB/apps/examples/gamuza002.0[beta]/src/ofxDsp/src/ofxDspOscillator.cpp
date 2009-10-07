/*
 *  ofxDspOscillator.cpp
 *  ofxDsp
 *
 *  Created by Stefan Kirch on 21.07.08.
 *  Copyright 2008 alphakanal. All rights reserved.
 *
 */

//	----------------------------------------------------------------------------------------------------

#include "ofxDspOscillator.h"

//	----------------------------------------------------------------------------------------------------

ofxDspOscillator::ofxDspOscillator(int objectId)
{
	mObjectId	= objectId;
	mObjectType	= kofxDsp_Oscillator;
	mParamIndex	= mGlobals->requestParameter(mObjectId, mObjectType);
	mParameter	= &mGlobals->mParameter[mParamIndex];
	
	mPhase		= 0.0f;
}

//	----------------------------------------------------------------------------------------------------

void ofxDspOscillator::render(float* buffer, int bufferSize)
{
	bool	active		= mParameter[kofxDsp_Oscillator_Active].value == kofxDsp_Active;
	int		type		= (int)mParameter[kofxDsp_Oscillator_Type].value;
	float	frequency	= mParameter[kofxDsp_Oscillator_Frequency].value;
	float	tuning		= mParameter[kofxDsp_Oscillator_Tuning].value * 2.0f - 1.0f;

	if(!active)
		return;

	float	phase		= mPhase;
	float	phaseInc	= (frequency + (frequency / 10.0f) * tuning) / mGlobals->mSampleRate;
	
	float out;
	
	switch (type)
	{
		case kofxDsp_Oscillator_Sine:
		{
			while(bufferSize--)
			{
				phase += phaseInc;
				
				if (phase > 1.0)
					phase -= 1.0;
				
				out = sin(phase * TWO_PI);
				
				*buffer++	+= out;
				*buffer++	+= out;		
			}
			
			break;
		}
			
		case kofxDsp_Oscillator_Saw:
		{
			while(bufferSize--)
			{
				phase += phaseInc;
				
				if (phase > 1.0)
					phase -= 2.0;
				
				out = phase;
				
				*buffer++	+= out;
				*buffer++	+= out;		
			}
			
			break;
		}
			
		case kofxDsp_Oscillator_Rect:
		{
			while(bufferSize--)
			{
				phase += phaseInc;
				
				if (phase > 1.0)
					phase -= 2.0;
				
				out = phase > 0 ? 1.0 : -1.0;
				
				*buffer++	+= out;
				*buffer++	+= out;		
			}
			
			break;
		}
			
		case kofxDsp_Oscillator_Noise:
		{
			while(bufferSize--)
			{
				out = ofRandomf();
				
				*buffer++	+= out;
				*buffer++	+= out;		
			}
			
			break;
		}
	}
	
	mPhase = phase;
}

//	----------------------------------------------------------------------------------------------------
