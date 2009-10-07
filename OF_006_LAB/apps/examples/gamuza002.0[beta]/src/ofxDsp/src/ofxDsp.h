/*
 *  ofxDsp.h
 *  ofxDsp
 *
 *  Created by Stefan Kirch on 21.07.08.
 *  Copyright 2008 alphakanal. All rights reserved.
 *
 */

//	----------------------------------------------------------------------------------------------------

#ifndef OFX_DSP
#define OFX_DSP

//	----------------------------------------------------------------------------------------------------

#include "ofxDspTypes.h"

#include "ofxDspAmplifier.h"
#include "ofxDspFilter.h"
#include "ofxDspOscillator.h"
#include "ofxDspShaper.h"

//	----------------------------------------------------------------------------------------------------

class ofxDsp
{
public:
	
	static			ofxDsp* Instance(ofSimpleApp* app);
	
	ofxDspObject*	addObject(int objectType);
	void			removeObject(int objectId);

	void			activate(bool activate);
	
	void			render(float* buffer, int bufferSize, int numberOfChannels);
	void			draw(float* scope, float* buffer, int bufferSize, int numberOfChannels);

	void			clearBuffer(float* buffer, int bufferSize);
	void			removeDC(float* buffer, int bufferSize);
	
	int				mObjectId;
	ofxDspGlobals*	mGlobals;
	vector			<ofxDspObject*>	mObjects;
	bool			mIsActive;
	float			dc[4];
	
private:
	
	ofxDsp(ofSimpleApp* app);
};

//	----------------------------------------------------------------------------------------------------

#endif

//	----------------------------------------------------------------------------------------------------
