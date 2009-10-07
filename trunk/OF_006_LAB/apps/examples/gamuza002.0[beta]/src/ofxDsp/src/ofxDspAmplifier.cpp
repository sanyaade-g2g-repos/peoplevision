/*
 *  ofxDspAmplifier.cpp
 *  ofxDsp
 *
 *  Created by Stefan Kirch on 22.07.08.
 *  Copyright 2008 alphakanal. All rights reserved.
 *
 */

//	----------------------------------------------------------------------------------------------------

#include "ofxDspAmplifier.h"

//	----------------------------------------------------------------------------------------------------

ofxDspAmplifier::ofxDspAmplifier(int objectId)
{
	mObjectId	= objectId;
	mObjectType	= kofxDsp_Amplifier;
	mParamIndex	= mGlobals->requestParameter(mObjectId, mObjectType);
	mParameter	= &mGlobals->mParameter[mParamIndex];

	mSpread		= 0.0f;
	mPanorama	= 0.0f;
	mLevel		= 0.0f;
}

//	----------------------------------------------------------------------------------------------------

void ofxDspAmplifier::render(float* buffer, int bufferSize)
{
	bool	active			= mParameter[kofxDsp_Amplifier_Active].value == kofxDsp_Active;
	float	spread			= 0.5f + mParameter[kofxDsp_Amplifier_Spread].value * 0.5f;
	float	panorama		= mParameter[kofxDsp_Amplifier_Panorama].value;
	float	level			= mParameter[kofxDsp_Amplifier_Level].value;

	if(!active)
		return;

	float	ramp			= 1.0f / bufferSize;
	
	float	l_spread		= mSpread;
	float	spreadDelta		= (spread - l_spread) * ramp;
	
	float	l_panorama		= mPanorama;
	float	panoramaDelta	= (panorama - l_panorama) * ramp;
	
	float	l_level			= mLevel;
	float	levelDelta		= (level - l_level) * ramp;
	
	float	left, right, invSpread, invPanorama;
	
	while(bufferSize--)
	{
		l_spread   += spreadDelta;
		l_panorama += panoramaDelta;
		l_level	   += levelDelta;
		
		invSpread	= 1.0f - l_spread;
		invPanorama	= 1.0f - l_panorama;
		
		left		= buffer[0];
		right		= buffer[1];
		
		left	    = left * invSpread + right * l_spread;
		right	    = right * invSpread + left * l_spread;

		left		= left * invPanorama;
		right		= right * l_panorama;
		
		left		= left * l_level;
		right		= right * l_level;

		*buffer++	= left;
		*buffer++	= right;
	}
	
	mSpread		= l_spread;
	mPanorama	= l_panorama;
	mLevel		= l_level;
}

//	----------------------------------------------------------------------------------------------------

