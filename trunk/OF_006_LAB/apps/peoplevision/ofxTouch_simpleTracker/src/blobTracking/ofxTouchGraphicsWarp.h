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
* This class handles perspective warping of the entire OpenGL world.
*
*/

#ifndef OFX_TOUCH_GRAPHICSWARP
#define OFX_TOUCH_GRAPHICSWARP


#include "ofMain.h"


class ofxTouchGraphicsWarp {

  public:
  
    GLfloat warpMatrix[16];  
    ofPoint corners[4];
    int whichCorner;  


    ofxTouchGraphicsWarp() {
        for(int i = 0; i < 16; i++){
            if(i % 5 != 0) warpMatrix[i] = 0.0;
            else warpMatrix[i] = 1.0;
        }
            
        corners[0].x = 0.0;
        corners[0].y = 0.0;

        corners[1].x = 1.0;
        corners[1].y = 0.0;

        corners[2].x = 1.0;
        corners[2].y = 1.0;
        
        corners[3].x = 0.0;
        corners[3].y = 1.0;
    }

    
    void findMatrix() {
        //we set it to the default - 0 translation
        //and 1.0 scale for x y z and w
        for(int i = 0; i < 16; i++){
            if(i % 5 != 0) warpMatrix[i] = 0.0;
            else warpMatrix[i] = 1.0;
        }

        //we need our points as opencv points
        //be nice to do this without opencv?
        CvPoint2D32f cvsrc[4];
        CvPoint2D32f cvdst[4];	

        //we set the warp coordinates
        //source coordinates as the dimensions of our window
        cvsrc[0].x = 0;
        cvsrc[0].y = 0;
        cvsrc[1].x = ofGetWidth();
        cvsrc[1].y = 0;
        cvsrc[2].x = ofGetWidth();
        cvsrc[2].y = ofGetHeight();
        cvsrc[3].x = 0;
        cvsrc[3].y = ofGetHeight();			

        //corners are in 0.0 - 1.0 range
        //so we scale up so that they are at the window's scale
        for(int i = 0; i < 4; i++){
            cvdst[i].x = corners[i].x  * (float)ofGetWidth();
            cvdst[i].y = corners[i].y * (float) ofGetHeight();
        }
            
        //we create a matrix that will store the results
        //from openCV - this is a 3x3 2D matrix that is
        //row ordered
        CvMat * translate = cvCreateMat(3,3,CV_32FC1);
        
        //this is the slightly easier - but supposidly less
        //accurate warping method 
        //cvWarpPerspectiveQMatrix(cvsrc, cvdst, translate); 


        //for the more accurate method we need to create
        //a couple of matrixes that just act as containers
        //to store our points  - the nice thing with this 
        //method is you can give it more than four points!
        
        CvMat* src_mat = cvCreateMat( 4, 2, CV_32FC1 );
        CvMat* dst_mat = cvCreateMat( 4, 2, CV_32FC1 );

        //copy our points into the matrixes
        cvSetData( src_mat, cvsrc, sizeof(CvPoint2D32f));
        cvSetData( dst_mat, cvdst, sizeof(CvPoint2D32f));

        //figure out the warping!
        //warning - older versions of openCV had a bug
        //in this function.
        cvFindHomography(src_mat, dst_mat, translate);
        
        //get the matrix as a list of floats
        float *matrix = translate->data.fl;


        //we need to copy these values
        //from the 3x3 2D openCV matrix which is row ordered
        //
        // ie:   [0][1][2] x
        //       [3][4][5] y
        //       [6][7][8] w
        
        //to openGL's 4x4 3D column ordered matrix
        //        x  y  z  w   
        // ie:   [0][3][ ][6]
        //       [1][4][ ][7]
        //		 [ ][ ][ ][ ]
        //       [2][5][ ][9]
        //       

        warpMatrix[0]		= matrix[0];
        warpMatrix[4]		= matrix[1];
        warpMatrix[12]	= matrix[2];
        
        warpMatrix[1]		= matrix[3];
        warpMatrix[5]		= matrix[4];
        warpMatrix[13]	= matrix[5];	
        
        warpMatrix[3]		= matrix[6];
        warpMatrix[7]		= matrix[7];
        warpMatrix[15]	= matrix[8];    
    }
    
    
    void apply() {
        glMultMatrixf(warpMatrix);    
    }
    
    
    void draw( int corner ) {
        //lets draw a bounding box
        
        if( corner >= 0 ) {
            ofNoFill();
            ofSetColor( 255, 255, 255 );
            ofRect(1, 1, ofGetWidth()-2, ofGetHeight()-2);
        }
        
        if( corner == 0 ) {
            drawCorner( 1,1 );
        } else if( corner == 1) {
            drawCorner( ofGetWidth()-2, 1 );        
        } else if( corner == 2) {
            drawCorner( ofGetWidth()-2, ofGetHeight()-2 );                 
        } else if( corner == 3 ) {
            drawCorner( 1, ofGetHeight()-2 );                 
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
    
    
    void keyPressed(int key ) {}

    
    void mouseDragged( int x, int y, int button ) {
		float scaleX = (float)x / ofGetWidth();
		float scaleY = (float)y / ofGetHeight();
		
		if(whichCorner >= 0){
			corners[whichCorner].x = scaleX;
			corners[whichCorner].y = scaleY;			
		}
        
        findMatrix();
    }
    
    void mousePressed( int x, int y, int button ) {        
        float smallestDist = 1.0;
        whichCorner = -1;

        for(int i = 0; i < 4; i++){
            float distx = corners[i].x - (float)x/ofGetWidth();
            float disty = corners[i].y - (float)y/ofGetHeight();
            float dist  = sqrt( distx * distx + disty * disty);
            
            if(dist < smallestDist && dist < 0.1){
                whichCorner = i;
                smallestDist = dist;
            }
        }
    }
    
    void mouseReleased() {
        whichCorner = -1;
    }


};

#endif
