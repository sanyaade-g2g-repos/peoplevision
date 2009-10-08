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
*/

#include "ofxTouchSettings.h"
#include "ofxTouchApp.h"


   

void ofxTouchSettingsItem::setWriter( ofTrueTypeFont* _font ) {
    font = _font;
}

int ofxTouchSettingsItem::getHeight( int width ) {
    return heightList;
}



void ofxTouchSettingsItem::draw( int x, int y, int width, bool selected ) {
    ofNoFill();
    if( selected ) {
        ofSetColor( 100, 100, 100, 255 );
    } else {
        ofSetColor( 100, 100, 100, 100 );
    }
    ofRect( x+margin, y+margin, width-2*margin-1, heightList-2*margin-1 );        
}



void ofxTouchSettingsItem::drawFocused( int x, int y, int width ) {
    ofFill();
    ofSetColor( 100, 100, 100, 100 );
    ofRect( x, y, width, heightFocused );        
}
    

void ofxTouchSettingsItem::keyPressed( int key ) {

}








///////
///////
///////

void ofxTouchSettingsSlider::loadSettings( ofxXmlSettings& XML ) {
    value = XML.getValue(name+":value", 60);
}


void ofxTouchSettingsSlider::saveSettings( ofxXmlSettings& XML ) {
    int tagNum = XML.addTag(name);  
    XML.setValue(name+":value", value, tagNum);
}
    
    
void ofxTouchSettingsSlider::draw( int x, int y, int width, bool selected ) {
    ofNoFill();
    if( selected ) {
        ofSetColor( 100, 100, 100, 255 );
        ofRect( x+margin, y+margin, width-2*margin-1, heightList-2*margin-1 );
    } else {
        ofSetColor( 100, 100, 100, 100 );
    }
    if( font != NULL ) {
        ofSetColor( 255, 255, 255, 100 );
        font->drawString( label, x+(width/2.0)-(font->stringWidth(label)/2.0),
                                 y+(heightList/2.0)+(font->stringHeight(label)/4.0) );
    }
}


void ofxTouchSettingsSlider::drawFocused( int x, int y, int width ) {
    ofFill();
    ofSetColor( 100, 100, 100, 100 );
    ofRect( x, y, width, heightFocused );
    if( font != NULL ) {
        int valX = x+width/2.0;
        int valY = y+0.45*heightFocused;
        
        ofSetColor( 255, 255, 255, 100 );
        font->drawString( label, x+(width/2.0)-(font->stringWidth(label)/2.0),
                                 y+1.5*font->getLineHeight()-heightList );
        font->drawString( ofToString(value, 0),
                          valX-(font->stringWidth(ofToString(value, 0))/2.0),
                          valY+(font->stringHeight(ofToString(value, 0))/4.0) );
                          

        ofTriangle( valX, valY+24, valX+8, valY+16, valX-8, valY+16);
        ofTriangle( valX, valY-24, valX+8, valY-16, valX-8, valY-16);                          
    }                
}
    

void ofxTouchSettingsSlider::keyPressed( int key ) {
    if( key == OF_KEY_DOWN ) {
        value -= 1;
        if( value < min ) { value = min; }
    } else if( key == OF_KEY_UP ) {
        value += 1;
        if( value > max ) { value = max; }
    } else if( key == OF_KEY_BACKSPACE || key == OF_KEY_DEL ) {
        touchSettings->focused = false;
    }
    
    applySettings();
}  





///////
///////
///////

void ofxTouchSettingsWarp::onFocus() {
    setCornerSelector( selected );
}

void ofxTouchSettingsWarp::loadSettings( ofxXmlSettings& XML ) {
    quad[0].x = XML.getValue(name+":corner0:x", 0);
    quad[0].y = XML.getValue(name+":corner0:y", 0);
    quad[1].x = XML.getValue(name+":corner1:x", maxQuad.x);
    quad[1].y = XML.getValue(name+":corner1:y", 0);
    quad[2].x = XML.getValue(name+":corner2:x", maxQuad.x);
    quad[2].y = XML.getValue(name+":corner2:y", maxQuad.y);
    quad[3].x = XML.getValue(name+":corner3:x", 0);
    quad[3].y = XML.getValue(name+":corner3:y", maxQuad.y);
}


void ofxTouchSettingsWarp::saveSettings( ofxXmlSettings& XML ) {
    int tagNum = XML.addTag(name);  
    XML.setValue(name+":corner0:x", quad[0].x, tagNum);
    XML.setValue(name+":corner0:y", quad[0].y, tagNum);
    XML.setValue(name+":corner1:x", quad[1].x, tagNum);
    XML.setValue(name+":corner1:y", quad[1].y, tagNum);
    XML.setValue(name+":corner2:x", quad[2].x, tagNum);
    XML.setValue(name+":corner2:y", quad[2].y, tagNum);
    XML.setValue(name+":corner3:x", quad[3].x, tagNum);
    XML.setValue(name+":corner3:y", quad[3].y, tagNum);
}


void ofxTouchSettingsWarp::draw( int x, int y, int width, bool selected ) {
    ofNoFill();
    if( selected ) {
        ofSetColor( 100, 100, 100, 255 );
        ofRect( x+margin, y+margin, width-2*margin-1, heightList-2*margin-1 );
    } else {
        ofSetColor( 100, 100, 100, 100 );
    }
    if( font != NULL ) {
        ofSetColor( 255, 255, 255, 100 );
        font->drawString( label, x+(width/2.0)-(font->stringWidth(label)/2.0),
                                 y+(heightList/2.0)+(font->stringHeight(label)/4.0) );
    }
}


void ofxTouchSettingsWarp::drawFocused( int x, int y, int width ) {    
    ofFill();
    ofSetColor( 100, 100, 100, 100 );
    ofRect( x, y, width, heightFocused );
    ofSetColor( 255, 255, 255, 100 );
    font->drawString( label, x+(width/2.0)-(font->stringWidth(label)/2.0),
                             y+1.5*font->getLineHeight()-heightList );    
    
    if( focused ) {  //focused within the focused view
        if( font != NULL ) {
            int valX = x+width/2.0;
            int valY = y+0.4*heightFocused;   
                    
            font->drawString( cornernames[selected],
                              valX-(font->stringWidth(cornernames[selected])/2.0),
                              y+1.5*font->getLineHeight() ); 
                              
            font->drawString( ofToString( quad[selected].x, 0),
                              valX-(font->stringWidth(ofToString(quad[selected].x, 0))/2.0),
                              valY+(font->stringHeight(ofToString(quad[selected].x, 0))/4.0) );
            ofTriangle( valX-34, valY, valX-26, valY+8, valX-26, valY-8);
            ofTriangle( valX+34, valY, valX+26, valY+8, valX+26, valY-8);
            
            valY += 3*font->getLineHeight();
            font->drawString( ofToString( quad[selected].y, 0),
                              valX-(font->stringWidth(ofToString(quad[selected].y, 0))/2.0),
                              valY+(font->stringHeight(ofToString(quad[selected].y, 0))/4.0) ); 
            ofTriangle( valX, valY+24, valX+8, valY+16, valX-8, valY+16);
            ofTriangle( valX, valY-24, valX+8, valY-16, valX-8, valY-16);                                                            

        }
    } else {
        int itemheight = heightList;
        ofNoFill();
        for( int i=0; i<4; ++i ) {
            if( i == selected ) {
                ofRect( x+margin, y+i*itemheight+margin, width-2*margin-1, itemheight );
            }
            font->drawString( cornernames[i], x+(width/2.0)-(font->stringWidth(cornernames[i])/2.0),
                                     y+margin+i*itemheight+(itemheight/2.0)+(font->stringHeight(cornernames[i])/4.0) );                         
        }    
    }
}


void ofxTouchSettingsWarp::keyPressed( int key ) {
    if( focused ) {
        if( key == OF_KEY_LEFT ) {
            quad[selected].x -= 1;
            if( quad[selected].x < 0 ) { quad[selected].x = 0; }
        } else if( key == OF_KEY_RIGHT ) {
            quad[selected].x += 1;
            if( quad[selected].x > maxQuad.x ) { quad[selected].x = maxQuad.x; }
        } else if( key == OF_KEY_UP ) {
            quad[selected].y -= 1;
            if( quad[selected].y < 0 ) { quad[selected].y = 0; }
        } else if( key == OF_KEY_DOWN) {
            quad[selected].y += 1;
            if( quad[selected].y > maxQuad.y ) { quad[selected].y = maxQuad.y; }
        } else if( key == OF_KEY_BACKSPACE || key == OF_KEY_DEL ) {
            focused = false;
        }
    } else {
        if( key == OF_KEY_UP ) {
            selected--;
            if( selected < 0 ) { selected = 3; }
            setCornerSelector( selected );
        } else if( key == OF_KEY_DOWN ) {
            selected++;
            if( selected >= 4 ) { selected = 0; }
            setCornerSelector( selected );
        } else if( key == OF_KEY_RETURN ) {
            focused = true;
        } else if( key == OF_KEY_BACKSPACE || key == OF_KEY_DEL ) {
            touchSettings->focused = false;
            setCornerSelector( -1 );
        }    
    }
    
    applySettings();
} 







///////
///////
///////


void ofxTouchSettingsWarpGraphics::setCornerSelector( int corner ) {
    app->selectedGraphicsWarpCorner = corner;
}


void ofxTouchSettingsWarpGraphics::applySettings() {
    app->graphicsWarp.corners[0].x = quad[0].x/maxQuad.x;    
    app->graphicsWarp.corners[0].y = quad[0].y/maxQuad.y;    
    app->graphicsWarp.corners[1].x = quad[1].x/maxQuad.x;    
    app->graphicsWarp.corners[1].y = quad[1].y/maxQuad.y;    
    app->graphicsWarp.corners[2].x = quad[2].x/maxQuad.x;    
    app->graphicsWarp.corners[2].y = quad[2].y/maxQuad.y;    
    app->graphicsWarp.corners[3].x = quad[3].x/maxQuad.x;    
    app->graphicsWarp.corners[3].y = quad[3].y/maxQuad.y;
    app->graphicsWarp.findMatrix();
}






///////
///////
///////

void ofxTouchSettingsWarpVision::setCornerSelector( int corner ) {
    app->selectedVisionWarpCorner = corner;
}


void ofxTouchSettingsWarpVision::applySettings() {
    app->visionWarp.corners[0].x = quad[0].x;    
    app->visionWarp.corners[0].y = quad[0].y;    
    app->visionWarp.corners[1].x = quad[1].x;    
    app->visionWarp.corners[1].y = quad[1].y;    
    app->visionWarp.corners[2].x = quad[2].x;    
    app->visionWarp.corners[2].y = quad[2].y;    
    app->visionWarp.corners[3].x = quad[3].x;    
    app->visionWarp.corners[3].y = quad[3].y;
    app->visionWarp.findMatrix();
}









///////
///////
///////

void ofxTouchSettingsSliderThreshold::applySettings() {
    app->filter->threshold = (int)value;    
}

void ofxTouchSettingsSliderMaxBlobSize::applySettings() {
	app->maxCountourSize = (int) value;
}





///////
///////
///////



void ofxTouchSettings::setup( ofxTouchApp* _app ) {
    app = _app;
    labelWriter.loadFont( "verdana.ttf", 14, true, false );
    
    ofxTouchSettingsSliderThreshold* thresh = new ofxTouchSettingsSliderThreshold( app, this );
    thresh->name = "threshold";
    thresh->label = "Vision Threshold";
    thresh->setWriter( &labelWriter );
    thresh->min = 0;
    thresh->max = 255;
	thresh->value = app->filter->threshold;
    items.push_back( thresh );    
	
	ofxTouchSettingsSliderMaxBlobSize* maxBlobSize = new ofxTouchSettingsSliderMaxBlobSize( app, this );
    maxBlobSize->name = "maxblobsize";
    maxBlobSize->label = "Max Blob Size";
    maxBlobSize->setWriter( &labelWriter );
    maxBlobSize->min = 0;
    maxBlobSize->max = 10000;
    maxBlobSize->value = app->maxCountourSize;
    items.push_back( maxBlobSize );    

    ofxTouchSettingsWarpGraphics* gwarp = new ofxTouchSettingsWarpGraphics( app, this, ofGetWidth(), ofGetHeight() );
    gwarp->name = "graphicsWarp";
    gwarp->label = "Graphics Warp";
    gwarp->setWriter( &labelWriter );
    items.push_back( gwarp );    

    ofxTouchSettingsWarpVision* vwarp = new ofxTouchSettingsWarpVision( app, this, 640, 480 );
    vwarp->name = "visionWarp";
    vwarp->label = "Vision Warp";
    vwarp->setWriter( &labelWriter );
    items.push_back( vwarp );  
        
    
    loadSettings("settings.xml");      
}


void ofxTouchSettings::loadSettings( string path ) {
	cout << "loading settings.xml" << endl;
	
	if( XML.loadFile("settings.xml") ){
		cout << "settings.xml loaded!" << endl;
	}else{
		cout << "unable to load settings.xml check data/ folder" << endl;
	}
    
    for( int i=0; i<items.size(); ++i ) {
		cout<<"WORKING WITH "<<i<<endl;
        items[i]->loadSettings( XML );
        items[i]->applySettings();
    }
}


void ofxTouchSettings::saveSettings( string path ) {
    XML.clear();
    for( int i=0; i<items.size(); ++i ) {
        items[i]->saveSettings( XML );
    }
    XML.saveFile( path );
}
    

    

void ofxTouchSettings::draw() {
    ofEnableAlphaBlending();
    
    if( labelWriter.bLoadedOk ) {
        if( focused ) {
            drawFocused();
        } else {
            drawList();
        }
    } else {
        ofDrawBitmapString( "ERROR: TrueType Font failed to load. \nPlease make sure 'verdana.ttf' is in the data folder!", 100, 100 );
    }
            
    ofDisableAlphaBlending();        
}


void ofxTouchSettings::drawList() {
    ofFill();
    ofSetColor( 100, 100, 100, 255 );
    string label = "Calibration Menu";
    labelWriter.drawString( label, x+(width/2.0)-(labelWriter.stringWidth(label)/2.0),
                             y+1.5*labelWriter.getLineHeight()-50 );
    ofSetColor( 100, 100, 100, 100 );
    height = 0;
    for( int i=0; i<items.size(); ++i ) {
        height += items[i]->getHeight( width );
    }
    if( height > maxHeight )  {
        height = maxHeight;
        overflow = true;
    }
    ofRect( x,y, width,height );
    
    ofSetColor( 100, 100, 100, 150 );
    labelWriter.drawString( "Menu keys are:\n[ENTER] ... select an item.\n[BACKSPACE] ... exit, one level up.\n[ARROW KEYS] ... modify values.",
                            x+1.5*width, y+10 );          
    
    ofSetColor( 0xffffff );
    int yoff = 0;
    int itemheight = 0;
    for( int i=0; i<items.size(); ++i ) {
        itemheight = items[i]->getHeight( width );
        if( i == selected ) {
            items[i]->draw( x, y+yoff, width, true );
        } else {
            items[i]->draw( x, y+yoff, width, false );            
        }
        yoff += itemheight;
    }       
}


void ofxTouchSettings::drawFocused() {
    items[selected]->drawFocused( x,y, width );
}


void ofxTouchSettings::keyPressed( int key ) {
    if( focused ) {
        items[selected]->keyPressed( key );
    } else {
        if( key == OF_KEY_UP ) {
            selected--;
            if( selected < 0 ) {
                selected = items.size()-1;
            }
        } else if( key == OF_KEY_DOWN ) {
            selected++;
            if( selected >= items.size() ) {
                selected = 0;
            }
        } else if( key == OF_KEY_RETURN ) {
            focused = true;
            items[selected]->onFocus();
        } else if( key == OF_KEY_BACKSPACE || key == OF_KEY_DEL ) {
            //save settings here
            saveSettings("settings.xml");
            app->bSettingsMenu = false;
        }            
    }
}    
    