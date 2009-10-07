/*
 *  ofxDspObject.cpp
 *  ofxDsp
 *
 *  Created by Stefan Kirch on 21.07.08.
 *  Copyright 2008 alphakanal. All rights reserved.
 *
 */

//	----------------------------------------------------------------------------------------------------

#include "ofxDspObject.h"

//	----------------------------------------------------------------------------------------------------

ofxDspObject::ofxDspObject()
{
	mObjectId	= -1;
	mObjectType	= -1;
	mParameter	= NULL;
	mGlobals	= ofxDspGlobals::Instance();
}

//	----------------------------------------------------------------------------------------------------
