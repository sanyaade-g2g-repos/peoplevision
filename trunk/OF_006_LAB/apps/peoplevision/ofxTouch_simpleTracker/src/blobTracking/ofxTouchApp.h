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

/*****************************************************************

	TO USE OFX VIDEOGRABBER INSTEAD OF REGULAR OF VIDEOGRABBER:
	
	1) define variable USING_OFX_VIDEOGRABBER
	2) define a format (optional)

	i.e.
		
	#define USING_OFX_VIDEOGRABBER
	#define BLOB_TRACKER_VIDEO_FORMAT VID_FORMAT_YUV422
	#define BLOB_TRACKER_COLOR_FORMAT VID_FORMAT_RGB
 
	or
	
	#define USING_OFX_VIDEOGRABBER
	#define BLOB_TRACKER_VIDEO_FORMAT VID_FORMAT_GREYSCALE
	#define BLOB_TRACKER_COLOR_FORMAT VID_FORMAT_GREYSCALE
	 
*****************************************************************/

#ifdef USING_OFX_VIDEOGRABBER
	#include "ofxVideoGrabber.h"
	
	#ifndef BLOB_TRACKER_VIDEO_FORMAT
		#define BLOB_TRACKER_VIDEO_FORMAT VID_FORMAT_YUV422
	#endif

	#ifndef BLOB_TRACKER_COLOR_FORMAT
		#define BLOB_TRACKER_COLOR_FORMAT VID_FORMAT_RGB
	#endif
#else
	#include "ofVideoGrabber.h"
#endif

#include "ofxTouchContourFinder.h"
#include "ofxTouchBlobTracker.h"
#include "ofxTouchAdaptiveFilter.h"
#include "ofxTouchGraphicsWarp.h"
#include "ofxTouchVisionWarp.h"
#include "ofxTouchSettings.h"

#define OFX_TOUCH_MAXBLOBSIZE 400

class ofxTouchApp : public ofxTouchBlobListener {

  public:
  
    int cwidth;
    int cheight;
    float xScaleToScreen;
    float yScaleToScreen;
	int maxCountourSize;
    
    vector<ofxTouchFinger>  fingers;
    
    bool bLiveVideo;
    string videoPlayerFile;
	
#ifdef USING_OFX_VIDEOGRABBER
	ofxVideoGrabber*  vidGrabber;
#else
    ofVideoGrabber*  vidGrabber;
#endif
	
    ofVideoPlayer*  vidPlayer;

#if BLOB_TRACKER_COLOR_FORMAT == VID_FORMAT_GREYSCALE
    ofxCvGrayscaleImage  colorImg;
#else
    ofxCvColorImage  colorImg;
#endif
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
    
    int mouseX, mouseY;
    
	//LAB ADD: EVENTS FOR TOUCH
	
	ofEvent<ofxTouchFinger> fingerDragged;
	ofEvent<ofxTouchFinger> fingerPressed;
	ofEvent<ofxTouchFinger> fingerReleased;
	
    ofxTouchApp( int _cwidth=640, int _cheight=480 ) {
        cwidth = _cwidth;
        cheight = _cheight;
        ttfWriter.loadFont( "verdana.ttf", 9, true, false );
        ttfWriter.setLineHeight(16);
        vidGrabber = NULL;
        vidPlayer = NULL;
        filter = NULL;
		
		ofAddListener( ofEvents.setup, this, &ofxTouchApp::_setup);
		ofAddListener( ofEvents.update, this, &ofxTouchApp::_update);
		ofAddListener(ofEvents.keyPressed, this, &ofxTouchApp::_keyPressed);
		ofAddListener( ofEvents.mousePressed, this, &ofxTouchApp::_mousePressed);
		ofAddListener(ofEvents.mouseReleased, this, &ofxTouchApp::_mouseReleased);
		ofAddListener( ofEvents.mouseDragged, this, &ofxTouchApp::_mouseDragged);
		
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
        
    
    virtual void _setup( ofEventArgs &e ) {
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
		//ofSetLogLevel(OF_LOG_VERBOSE);
		
		//setup video grabber
#ifdef USING_OFX_VIDEOGRABBER
		
		vidGrabber = new ofxVideoGrabber();
		vidGrabber->setDeviceID(0);
		
		Libdc1394Grabber *sdk = new Libdc1394Grabber;
		//sdk->setFormat7(VID_FORMAT7_0);
		sdk->listDevices();
		sdk->setDiscardFrames(true);
		//sdk->set1394bMode(true);
		//sdk->setROI(0,0,320,200);
		//sdk->setDeviceID("b09d01008bc69e:0");
		
		//vidGrabber = new ofxVideoGrabber();
		ofxIIDCSettings *settings = new ofxIIDCSettings;
		settings->setXMLFilename("mySettingsFile.xml");
		
		vidGrabber->setVerbose(true);
		
		//bool result = vidGrabber->initGrabber( cwidth, cheight, BLOB_TRACKER_VIDEO_FORMAT, BLOB_TRACKER_COLOR_FORMAT, 30, true, sdk, settings );
		//bool result = vidGrabber->initGrabber( cwidth, cheight, VID_FORMAT_YUV422, VID_FORMAT_RGB, 30, true, sdk, settings );
		
		//bool result = vidGrabber->initGrabber( cwidth, cheight, VID_FORMAT_YUV422, VID_FORMAT_RGB, 30 );
		// or like this:
		bool result = vidGrabber->initGrabber( cwidth, cheight, BLOB_TRACKER_VIDEO_FORMAT, BLOB_TRACKER_COLOR_FORMAT, 60, true, sdk, settings );

		// or like this:
		//bool result = vidGrabber->initGrabber( cwidth, cheight, VID_FORMAT_YUV411, VID_FORMAT_RGB, 30, true, new Libdc1394Grabber, new ofxIIDCSettings);
		
		if(result) {
			ofLog(OF_LOG_VERBOSE,"Camera successfully initialized.");
		} else {
			ofLog(OF_LOG_FATAL_ERROR,"Camera failed to initialize.");
		}				
		vidGrabber->update();
#else
		
		vidGrabber = new ofVideoGrabber();
		vidGrabber->setVerbose(true);
		vidGrabber->initGrabber(cwidth,cheight);      
#endif
    }
    
    

    virtual void runComputerVision() {
        bool bNewFrame = false;
               
        if( bLiveVideo ) {			
		#ifdef USING_OFX_VIDEOGRABBER
			vidGrabber->update();
		#endif
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

        
        //if (bNewFrame){
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
    
   // }    
    
    virtual void _update( ofEventArgs &e ) {
        ofBackground(0,0,0);
        if( !bSimulate ) {
            // let the app repaint to begin with
            if( ofGetFrameNum() > 1 ) {
                runComputerVision();
            }
        }
    }
    
    
    
    //virtual void draw( ofEventArgs &e ) {
    virtual void draw() {
        graphicsWarp.apply();
            
        ofSetColor( 0xffffff );
        if( bSetupMode && !bSimulate ) {
            if( ttfWriter.bLoadedOk ) {
                afterAcquiringImg.draw( 0,0, 256,192  );
                afterWarpImg.draw( 256,0, 256,192 );
                filter->draw( 512, 0, 256,192 );            
                afterFilteringImg.draw( 768,0, 256,192 );
                blobTracker.draw( 768, 0, 256.0/cwidth );

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
        } else {
			vidGrabber->draw(ofGetWidth()-cwidth,0);
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
    virtual void _keyPressed( ofKeyEventArgs &e ) {
		
		int key = e.key;
		
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
#ifdef USING_OFX_VIDEOGRABBER
			else if (key == 's' || key == 'S'){
				vidGrabber->videoSettings();
			}
#endif
        }

        if( key == 19 ) {  // control-s
            setSetupMode( !bSetupMode );
        }  else if( key == 6 ) {  // control-f
            ofToggleFullscreen();
        }
    }
    
    virtual void _keyReleased( int key ) {
    }



    // Mouse Events
    //
    //
    virtual void _mouseMoved( ofMouseEventArgs &e ) {
		int x = e.x;
		int y = e.y;
		int button = e.button;
		
        mouseX = x; mouseY = y;
    }
    
    virtual void _mouseDragged( ofMouseEventArgs &e ) { 
		int x = e.x;
		int y = e.y;
		int button = e.button;
        //graphicsWarp.mouseDragged( x,y, button );
        
        if( bSimulate ) {
            mouseX = x; mouseY = y;
            simulationFinger.x = x;
            simulationFinger.y = y;
            simulationFinger.radius = 10;
            
            ofxTouchFinger& finger = getFinger(simulationFinger.id);
            finger.x = x;
            finger.y = y;
            
			ofNotifyEvent(fingerDragged,simulationFinger,this);
            //fingerDragged( simulationFinger );
        }
    }
    
    virtual void _mousePressed( ofMouseEventArgs &e ) {
		int x = e.x;
		int y = e.y;
		int button = e.button;
		
        //graphicsWarp.mousePressed( x,y, button );
        
        if( bSimulate ) {
            simulationFinger.id++;
            simulationFinger.initialOrder = 0;
            simulationFinger.x = x;
            simulationFinger.y = y;
            simulationFinger.radius = 10;
            fingers.push_back( simulationFinger );
            
			ofNotifyEvent(fingerPressed,simulationFinger,this);
			
            //fingerPressed( simulationFinger );
        }        
    }
    
    virtual void _mouseReleased( ofMouseEventArgs &e ) {
		int x = e.x;
		int y = e.y;
		int button = e.button;
		
        //graphicsWarp.mouseReleased();
        
        if( bSimulate ) {
            simulationFinger.x = mouseX;
            simulationFinger.y = mouseY;
            simulationFinger.radius = 10;
            fingers.erase( fingers.begin()+fingerOrder(simulationFinger.id) );
            
			ofNotifyEvent(fingerReleased,simulationFinger,this);
            //fingerReleased( simulationFinger );
        }        
    }



    // Touch Events
    //
    //
    virtual void blobMoved( const ofxTouchTrackedBlob& b ) {
        ofxTouchFinger& finger = getFinger(b.id);
        finger.set( b, xScaleToScreen, yScaleToScreen );
        
		ofNotifyEvent(fingerDragged,simulationFinger,this);
		//fingerDragged( finger );    
    }   
    virtual void blobOn( const ofxTouchTrackedBlob& b ) {
        fingers.push_back( ofxTouchFinger(b) );
        ofxTouchFinger& finger = fingers.back();
        finger.initialOrder = fingerOrder(finger.id);
        finger.set( b, xScaleToScreen, yScaleToScreen ); 
		ofNotifyEvent(fingerPressed,simulationFinger,this);       
        //fingerPressed( fingers.back() );
    }
    virtual void blobOff( const ofxTouchTrackedBlob& b ) {
		ofNotifyEvent(fingerReleased,simulationFinger,this);   
        //fingerReleased( getFinger(b.id) );
        fingers.erase( fingers.begin()+fingerOrder(b.id) );
    }
    
    // to be overwritten by subclass
    //virtual void fingerDragged( const ofxTouchFinger& finger ) {}
    //virtual void fingerPressed( const ofxTouchFinger& finger ) {}    
    //virtual void fingerReleased( const ofxTouchFinger& finger ) {}
    
    
    
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
