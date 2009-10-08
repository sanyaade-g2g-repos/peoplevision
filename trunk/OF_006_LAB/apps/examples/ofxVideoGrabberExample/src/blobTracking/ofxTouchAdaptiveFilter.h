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
* The default filter.
*/

#ifndef OFX_TOUCHADAPTIVEFILTER
#define OFX_TOUCHADAPTIVEFILTER

#include "ofxTouchFilter.h"


class ofxTouchAdaptiveFilter : public ofxTouchFilter {
  public:
  
    ofxCvShortImage  floatBgImg;    
    ofxCvGrayscaleImage  grayBgImg;    

    
    void allocate( int w, int h ) {
        cwidth = w;
        cheight = h;
        threshold = 60;
        bLearnBackground = true;
        floatBgImg.allocate( cwidth,cheight );

        grayBgImg.allocate( cwidth,cheight );
    }
        
    
    void apply( ofxCvGrayscaleImage& img ) {
        
        if( bLearnBackground ) {
			floatBgImg = img;
			bLearnBackground = false;
        } else {
            floatBgImg.addWeighted( img, 0.001);
        }

       // grayBgImg = floatBgImg;  // this is buggy in OF v0.05
        //cvConvert( floatBgImg.getCvImage(), grayBgImg.getCvImage() );
        cvConvertScale( floatBgImg.getCvImage(),
					   grayBgImg.getCvImage(),
					   255.0f/65535.0f, 0 );       
        grayBgImg.flagImageChanged();
		
        //img.absDiff( grayBgImg );  //we actually don't want darker spots
        cvSub( img.getCvImage(), grayBgImg.getCvImage(), img.getCvImage() );
        
        //img.blur( 11 );
        img.threshold( threshold );
    }
    
    
    void draw( int x, int y ) {
        grayBgImg.draw( x,y );
    }

    void draw( int x, int y, int w, int h ) {
        grayBgImg.draw( x,y, w,h );
    }    
  
};


#endif
