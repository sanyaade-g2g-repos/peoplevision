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

class ofxCYASettings {
public:
	static ofxCYASettings* getInstance();
	void method();
	~ofxCYASettings() {
		instanceFlag = false;
	}
	
	// tracking vars
	bool bLearnBackground;
	bool bSmartLearnBackground;
	bool bLearnBackgroundProgressive;
	bool bTrackDark;
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
	int minBlob;
	int maxBlob;
	float fLearnRate;
	float haarArea;
	float minHaarArea;
	float maxHaarArea;
	bool bCentroidDampen;
	
private:
	static bool instanceFlag;
	static ofxCYASettings *single;
	ofxCYASettings() {
		//private constructor
	}
};