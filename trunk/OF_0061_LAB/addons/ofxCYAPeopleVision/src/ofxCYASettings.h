/*
 *  ofxCYASettings.h
 *  CYAPeopleVision
 *
 *  Created by Joshua Walton on 12/7/09.
 *  Copyright 2009 Lab at Rockwell Group. All rights reserved.
 *
 */

#include "ofMain.h"
#include <iostream>

enum {
	TRACK_DARK, TRACK_LIGHT, TRACK_ABSOLUTE
};

class ofxCYASettings {
public:
	static ofxCYASettings* getInstance();
	void method();
	~ofxCYASettings() {
		delete single;
		instanceFlag = false;
	}
	
	string haarFile;

	// image vars
	int smooth;
	int highpassBlur;
	int highpassNoise;
	int	highpassAmp;
	int threshold;	
	bool bHighpass;
	bool bAmplify;
	bool bSmooth;
	bool bLearnBackground;
	
	// tracking vars
	bool bLearnBackgroundProgressive;
	float fLearnRate;
	int trackType;
	bool bFindHoles;
	float minBlob;
	float maxBlob;
	bool bCentroidDampen;
	
	//sensing vars
	bool bTrackOpticalFlow;
	float minOpticalFlow;
	float maxOpticalFlow;
	bool bDetectHaar;
	float haarAreaPadding;
//	float minHaarArea;
//	float maxHaarArea;
	
	// communication vars
	bool sendOsc;
	bool sendTuio;
	string oscHost;
	int oscPort;
	string tuioHost;
	int tuioPort;
	
	ofPoint quadWarpScaled[4];
	ofPoint quadWarpOriginal[4];
	
private:
	
	static bool instanceFlag;
	static ofxCYASettings *single;
	ofxCYASettings() {
		//private constructor
	}
};