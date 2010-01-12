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
* This class handles a perspective warp on an image. It uses
* a nifty way to precalulate the transformation and store it
* in the form of mapping images. The cool thing about this is
* that additional transformation can be accumulated without
* any performace penalty when applying the transformations.
*
*/

#ifndef OFX_TOUCH_VISIONWARP
#define OFX_TOUCH_VISIONWARP

#include "ofMain.h"
#include "ofxCvMain.h"



class ofxTouchVisionWarp {
  public:
    
    int cwidth;
    int cheight;
    ofPoint corners[4];
    

    ofxTouchVisionWarp() {
        cwidth = 320;
        cheight = 240;
        translate = cvCreateMat( 3,3, CV_32FC1 );
    }    

    virtual ~ofxTouchVisionWarp() {
        cvReleaseMat( &translate );
        cvReleaseImage( &_mapX );
        cvReleaseImage( &_mapY );
        cvReleaseImage( &mapX );
        cvReleaseImage( &mapY );
    }
    
    
    void allocate( int _cwidth, int _cheight ) {
        cwidth = _cwidth;
        cheight = _cheight;
        
        CvSize csize = cvSize( cwidth, cheight );
        _mapX = cvCreateImage( csize, IPL_DEPTH_32F, 1 );
        _mapY = cvCreateImage( csize, IPL_DEPTH_32F, 1 );
        mapX = cvCreateImage( csize, IPL_DEPTH_32F, 1 );
        mapY = cvCreateImage( csize, IPL_DEPTH_32F, 1 );
        
        
        //assign pixel value according to location
        //
        for( int i=0;i<_mapX->height;i++ ) {
            for( int j=0;j<_mapX->width;j++ ) {
                ((float *)(_mapX->imageData + i*_mapX->widthStep))[j] = (float)j;
                //(float)_mapX[i][j]= (float)j;
            }
        }
        for( int i=0;i<_mapY->height;i++ ) {
            for( int j=0;j<_mapY->width;j++ ) {
                ((float *)(_mapY->imageData + i*_mapY->widthStep))[j] = (float)i;
            }
        }
        
    }
    
    void findMatrix() {
        cvSetZero( translate );

        cvdst[0].x = 0;
        cvdst[0].y = 0;
        cvdst[1].x = cwidth;
        cvdst[1].y = 0;
        cvdst[2].x = cwidth;
        cvdst[2].y = cheight;
        cvdst[3].x = 0;
        cvdst[3].y = cheight;

        cvsrc[0].x = corners[0].x;
        cvsrc[0].y = corners[0].y;
        cvsrc[1].x = corners[1].x;
        cvsrc[1].y = corners[1].y;
        cvsrc[2].x = corners[2].x;
        cvsrc[2].y = corners[2].y;
        cvsrc[3].x = corners[3].x;
        cvsrc[3].y = corners[3].y;
        
        cvWarpPerspectiveQMatrix( cvsrc, cvdst, translate );
        
        cvWarpPerspective( _mapX, mapX, translate );
        cvWarpPerspective( _mapY, mapY, translate );
    }
    
    
    void apply( ofxCvGrayscaleImage& img ) {
        // use precalculated displacement maps
        img.remap( mapX, mapY );
    }

    void draw( int x, int y, float scale,  int corner ) {
        if( corner >= 0 ) {
            ofNoFill();
            ofSetColor( 255, 255, 255 );
            
            glPushMatrix();
            glScalef( scale, scale, 1.0 );
            ofLine( x+corners[0].x, y+corners[0].y, x+corners[1].x, y+corners[1].y );
            ofLine( x+corners[1].x, y+corners[1].y, x+corners[2].x, y+corners[2].y );
            ofLine( x+corners[2].x, y+corners[2].y, x+corners[3].x, y+corners[3].y );
            ofLine( x+corners[3].x, y+corners[3].y, x+corners[0].x, y+corners[0].y );
            
            if( corner < 4 ) {
                drawCorner( x+corners[corner].x, y+corners[corner].y );
            }
            glPopMatrix();
        }
        
        ofSetColor(0xffffff);   
    }
    
    
    void drawCorner( int x, int y) {
        static float brightness = 0;
        ofFill();
        ofRect(x-4, y-4, 8, 8);
        //int light = 55 + brightness % 200;
        int light = 155 + 100*sin(brightness);
        ofSetColor( light, light, light, light );
        ofTriangle( x-24, y, x-16, y+8, x-16, y-8);
        ofTriangle( x+24, y, x+16, y+8, x+16, y-8);
        ofTriangle( x, y+24, x+8, y+16, x-8, y+16);
        ofTriangle( x, y-24, x+8, y-16, x-8, y-16);
        brightness += 0.1;
    } 
    
    
  private:
    CvPoint2D32f cvsrc[4];
    CvPoint2D32f cvdst[4];
    CvMat* translate;
    
    IplImage*  _mapX;
    IplImage*  _mapY; 
    IplImage*  mapX;
    IplImage*  mapY;    
    
};


#endif

