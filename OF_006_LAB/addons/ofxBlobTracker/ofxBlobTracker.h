/*
 *  ofxBlobTracker.h
 *  openFrameworks
 *
 * This file is part of ofxBlobTracker.
 * ofxBlobTracker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation, either version 3 of 
 * the License, or (at your option) any later version. For details
 * see <http://www.gnu.org/licenses/>.
 *  Created by Brett "the Factor" Renfer on 7/22/09.
 *  Copyright 2009 Rockwell Group. All rights reserved.
 *
 * BASED ON ofxTouchBlobTracker
 * Copyright 2008 NOR_/D <http://nortd.com>
 * * *
 * This class tracks blobs between frames.
 * Most importantly it assignes persistent id to new blobs, correlates
 * them between frames and removes them as blobs dissappear. It also
 * compensates for ghost frames in which blobs momentarily dissappear.
 *
 * Based on the tracking it fires events when blobs come into existence,
 * move around, and disappear. The object which receives the callbacks
 * can be specified with setListener().
 */
 
 
#ifndef OFX_BLOBTRACKER
#define OFX_BLOBTRACKER

#include <map>
#include <vector>
#include "ofMain.h"
#include "ofEvents.h"
#include "ofxTrackedBlob.h"

class ofxBlobTracker {

	public:

		vector<ofxTrackedBlob>  blobs;    

		ofxBlobTracker();
		
		void trackBlobs( const vector<ofxTouchBlob>& blobs );
		void draw( float x, float y, float scale=1.0 );  // draws all blobs
			 
		protected:

		int currentID;
		int extraIDs;
		int numcheck;

		ofxTouchBlobListener* listener;

		int reject_distance_threshold;
		int ghost_frames;
		float minimumDisplacementThreshold;

		vector<vector<int> > matrix;
		vector<int> ids;
		vector<vector<ofxTouchTrackedBlob> > history;


		void doBlobOn( const ofxTrackedBlob& b );    
		void doBlobMoved( const ofxTrackedBlob& b );    
		void doBlobOff( const ofxTrackedBlob& b );    

		inline void permute( int k );
		inline bool checkValid( int start );
		inline bool checkValidNew( int start );    
};
 
 
 
 #endif
 
