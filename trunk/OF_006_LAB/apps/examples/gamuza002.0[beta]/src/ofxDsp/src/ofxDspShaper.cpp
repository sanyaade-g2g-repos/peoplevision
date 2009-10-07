/*
 *  ofxDspShaper.cpp
 *  ofxDsp
 *
 *  Created by Stefan Kirch on 22.07.08.
 *  Copyright 2008 alphakanal. All rights reserved.
 *
 */

//	----------------------------------------------------------------------------------------------------

#include "ofxDspShaper.h"

//	----------------------------------------------------------------------------------------------------

ofxDspShaper::ofxDspShaper(int objectId)
{
	mObjectId	= objectId;
	mObjectType	= kofxDsp_Shaper;
	mParamIndex	= mGlobals->requestParameter(mObjectId, mObjectType);
	mParameter	= &mGlobals->mParameter[mParamIndex];
}

//	----------------------------------------------------------------------------------------------------

void ofxDspShaper::render(float* buffer, int bufferSize)
{
	bool	active	= mParameter[kofxDsp_Shaper_Active].value == kofxDsp_Active;
	int		type	= (int)mParameter[kofxDsp_Shaper_Type].value;
	float	drive	= mParameter[kofxDsp_Shaper_Drive].value;
	float	level	= mParameter[kofxDsp_Shaper_Level].value;

	if(!active)
		return;

	switch(type)
	{
		case kofxDsp_Shaper_Diode:
		{
			drive = halfDouble(drive);
			level = halfDouble(level);
			
			while(bufferSize--)
			{
				*buffer++	= fastTanh(*buffer * drive) * level;
				*buffer++	= fastTanh(*buffer * drive) * level;
			}
			break;
		}
			
		case kofxDsp_Shaper_Silicon:
		{
			drive  *= 0.98f;
			drive	= 2.0f * drive / (1.0f - drive);
			level	= halfDouble(level) * 0.5f;
			
			float	in;
			
			while(bufferSize--)
			{
				in			= *buffer;
				*buffer++	= ((1.0f + drive) * in / (1.0f + drive * fabsf(in))) * level;
				in			= *buffer;
				*buffer++	= ((1.0f + drive) * in / (1.0f + drive * fabsf(in))) * level;
			}
			break;
		}
	}
}

//	----------------------------------------------------------------------------------------------------

