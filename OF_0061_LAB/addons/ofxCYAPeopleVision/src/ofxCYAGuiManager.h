/*
 *  ofxCYAGuiManager.h
 *  CYAPeopleVision
 *
 *  Created by Joshua Walton on 12/7/09.
 *  Copyright 2009 Lab at Rockwell Group. All rights reserved.
 *
 */

//#include "ofxSimpleGuiToo.h"
#include "ofxControlPanel.h"

// maybe there's a simpler way to integrate this?
#include "ofxCvCoordWarpingGui.h"

typedef struct  {	
	int type;
	string key;
	
	float* f;
	int* i;	
	bool* b;
	
} ofxCYAGUICustomParam;

class ofxCYAGuiManager {
	
  public:
	ofxCYAGuiManager();
	//ofxSimpleGuiToo	gui;
	
	bool enableGui;	
	void setupQuadGui ( int cameraWidth, int cameraHeight );
	void drawQuadGui();
	void drawQuadGui( int x, int y, int width, int height );
	
	void update(ofEventArgs &e);
	void draw(ofEventArgs &e);
	
	//forwarded events
	void mousePressed(ofMouseEventArgs &e);	
	void mouseDragged(ofMouseEventArgs &e);	
	void mouseReleased(ofMouseEventArgs &e);
	
	void keyPressed(ofKeyEventArgs &e);
	void keyReleased(ofKeyEventArgs &e);
	
	void addSlider(string name, int* value, int min, int max);
	void addSlider(string name, float* value, float min, float max);
	void addToggle(string name, bool* value);
	

  protected:
	//a little goofy way to keep track of custom params. works but i'm open to suggestions
	ofxControlPanel panel;
	vector<ofxCYAGUICustomParam> params;
	
	//an even goofier way to easily add the quad qui
	bool quadGuiSetup;
	ofxCvCoordWarpingGui quadGui;
	
};