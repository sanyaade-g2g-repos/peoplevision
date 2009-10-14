/*
 *  ofxPersonTuio.h
 *  peopleTracker_000
 *
 *  Created by Brett Renfer on 10/14/09.
 *  Copyright 2009 Rockwell Group. All rights reserved.
 *
 */

#pragma once

#include "ofxPerson.h"
#include "ofxOsc.h"

class ofxPersonTuio : public ofxPerson
{
	public:
		
		ofxOscMessage getTuio()
		{
			ofxOscMessage m;
			
			m.setAddress("/tuio/2Dcur/set");
			
			return m;
		};
	
};
