/*
* Copyright 2008 NOR_/D <http://nortd.com>
*
* This file is part of ofxTouch.
* ofxTouch is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as 
* published by the Free Software Foundation, either version 3 of 
* the License, or (at your option) any later version. For details
* see <http://www.gnu.org/licenses/>.
*
* * *
* This set of classes manage calibration settings.
* They both provide a simple key-driven GUI and an xml
* file accessor that automatically writes/loads settings
* to/from file.
*
* NOTE: These classes are highly dependent on
* ofxTouchGraphicsWarp, ofxTouchVisionWarp, ofxTouchApp
* You are on your own if you reuse code from here!
*
*/


#ifndef OFX_TOUCH_SETTINGS
#define OFX_TOUCH_SETTINGS


#include "ofMain.h"
#include "ofxXmlSettings.h"

class ofxTouchApp;
class ofxTouchSettings;


class ofxTouchSettingsItem {
  public:

    ofxTouchApp* app;
     ofxTouchSettings* touchSettings;
    int heightList;
    int heightFocused;
    int margin;
    string name;
    string label;
    ofTrueTypeFont* font;
   

    ofxTouchSettingsItem( ofxTouchApp* _app, ofxTouchSettings* _touchSettings ) {
        app = _app;
        touchSettings = _touchSettings;
        heightList = 50;
        heightFocused = 250;
        margin = 3;
    }
    
    virtual void onFocus() {}
    virtual void applySettings() = 0;
    virtual void loadSettings( ofxXmlSettings& XML ) = 0;
    virtual void saveSettings( ofxXmlSettings& XML ) = 0;    
    virtual void setWriter( ofTrueTypeFont* _font );    
    virtual int getHeight( int width );
    virtual void draw( int x, int y, int width, bool selected=false );
    virtual void drawFocused( int x, int y, int width );
    virtual void keyPressed( int key );
    
};






class ofxTouchSettingsSlider: public ofxTouchSettingsItem {
  public:

    float value;
    float min;
    float max;

    ofxTouchSettingsSlider( ofxTouchApp* _app, ofxTouchSettings* _touchSettings ) : ofxTouchSettingsItem( _app, _touchSettings ) {
        label = "";
        value = 0;
        min = 0;
        max = 10;
        heightFocused = 150;
    }
    
    virtual void applySettings() {}  
    virtual void loadSettings( ofxXmlSettings& XML );
    virtual void saveSettings( ofxXmlSettings& XML );    
    void draw( int x, int y, int width, bool selected=false );
    void drawFocused( int x, int y, int width );
    void keyPressed( int key );
    
};




class ofxTouchSettingsWarp: public ofxTouchSettingsItem {
  public:
    
    int selected;
    bool focused;
    ofPoint quad[4];
    ofPoint maxQuad;
    string cornernames[4];
  
    ofxTouchSettingsWarp( ofxTouchApp* _app, ofxTouchSettings* _touchSettings, 
                          int maxWidth, int maxHeight ) 
                          : ofxTouchSettingsItem( _app, _touchSettings ) {
        selected = 0;
        focused = false;
        maxQuad.x = maxWidth;
        maxQuad.y = maxHeight;
        quad[0].x = 0; quad[0].y = 0;
        quad[1].x = maxQuad.x; quad[1].y = 0;
        quad[2].x = maxQuad.x; quad[3].y = maxQuad.y;
        quad[3].x = 0; quad[2].y = maxQuad.y;         
        cornernames[0] = "top-left";
        cornernames[1] = "top-right";
        cornernames[2] = "bottom-right";
        cornernames[3] = "bottom-left";
        heightFocused = 200+2*margin;        
    }

    virtual void onFocus();
    virtual void setCornerSelector( int corner ) {}
    virtual void applySettings() {}    
    virtual void loadSettings( ofxXmlSettings& XML );
    virtual void saveSettings( ofxXmlSettings& XML );
    virtual void draw( int x, int y, int width, bool selected=false );
    virtual void drawFocused( int x, int y, int width );
    void keyPressed( int key );
    
};








//////////
/////////
/////////

//LAB ADDITION: MAX BLOB SIZE SLIDER

class ofxTouchSettingsSliderMaxBlobSize: public ofxTouchSettingsSlider {
public:
	
	
    ofxTouchSettingsSliderMaxBlobSize( 
									ofxTouchApp* _app, ofxTouchSettings* _touchSettings )
	: ofxTouchSettingsSlider( _app, _touchSettings ) {}
	
    void applySettings();
    
};

class ofxTouchSettingsSliderThreshold: public ofxTouchSettingsSlider {
  public:
  

    ofxTouchSettingsSliderThreshold( 
        ofxTouchApp* _app, ofxTouchSettings* _touchSettings )
        : ofxTouchSettingsSlider( _app, _touchSettings ) {}

    void applySettings();
    
};



class ofxTouchSettingsWarpGraphics: public ofxTouchSettingsWarp {
  public:
  
    ofxTouchSettingsWarpGraphics(
        ofxTouchApp* _app, ofxTouchSettings* _touchSettings, 
        int maxWidth, int maxHeight ) 
        : ofxTouchSettingsWarp( _app, _touchSettings, 
                                maxWidth, maxHeight ) {}

    void setCornerSelector( int corner );        
    void applySettings();
    
};


class ofxTouchSettingsWarpVision: public ofxTouchSettingsWarp {
  public:
  
    ofxTouchSettingsWarpVision(
        ofxTouchApp* _app, ofxTouchSettings* _touchSettings, 
        int maxWidth, int maxHeight ) 
        : ofxTouchSettingsWarp( _app, _touchSettings, 
                                maxWidth, maxHeight ) {}

    void setCornerSelector( int corner );        
    void applySettings();
    
};


//////////
/////////
/////////





class ofxTouchSettings {
  public:

    int x;
    int y;
    int width;
    int height;
    int maxHeight;
    bool overflow;
    int selected;
    bool focused;
    vector<ofxTouchSettingsItem*> items;
    ofxTouchApp* app;
    ofTrueTypeFont	labelWriter;
    ofxXmlSettings XML;
       
   
    ofxTouchSettings() {
        x = 100;
        y = 0.1*ofGetHeight();
        width = 250;
        maxHeight = 300;
        overflow = false;
        selected = 0;
        focused = false;
    }
    
    virtual ~ofxTouchSettings() {
        for( int i=0; i<items.size(); ++i ) {
            delete items[i];
        }    
    }

    void setup( ofxTouchApp* _app );
    ofxTouchSettingsSlider* addNewSlider( string label );
    void loadSettings( string path );
    void saveSettings( string path );
    void draw();
    void drawList();
    void drawFocused();
    void keyPressed( int key );
    
};



#endif


