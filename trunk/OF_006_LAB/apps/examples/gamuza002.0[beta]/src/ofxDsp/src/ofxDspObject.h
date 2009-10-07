/*
 *  ofxDspObject.h
 *  ofxDsp
 *
 *  Created by Stefan Kirch on 21.07.08.
 *  Copyright 2008 alphakanal. All rights reserved.
 *
 */

//	----------------------------------------------------------------------------------------------------

#ifndef OFX_DSP_OBJECT
#define OFX_DSP_OBJECT

//	----------------------------------------------------------------------------------------------------

#include "ofxDspTypes.h"
#include "ofxDspGlobals.h"

//	----------------------------------------------------------------------------------------------------

class ofxDspObject
{		
public:
	
	ofxDspObject();
	
	virtual	void		render(float* buffer, int bufferSize){}
	virtual	int			getParameterId(int parameter){return mParamIndex + parameter;}
	
	int					mObjectId, mObjectType, mParamIndex;
	ofxDspParameter*	mParameter;
	ofxDspGlobals*		mGlobals;
};

//	----------------------------------------------------------------------------------------------------

#endif

//	----------------------------------------------------------------------------------------------------
