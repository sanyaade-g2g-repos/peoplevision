/*
 *  of1394VideoGrabber.h
 *  openFrameworks
 *
 *  Created by Zach Gage on 8/8/08.
 *  Copyright 2008 stfj. All rights reserved.
 *
 */
#ifndef _OF_1394_VIDEO_GRABBER
#define _OF_1394_VIDEO_GRABBER

#include "ofMain.h"
#include "ofConstants.h"
#include "ofTexture.h"
#include "ofGraphics.h"
#include <iostream>

#include <dc1394/dc1394.h>
//#include <dc1394/format7.h>
#include <dc1394/vendor/avt.h>

#include "ofxThread.h"

class of1394VideoGrabber : public ofxThread{
	
	public :
	
	of1394VideoGrabber();
	~of1394VideoGrabber();
	
	void 			listDevices();
	bool 			isFrameNew();
	void			grabFrame();
	void 			initGrabber();
//	void			initGrabber(int w, int h, bool bTexture);
	void			videoSettings();
	unsigned char 	* getPixels();
	void 			setVerbose(bool bTalkToMe);
	void			setDeviceID(int _deviceID);
	void 			setUseTexture(bool bUse);
	void 			draw(float x, float y, float w, float h);
	void 			draw(float x, float y);
	//int 			width, height;
	ofTexture 				tex;
	
	bool dying;
	
	dc1394camera_t *camera;
    
    dc1394featureset_t features;
    dc1394framerates_t framerates;
    dc1394video_modes_t video_modes;
    dc1394framerate_t framerate;
    dc1394video_mode_t video_mode;
    dc1394color_coding_t coding;
    unsigned int width, height;
	
	unsigned int frameCt, oldFrameCt;
	
    dc1394video_frame_t *frame;
	//dc1394video_frame_t *deBayeredFrame;
    dc1394_t * d;
    dc1394camera_list_t * list;
	
	dc1394error_t err;
	
	void threadedFunction(){
		while( isThreadRunning())
		{
			if(lock())
			{
				try
				{
					if(isFrameNew())
					{
						err = dc1394_capture_dequeue( camera, DC1394_CAPTURE_POLICY_WAIT, &frame ) ;
						
						//cout<<err<<endl;
						//dc1394_deinterlace_stereo(frame,frame,width,height);
							
							//dc1394_debayer_frames(frame, frame, DC1394_BAYER_METHOD_SIMPLE);
						
						
							
						memcpy( pixels, frame->image, width * height * 3 ) ;
						
						dc1394_capture_enqueue( camera, frame ) ;
						
						//cout<<pixels[0]<<" "<<pixels[1]<<" "<<pixels[2]<<endl;
						//-----
						//vidGrabber.unlock();
						//pixels = temp;
						
						
						int totalPixels = width*height*3;
						
						bool oddLine=false;
						int lineNum=0;
						int pxlNum=0;
						int offsetAdd = width*2;
						
						int sinceLastRow=0;
						
						for (int i = 0; i < totalPixels/3; i++)
						{
							
							if(sinceLastRow==0 && i!=0) //if at the start and not the real start
							{
								oddLine = !oddLine; // alternate odd lines
								if(!oddLine)
								{
									lineNum++; //if it's not an odd line, increase line number (double the lines)
								}
							}
							
							if(!oddLine) //if its not an odd line, draw it normally
							{
								image[i] = pixels[lineNum*width*3+sinceLastRow]; //offset plus the ( line number * total image width ) plus the real position
							}
							else
							{
								//if it is an odd line:
								//offset plus the ( line number * total image width ) plus the real position;  //draw the same pixel as last row
								image[i] = (pixels[lineNum*width*3+sinceLastRow] + pixels[lineNum*width*3+sinceLastRow+width*3])/2;
							}
							
							sinceLastRow++;
							if(sinceLastRow==width) //if at the end of a 3rd of the image,
							{
								sinceLastRow=0; // set the last row back to zero.
							}
						}
						
						//videoTexture.loadData(videoInverted, width/3, width/3, GL_RGB);
						//-----
						
					}
				}
				catch(int errrr){}
				unlock();
				
				ofSleepMillis(20);
			}
			
		}
	}
	
protected:
	
	bool					bChooseDevice;
	int						deviceID;
	bool					bUseTexture;
	bool 					bVerbose;
	bool 					bGrabberInited;
	unsigned char * 		pixels;
	unsigned char *			image;
	bool 					bIsFrameNew;
	
	
};




#endif

