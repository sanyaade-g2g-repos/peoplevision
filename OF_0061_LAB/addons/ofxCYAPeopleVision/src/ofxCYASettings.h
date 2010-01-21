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

	// tracking vars
	bool bLearnBackground;
//	bool bSmartLearnBackground; //jg disabled this feature
	bool bLearnBackgroundProgressive;
	int trackType;
	bool bHighpass;
	bool bAmplify;
	bool bSmooth;
	bool bDetectHaar;
	bool bUseHaarAsCenter;
	bool bTrackOpticalFlow;
	bool bFindHoles;
	int smooth;
	int highpassBlur;
	int highpassNoise;
	int	highpassAmp;
	int threshold;	
	float minBlob;
	float maxBlob;
	float fLearnRate;
	float haarAreaPadding;
//	float minHaarArea;
//	float maxHaarArea;
	bool bCentroidDampen;
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