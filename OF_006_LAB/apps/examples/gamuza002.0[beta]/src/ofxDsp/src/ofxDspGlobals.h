/*
 *  ofxDspGlobals.h
 *  ofxDsp
 *
 *  Created by Stefan Kirch on 22.07.08.
 *  Copyright 2008 alphakanal. All rights reserved.
 *
 */

//	----------------------------------------------------------------------------------------------------

#ifndef OFX_DSP_GLOBALS
#define OFX_DSP_GLOBALS

//	----------------------------------------------------------------------------------------------------

#include "ofxDspTypes.h"

//	----------------------------------------------------------------------------------------------------

class ofxDspGlobals
{	
public:
	
	static			ofxDspGlobals* Instance();
	int				requestParameter(int objectId, int objectType);
	void			setParameter(int id, float value);
	void			setNormalizedParameter(int id, float value);
	
	float			mSampleRate;
	ofSimpleApp*	mSimpleApp;
	
	int				mParamIndex;
	ofxDspParameter	mParameter[OFXDSP_MAXPARAM];	//	todo
	
private:
	
	ofxDspGlobals();	
};

//	----------------------------------------------------------------------------------------------------

#endif

//	----------------------------------------------------------------------------------------------------
