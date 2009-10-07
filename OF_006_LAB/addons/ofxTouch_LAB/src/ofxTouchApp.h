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
* This is a ofSimpleApp equivalent which fires multitouch events.
*/

#ifndef _OFX_TOUCHAPP
#define _OFX_TOUCHAPP

#include "ofBaseApp.h"
#include "ofxTouchFinger.h"
#include "ofxTouchBlobListener.h"
#include "ofVideoGrabber.h"
#include "ofxTouchContourFinder.h"
#include "ofxTouchBlobTracker.h"
#include "ofxTouchAdaptiveFilter.h"
#include "ofxTouchGraphicsWarp.h"
#include "ofxTouchVisionWarp.h"
#include "ofxTouchSettings.h"


#define OFX_TOUCH_MAXBLOBSIZE 400

class ofxTouchApp : public ofSimpleApp, public ofxTouchBlobListener {

  public:
  
    int cwidth;
    int cheight;
    float xScaleToScreen;
    float yScaleToScreen;
	int maxCountourSize;
    
    vector<ofxTouchFinger>  fingers;
    
    bool bLiveVideo;
    string videoPlayerFile;
    ofVideoGrabber*  vidGrabber;
    ofVideoPlayer*  vidPlayer;

    ofxCvColorImage  colorImg;
    ofxCvGrayscaleImage  img;

    ofxCvGrayscaleImage  afterAcquiringImg;
    ofxCvGrayscaleImage  afterWarpImg;
    ofxCvGrayscaleImage  afterFilteringImg;

    ofxTouchFilter*  filter;
    ofxTouchContourFinder  contourFinder;
    ofxTouchBlobTracker  blobTracker;
    
    ofxTouchGraphicsWarp  graphicsWarp;
    int selectedGraphicsWarpCorner;
    
    ofxTouchVisionWarp  visionWarp;
    int selectedVisionWarpCorner;
    
    ofTrueTypeFont	ttfWriter;
    bool bSettingsMenu;
    
    
    
    ofxTouchApp( int _cwidth=320, int _cheight=240 ) {
        cwidth = _cwidth;
        cheight = _cheight;
        ttfWriter.loadFont( "verdana.ttf", 9, true, false );
        ttfWriter.setLineHeight(16);
        vidGrabber = NULL;
        vidPlayer = NULL;
        filter = NULL;
		
		maxCountourSize = OFX_TOUCH_MAXBLOBSIZE;
    }
    
    
    virtual ~ofxTouchApp() {
        if( filter != NULL ) {
            delete filter;
        }
        if( vidGrabber != NULL ) {
            delete vidGrabber;
        }        
        if( vidPlayer != NULL ) {
            delete vidPlayer;
        }     
    }
        
    
    virtual void _setup() {
        bSetupMode = false;
        bSimulate = false;
        xScaleToScreen = ofGetWidth()/(float)cwidth;
		yScaleToScreen = ofGetHeight()/(float)cheight;
        camFrameCount = 0;
        selectedGraphicsWarpCorner = -1;
        selectedVisionWarpCorner = -1;        
        
        bLiveVideo = true;
        videoPlayerFile = "touchkit-fingers.mov";

        colorImg.allocate(cwidth,cheight);
        img.allocate(cwidth,cheight);
        afterAcquiringImg.allocate(cwidth,cheight);
        afterWarpImg.allocate(cwidth,cheight);
        afterFilteringImg.allocate(cwidth,cheight);
        
        if( filter == NULL ) {
            filter = new ofxTouchAdaptiveFilter();
        }
        filter->allocate( cwidth, cheight );
        visionWarp.allocate( cwidth, cheight );    
        blobTracker.setListener( this );
        
        settingsMenu.setup( this );
        bSettingsMenu = false;
        
        ofSetVerticalSync( true );
        ofHideCursor();        
        
        setup();  //call setup in user app code
    }
    
    

    virtual void runComputerVision() {
        bool bNewFrame = false;
        
        
        if( bLiveVideo ) {
            if( vidGrabber == NULL ) {
                vidGrabber = new ofVideoGrabber();
                vidGrabber->setVerbose(true);
                vidGrabber->initGrabber(cwidth,cheight);            
            }
            vidGrabber->grabFrame();
            bNewFrame = vidGrabber->isFrameNew();            
        
        } else {
            if( vidPlayer == NULL ) {
                vidPlayer = new ofVideoPlayer();            
                vidPlayer->loadMovie( videoPlayerFile );
                vidPlayer->play();            
            }
            vidPlayer->idleMovie();
            bNewFrame = vidPlayer->isFrameNew();         
        
        }

        
        if (bNewFrame){
            if( bSetupMode ) {
                if( (camFrameCount % 10) == 0 ) {
                    camFps = 10000.0/(ofGetElapsedTimeMillis() - camFrameStartTime);
                    camFrameStartTime = ofGetElapsedTimeMillis();
                }
                dspStartTime = ofGetElapsedTimeMillis();
            }
            
            if( bLiveVideo ) {
                colorImg.setFromPixels(vidGrabber->getPixels(), cwidth,cheight);
            } else {
                colorImg.setFromPixels(vidPlayer->getPixels(), cwidth,cheight);
            }
            
            img = colorImg;
            img.mirror( false, true );
            if( bSetupMode ) { afterAcquiringImg = img; }            
            
            visionWarp.apply( img );
            if( bSetupMode ) { afterWarpImg = img; }
                      
            filter->apply( img );
            if( bSetupMode ) { afterFilteringImg = img; }

            //findContures( img, minSize, maxSize, nMax, inner contours yes/no )
          //  contourFinder.findContours( img, 5,400, 10, false );

			/*******************************************************
			 LAB EDIT: ADJUSTABLE MAX CONTOUR SIZE!
			*******************************************************/

            contourFinder.findContours( img, 5, maxCountourSize, 10, false );
            blobTracker.trackBlobs( contourFinder.blobs );
            
            if( bSetupMode ) {
                dspTime = ofGetElapsedTimeMillis() - dspStartTime;
                camFrameCount++;
            }
        }
    
    }    
    
    virtual void _update() {
        ofBackground(0,0,0);
        if( !bSimulate ) {
            // let the app repaint to begin with
            if( ofGetFrameNum() > 1 ) {
                runComputerVision();
            }
        }

        update();  //call update in user app code    
    }
    
    
    
    virtual void _draw() {
        graphicsWarp.apply();
        
        draw();  //call draw in user app code      
        
            
        ofSetColor( 0xffffff );
        if( bSetupMode && !bSimulate ) {
            if( ttfWriter.bLoadedOk ) {
                afterAcquiringImg.draw( 0,0, 256,192  );
                afterWarpImg.draw( 256,0, 256,192 );
                filter->draw( 0,256, 256,192 );            
                afterFilteringImg.draw( 256,256, 256,192 );
                blobTracker.draw( 256, 256, 256.0/cwidth );

                ofSetColor( 0x666666 );                
                ttfWriter.drawString( "input feed", 0+90, 192+12 );
                ttfWriter.drawString( "after image warp", 256+65, 192+12 );
                ttfWriter.drawString( "adaptive background", 512+60, 192+12 );
                ttfWriter.drawString( "tracking", 768+90, 192+12 );                                                    

                ofSetColor( 0xffffff );
                ttfWriter.drawString( "You are in SETUP MODE: \n\n[CTRL]+[s] ... toggle setup mode \n[CTRL]+[f] ... toggle full screen mode \n[ENTER] ... enter settings menu (only in setup mode) \n[c] ... camera setup (only with live feed, exit fullscreen first) \n[SPACE] ... reset background ", 
                                                512, ofGetHeight()-200 ); 
                ttfWriter.drawString( ofToString(dspTime) + "ms dsp", 
                                    512, ofGetHeight()-80 ); 
                ttfWriter.drawString( ofToString(camFps) + "fps (cam)", 
                                    512, ofGetHeight()-60 ); 
                ttfWriter.drawString( ofToString(ofGetFrameRate(), 2) + "fps", 
                                    512, ofGetHeight()-40 );
            } else {
                ofDrawBitmapString( "ERROR: TrueType Font failed to load. \nPlease make sure 'verdana.ttf' is in the data folder!", 100, 100 );
            }                                                                                                     
        }
        
        graphicsWarp.draw( selectedGraphicsWarpCorner );
        visionWarp.draw( 0,0, 256.0/cwidth, selectedVisionWarpCorner );        
        
        if( bSettingsMenu ) {
            settingsMenu.draw();
        }
        
    }
    
    
    virtual void _exit() {
        ofShowCursor();
    }
    
    
    
    // Key Events
    //
    //    
    virtual void _keyPressed( int key ) {
    
        if( bSetupMode ) {
            if( bSettingsMenu ) {
                settingsMenu.keyPressed( key );
            }
            
            if( key == 'c') {
                if( bLiveVideo && vidGrabber != NULL ) {
                    vidGrabber->videoSettings();
                }
            } else if( key == OF_KEY_RETURN ) {
                bSettingsMenu = true;
            } else if( key == ' ' ) {
                filter->bLearnBackground = true;
            }                      
        }

        if( key == 19 ) {  // control-s
            setSetupMode( !bSetupMode );
        }  else if( key == 6 ) {  // control-f
            ofToggleFullscreen();
        }
        
        keyPressed( key );  //call keyPressed in user app code
    }
    
    virtual void _keyReleased( int key ) {
        keyReleased( key );  //call keyReleased in user app code
    }



    // Mouse Events
    //
    //
    virtual void _mouseMoved( int x, int y ) {
        mouseX = x; mouseY = y;
        mouseMoved( x,y );
    }
    
    virtual void _mouseDragged( int x, int y, int button ) { 
        //graphicsWarp.mouseDragged( x,y, button );
        mouseDragged( x,y, button );
        
        if( bSimulate ) {
            mouseX = x; mouseY = y;
            simulationFinger.x = x;
            simulationFinger.y = y;
            simulationFinger.radius = 10;
            
            ofxTouchFinger& finger = getFinger(simulationFinger.id);
            finger.x = x;
            finger.y = y;
            
            fingerDragged( simulationFinger );
        }
    }
    
    virtual void _mousePressed( int x, int y, int button ) {
        //graphicsWarp.mousePressed( x,y, button );
        mousePressed( x,y, button );
        
        if( bSimulate ) {
            simulationFinger.id++;
            simulationFinger.initialOrder = 0;
            simulationFinger.x = x;
            simulationFinger.y = y;
            simulationFinger.radius = 10;
            fingers.push_back( simulationFinger );
            
            fingerPressed( simulationFinger );
        }        
    }
    
    virtual void _mouseReleased() {
        //graphicsWarp.mouseReleased();
        mouseReleased();
        
        if( bSimulate ) {
            simulationFinger.x = mouseX;
            simulationFinger.y = mouseY;
            simulationFinger.radius = 10;
            fingers.erase( fingers.begin()+fingerOrder(simulationFinger.id) );
            
            fingerReleased( simulationFinger );
        }        
    }



    // Touch Events
    //
    //
    virtual void blobMoved( const ofxTouchTrackedBlob& b ) {
        ofxTouchFinger& finger = getFinger(b.id);
        finger.set( b, xScaleToScreen, yScaleToScreen );
        fingerDragged( finger );    
    }   
    virtual void blobOn( const ofxTouchTrackedBlob& b ) {
        fingers.push_back( ofxTouchFinger(b) );
        ofxTouchFinger& finger = fingers.back();
        finger.initialOrder = fingerOrder(finger.id);
        finger.set( b, xScaleToScreen, yScaleToScreen );        
        fingerPressed( fingers.back() );
    }
    virtual void blobOff( const ofxTouchTrackedBlob& b ) {
        fingerReleased( getFinger(b.id) );
        fingers.erase( fingers.begin()+fingerOrder(b.id) );
    }
    
    // to be overwritten by subclass
    virtual void fingerDragged( const ofxTouchFinger& finger ) {}
    virtual void fingerPressed( const ofxTouchFinger& finger ) {}    
    virtual void fingerReleased( const ofxTouchFinger& finger ) {}
    
    
    
    // Various Methods
    //
    //    
    virtual void setSetupMode( bool _bSetupMode ) {
        bSetupMode = _bSetupMode;
        if( bSetupMode ) {
            ofShowCursor();
        } else {
            ofHideCursor();
        }
    }


    virtual void setSimulationMode( bool _bSimulate ) {
        bSimulate = _bSimulate;
    }


    virtual void setVideoPlayerMode( bool _bVideoPlayer ) {
        bLiveVideo = !_bVideoPlayer;
    }


    virtual void setVideoPlayerFile( string videofile ) {
        videoPlayerFile = videofile;
    }

                
    int fingerOrder( int id ) {
        // This is a bit inefficient but ok when
        // assuming low numbers of fingers
        // a better way would be to use a hash table
        int count = 0;
        for( int i=0; i<fingers.size(); i++ ) {
            if( fingers[i].id < id ) {
                count++;
            }
        }
        return count;
    }

    ofxTouchFinger&  getFinger( int id ) {
        // This is a bit inefficient but ok when
        // assuming low numbers of fingers
        // a better way would be to use a hash table
        for( int i=0; i<fingers.size(); i++ ) {
            if( fingers[i].id == id ) {
                return fingers[i];
            }
        }
        
    }   
    
    float ofRangemap(float r1min, float r1max, float r2min, float r2max, float r1val) {
      /**
      *                     r1val
      *    |- - - - -|--------x----|
      *    0       r1min       \  r1max
      *                         \
      *    |- - - - - -|---------x-------|
      *    0         r2min      return  r2max
      */
      r1val = CLAMP(r1val, r1min, r1max);
      return (r1val - r1min) * ((r2max - r2min)/(r1max - r1min)) + r2min;
    }
    
    

    
    
  protected:
  
    int camFps;
    int camFrameCount;
    int camFrameStartTime;
    int dspTime;
    int dspStartTime;
    
    bool bSetupMode;    
    bool bSimulate;    
    ofxTouchFinger simulationFinger;
    
    ofxTouchSettings  settingsMenu;
    
};

#endif
