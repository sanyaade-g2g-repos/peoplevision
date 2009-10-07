/*
 *  ofxDspFilter.cpp
 *  ofxDsp
 *
 *  Created by Stefan Kirch on 21.07.08.
 *  Copyright 2008 alphakanal. All rights reserved.
 *
 */

//	----------------------------------------------------------------------------------------------------

#include "ofxDspFilter.h"

//	----------------------------------------------------------------------------------------------------

ofxDspFilter::ofxDspFilter(int objectId)
{
	mObjectId	= objectId;
	mObjectType	= kofxDsp_Filter;
	mParamIndex	= mGlobals->requestParameter(mObjectId, mObjectType);
	mParameter	= &mGlobals->mParameter[mParamIndex];
	
	mCutOff		= 0.0f;
	mResonance	= 0.0f;
	
	memset(&svf[0], 0, 10 * sizeof(float));
}

//	----------------------------------------------------------------------------------------------------

void ofxDspFilter::render(float* buffer, int bufferSize)
{
	bool	active		= mParameter[kofxDsp_Filter_Active].value == kofxDsp_Active;
	int		type		= (int)mParameter[kofxDsp_Filter_Type].value;
	float	cutOff		= 2.0f * mParameter[kofxDsp_Filter_Cutoff].value / mGlobals->mSampleRate;
	float	resonance	= mParameter[kofxDsp_Filter_Resonance].value;

	if(!active)
		return;

	cutOff		= cutOff * cutOff * cutOff;
	resonance	= 1.0 - resonance * 0.98;

	float	higL	= svf[0];
	float	midL	= svf[1];
	float	lowL	= svf[2];
	float	rejL	= svf[3];
	float	pekL	= svf[4];
	
	float	higR	= svf[5];
	float	midR	= svf[6];
	float	lowR	= svf[7];
	float	rejR	= svf[8];
	float	pekR	= svf[9];
	
	float	in;
	float*	outL;
	float*	outR;
		
	float	ramp			= 1.0f / bufferSize;
	
	float	l_cutOff		= mCutOff;
	float	cutOffDelta		= (cutOff - l_cutOff) * ramp;
	
	float	l_resonance		= mResonance;
	float	resonanceDelta	= (resonance - l_resonance) * ramp;

	switch(type)
	{
		default:
		case kofxDsp_Filter_LP12: outL = &lowL; outR = &lowR; break;
		case kofxDsp_Filter_HP12: outL = &higL; outR = &higR; break;
		case kofxDsp_Filter_BP12: outL = &midL; outR = &midR; break;
		case kofxDsp_Filter_BR12: outL = &rejL; outR = &rejR; break;
		case kofxDsp_Filter_PK12: outL = &pekL; outR = &pekR; break;
	}
	
	while(bufferSize--)
	{
		l_cutOff	+= cutOffDelta;
		l_resonance	+= resonanceDelta;
		
		in			 = *buffer;
		rejL		 = fastTanh(in - (l_resonance * midL));
		lowL		+= midL * l_cutOff;
		higL		 = rejL - lowL;
		midL		+= higL * l_cutOff;
		pekL		 = (lowL - higL) * 0.65;

		rejL		 = fastTanh(in - (l_resonance * midL));
		lowL		+= midL * l_cutOff;
		higL		 = rejL - lowL;
		midL		+= higL * l_cutOff;
		pekL		 = (lowL - higL) * 0.65;

		*buffer++	 = *outL;

		in			 = *buffer;
		rejR		 = fastTanh(in - (l_resonance * midR));
		lowR		+= midR * l_cutOff;
		higR		 = rejR - lowR;
		midR		+= higR * l_cutOff;
		pekR		 = (lowR - higR) * 0.65;

		in			 = *buffer;
		rejR		 = fastTanh(in - (l_resonance * midR));
		lowR		+= midR * l_cutOff;
		higR		 = rejR - lowR;
		midR		+= higR * l_cutOff;
		pekR		 = (lowR - higR) * 0.65;

		*buffer++	 = *outR;
	}
	
	mCutOff		= l_cutOff;
	mResonance	= l_resonance;
	
	svf[0]		= higL;
	svf[1]		= midL;
	svf[2]		= lowL;
	svf[3]		= rejL;
	svf[4]		= pekL;
	
	svf[5]		= higR;
	svf[6]		= midR;
	svf[7]		= lowR;
	svf[8]		= rejR;
	svf[9]		= pekR;
}

//	----------------------------------------------------------------------------------------------------

