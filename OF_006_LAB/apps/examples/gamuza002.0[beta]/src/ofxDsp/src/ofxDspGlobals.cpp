/*
 *  ofxDspGlobals.cpp
 *  ofxDsp
 *
 *  Created by Stefan Kirch on 22.07.08.
 *  Copyright 2008 alphakanal. All rights reserved.
 *
 */

//	----------------------------------------------------------------------------------------------------

#include "ofxDspGlobals.h"

//	----------------------------------------------------------------------------------------------------

ofxDspGlobals* ofxDspGlobals::Instance()
{
	static ofxDspGlobals globals;	
	return &globals;
}

//	----------------------------------------------------------------------------------------------------

ofxDspGlobals::ofxDspGlobals()
{
	mSampleRate = OFXDSP_SAMPLERATE;
	mParamIndex	= 0;
}

//	----------------------------------------------------------------------------------------------------

int ofxDspGlobals::requestParameter(int objectId, int objectType)
{
	int					index		= mParamIndex;
	ofxDspParameter*	parameter	= &mParameter[mParamIndex];
	ofxDspObjectInfo	objectInfo	= kofxDsp_Object_Info[objectType];
		
	for (int i = 0; i < objectInfo.numberOfParameter; i++)
	{
		parameter[i].objectId		= objectId;
		parameter[i].objectType		= objectType;
		parameter[i].parameterId	= i;
		parameter[i].value			= objectInfo.parameterInfo[i].value;
	}
	
	mParamIndex += objectInfo.numberOfParameter;
	
	return index;
}

//	----------------------------------------------------------------------------------------------------

void ofxDspGlobals::setParameter(int id, float value)
{
	if(id < OFXDSP_MAXPARAM)	//	todo
		mParameter[id].value = value;
}

//	----------------------------------------------------------------------------------------------------

void ofxDspGlobals::setNormalizedParameter(int id, float value)
{
	if(id < OFXDSP_MAXPARAM)	//	todo
	{
		ofxDspParameter*	parameter		= &mParameter[id];
		ofxDspObjectInfo	objectInfo		= kofxDsp_Object_Info[parameter->objectType];
		ofxDspParameterInfo	parameterInfo	= objectInfo.parameterInfo[parameter->parameterId];
		float				min				= parameterInfo.min;
		float				max				= parameterInfo.max;
		float				delta			= max - min;
		
		parameter->value =  min + value * delta;
	}
}

//	----------------------------------------------------------------------------------------------------
