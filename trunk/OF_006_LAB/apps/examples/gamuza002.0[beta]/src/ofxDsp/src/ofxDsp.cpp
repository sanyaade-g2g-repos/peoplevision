/*
 *  ofxDsp.cpp
 *  ofxDsp
 *
 *  Created by Stefan Kirch on 21.07.08.
 *  Copyright 2008 alphakanal. All rights reserved.
 *
 */

//	----------------------------------------------------------------------------------------------------

#include "ofxDsp.h"

//	----------------------------------------------------------------------------------------------------

ofxDsp* ofxDsp::Instance(ofSimpleApp* simpleApp)
{
	static ofxDsp dsp(simpleApp);	
	return &dsp;
}

//	----------------------------------------------------------------------------------------------------

ofxDsp::ofxDsp(ofSimpleApp* simpleApp)
{
	mObjectId				= 0;
	mGlobals				= ofxDspGlobals::Instance();
	mGlobals->mSampleRate	= OFXDSP_SAMPLERATE;
	mGlobals->mSimpleApp	= simpleApp;
	
	memset(&dc[0], 0, 4 * sizeof(float));
}

//	----------------------------------------------------------------------------------------------------

ofxDspObject* ofxDsp::addObject(int objectType)
{
	ofxDspObject*	object		= NULL;
	int				objectId	= mObjectId;
	
	switch(objectType)
	{
		case kofxDsp_Amplifier:
			object = new ofxDspAmplifier(mObjectId);
			break;
		case kofxDsp_Filter:
			object = new ofxDspFilter(mObjectId);
			break;
		case kofxDsp_Oscillator:
			object = new ofxDspOscillator(mObjectId);			
			break;
		case kofxDsp_Shaper:
			object = new ofxDspShaper(mObjectId);
			break;
	}
	
	if(object)
	{
		mObjects.push_back(object);
		mObjectId++;
	}
	
	return object;
}

//	----------------------------------------------------------------------------------------------------

void ofxDsp::removeObject(int objectId)
{
	for(int i = 0; i < mObjects.size(); i++)
	{
		ofxDspObject* object = (ofxDspObject*)mObjects.at(i);
		
		if(object->mObjectId == objectId)
			mObjects.erase (mObjects.begin() + i);
	}	
}

//	----------------------------------------------------------------------------------------------------

void ofxDsp::activate(bool activate)
{
	mIsActive = activate;
	
	if(activate)
		ofSoundStreamSetup(OFXDSP_CHANNELS, 0, mGlobals->mSimpleApp, OFXDSP_SAMPLERATE, OFXDSP_BUFFERSIZE, 4);
	else
		ofSoundStreamClose();
}

//	----------------------------------------------------------------------------------------------------

void ofxDsp::render(float* buffer, int bufferSize, int numberOfChannels)
{
	if(numberOfChannels != OFXDSP_CHANNELS)
		return;
	
	clearBuffer(buffer, bufferSize);
	
	for(int i = 0; i < mObjects.size(); i++)
	{
		ofxDspObject* tmpObj = (ofxDspObject*)mObjects.at(i);
		tmpObj->render(buffer, bufferSize);
	}

	removeDC(buffer, bufferSize);
}

//	----------------------------------------------------------------------------------------------------

void ofxDsp::draw(float* scope, float* buffer, int bufferSize, int numberOfChannels)
{
	if(numberOfChannels != OFXDSP_CHANNELS)
		return;

	if(!scope)
		return;

	while(bufferSize--)
		*scope++ = hardClip((*buffer++ + *buffer++) * 0.5f);
}

//	----------------------------------------------------------------------------------------------------

void ofxDsp::clearBuffer(float* buffer, int bufferSize)
{
	while(bufferSize--)
	{
		*buffer++	= OFXDSP_DENORMALS;
		*buffer++	= OFXDSP_DENORMALS;		
	}
}

//	----------------------------------------------------------------------------------------------------

void ofxDsp::removeDC(float* buffer, int bufferSize)
{
	float	dc1L	= dc[0];
	float	dc2L	= dc[1];
	float	dc1R	= dc[2];
	float	dc2R	= dc[3];
	
	float	in;
	
	while(bufferSize--)
	{
		in			= *buffer;
		dc1L		= 0.999 * dc1L + in - dc2L;
		dc2L		= in;
		*buffer++	= dc1L;		

		in			= *buffer;
		dc1R		= 0.999 * dc1R + in - dc2R;
		dc2R		= in;
		*buffer++	= dc1R;		
	}
	
	dc[0]	= dc1L;
	dc[1]	= dc2L;
	dc[2]	= dc1R;
	dc[3]	= dc2R;
}

//	----------------------------------------------------------------------------------------------------
