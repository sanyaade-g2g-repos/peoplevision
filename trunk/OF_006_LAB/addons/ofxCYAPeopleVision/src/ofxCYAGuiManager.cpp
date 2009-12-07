/*
 *  ofxCYAGuiManager.cpp
 *  CYAPeopleVision
 *
 *  Created by Joshua Walton on 12/7/09.
 *  Copyright 2009 Lab at Rockwell Group. All rights reserved.
 *
 */

#include "ofxCYAGuiManager.h"
#include "ofxCYASettings.h"

ofxCYAGuiManager::ofxCYAGuiManager() {
	//set gui
	//gui.addSlider(string name, int *value, int min, int max);
	ofxCYASettings *p_Settings;
	p_Settings = ofxCYASettings::getInstance();
	cout <<"Current bLearnBackground: "<<p_Settings->bLearnBackground<<endl;
	
	gui.addToggle("learn background", &p_Settings->bLearnBackground);
	gui.addToggle("smart learn background", &p_Settings->bSmartLearnBackground);
	gui.addSlider("threshold", &p_Settings->threshold, 0, 255);
	gui.addSlider("minimum blob", &p_Settings->minBlob, 0.0f, 640.0f*480.0f/10.0f);
	gui.addSlider("maximum blob", &p_Settings->maxBlob, 640.0f, 640.0f*480.0f*3);
	gui.addSlider("bg learn rate", &p_Settings->fLearnRate, 0.0f, 1000.0f);
	gui.addToggle("smoothing on", &p_Settings->bSmooth);
	gui.addSlider("smooth", &p_Settings->smooth, 0, 15);
	gui.addToggle("highpass on", &p_Settings->bHighpass);
	gui.addSlider("highpass blur", &p_Settings->highpassBlur, 0, 200);
	gui.addSlider("highpass noise", &p_Settings->highpassNoise, 0, 30);
	gui.addToggle("amplify on", &p_Settings->bAmplify);
	gui.addSlider("amplify", &p_Settings->highpassAmp, 0, 200);
	gui.addToggle("find holes", &p_Settings->bFindHoles);
	gui.addToggle("track optical flow", &p_Settings->bTrackOpticalFlow);
	gui.addToggle("track dark blobs", &p_Settings->bTrackDark);
	gui.addToggle("track faces", &p_Settings->bDetectHaar);
	gui.addSlider("haar threshold add", &p_Settings->haarArea, 0.0f, 200.0f);
	gui.addToggle("send haar center as blob center", &p_Settings->bUseHaarAsCenter);
	gui.addSlider("minimum haar checkable blob", &p_Settings->minHaarArea, 0.0f, 640.0f);
	gui.addSlider("maximum haar checkable blob", &p_Settings->maxHaarArea, 640.0f, 640.0f*480.0f);
	
	
	gui.loadFromXML();
	
	
}

void ofxCYAGuiManager::draw() {
	gui.draw();
}