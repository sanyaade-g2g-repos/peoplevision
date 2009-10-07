/*
 *  of1394VideoGrabberBW.cpp
 *  openFrameworks
 *
 *  Created by Zach Gage on 8/8/08.
 *  Copyright 2008 stfj. All rights reserved.
 *
 */

#include "of1394VideoGrabberBW.h"


//--------------------------------------------------------------------
of1394VideoGrabberBW::of1394VideoGrabberBW()
{	
	// common
	bIsFrameNew				= false;
	bVerbose 				= true;
	bGrabberInited 			= false;
	bUseTexture				= true;
	bChooseDevice			= false;
	deviceID				= 0;
	width 					= 640;	// default setting
	height 					= 480;	// default setting

	//frameCt =0;
	//oldFrameCt=0;

	camera=NULL;
	
	failedToInit = true;
}


//--------------------------------------------------------------------
of1394VideoGrabberBW::~of1394VideoGrabberBW()
{
	if(!failedToInit) // if the init failed, then trying to do this stuff is a very bad idea
	{		
		stopThread();
		ofSleepMillis(200);
		dc1394_video_set_transmission(camera, DC1394_OFF);
		dc1394_capture_stop(camera);
		dc1394_camera_free(camera);
		dc1394_free (d);
	}
}

void of1394VideoGrabberBW::close(){
	if(!failedToInit) // if the init failed, then trying to do this stuff is a very bad idea
	{		
		stopThread();
		ofSleepMillis(200);
		dc1394_video_set_transmission(camera, DC1394_OFF);
		dc1394_capture_stop(camera);
		dc1394_camera_free(camera);
		dc1394_free (d);
	}
}


//--------------------------------------------------------------------
void of1394VideoGrabberBW::listDevices()
{
	//nothing to see here, move along.
}

//--------------------------------------------------------------------
void of1394VideoGrabberBW::setVerbose(bool bTalkToMe)
{
	bVerbose = bTalkToMe;
}


//--------------------------------------------------------------------
void of1394VideoGrabberBW::setDeviceID(int _deviceID)
{
	deviceID		= _deviceID;
	bChooseDevice	= true;
}

//---------------------------------------------------------------------------
unsigned char * of1394VideoGrabberBW::getPixels()
{
	if( lock() )
	{
		return pixels;
		//return image;
	}
	else
	{
		return NULL;
	}
}

//---------------------------------------------------------------------------
bool  of1394VideoGrabberBW::isFrameNew()
{
	bool temp=false;
	
		dc1394_avt_get_frame_info(camera, &frameCt);
		if(frameCt != oldFrameCt);
		{
		   oldFrameCt=frameCt;
			
		   temp= true;
		}
	
	   return temp;

}

//--------------------------------------------------------------------
void of1394VideoGrabberBW::grabFrame()
{	
	
}


//--------------------------------------------------------------------
void of1394VideoGrabberBW::videoSettings(void)
{

}


//--------------------------------------------------------------------
int of1394VideoGrabberBW ::initGrabber()
{
	// moved from constructor
	int i;
	
	d = dc1394_new ();
	
    err=dc1394_camera_enumerate (d, &list);
    if(err!=0 && bVerbose)("\n Failed to enumerate cameras\n ");
	
    if (list->num == 0) {
        if(err!=0 && bVerbose)printf("\n No cameras found\n ");
		// return 1;
    }
	
    camera = dc1394_camera_new (d, list->ids[0].guid);
    if (!camera) {
        if(err!=0 && bVerbose)printf("\n Failed to initialize camera with guid %PRIx64", list->ids[0].guid);
        //return 1;
    }
    dc1394_camera_free_list (list);
	
    if(err!=0 && bVerbose)printf("Using camera with GUID %PRIx64\n", camera->guid);
	
    /*-----------------------------------------------------------------------
     *  get the best video mode and highest framerate. This can be skipped
     *  if you already know which mode/framerate you want...
     *-----------------------------------------------------------------------*/
    // get video modes:
    err=dc1394_video_get_supported_modes(camera,&video_modes);
    if(err!=0 && bVerbose)printf("\n Can't get video modes\n ");
	
    // select highest res mode:
    for (i=video_modes.num-1;i>=0;i--) {
        //if (!dc1394_is_video_mode_scalable(video_modes.modes[i])) {
		dc1394_get_color_coding_from_video_mode(camera,video_modes.modes[i], &coding);
		if (coding==DC1394_COLOR_CODING_MONO8) {
			video_mode=video_modes.modes[i];
			break;
		}
        //}
    }
    if (i < 0) {
        if(err!=0 && bVerbose)printf("\n Could not get a valid MONO8 mode\n ");
        //cleanup_and_exit(camera);
    }
	
    err=dc1394_get_color_coding_from_video_mode(camera, video_mode,&coding);
    if(err!=0 && bVerbose)printf("\n Could not get color coding\n ");
	
    // get highest framerate
    //err=dc1394_video_get_supported_framerates(camera,video_mode,&framerates);
    //if(err!=0 && bVerbose)printf("\n Could not get framrates\n ");
    //framerate=DC1394_FRAMERATE_15;//framerates.framerates[framerates.num-1];
	
    /*-----------------------------------------------------------------------
     *  setup capture
     *-----------------------------------------------------------------------*/
	
	
	
	video_mode=DC1394_VIDEO_MODE_FORMAT7_1;
	
    err=dc1394_video_set_iso_speed(camera, DC1394_ISO_SPEED_400);
    //if(err!=0 && bVerbose)printf("\n Could not set iso speed\n ");
	
	unsigned int maxH, maxV;
	
	dc1394_format7_get_max_image_size(camera, video_mode, &maxH, &maxV);
	
	//printf("\n image size: maxH: %i   maxV: %i \n",maxH,maxV);
	
    err=dc1394_video_set_mode(camera, video_mode);
	//dc1394_format7_set_roi(camera, video_mode, coding, 10,0,0,maxH,maxV);
    
	
	//dc1394_format7_set_packet_size(camera, video_mode, 10);
	
	
	//if(err!=0 && bVerbose)printf("\n Could not set video mode\n ");
	
	
	
	
	
    //err=dc1394_video_set_framerate(camera, framerate);
    //if(err!=0 && bVerbose)printf("\n Could not set framerate\n ");
	err=dc1394_memory_load(camera, 1);
	//cout<<"err"<<err<<endl;
    err=dc1394_capture_setup(camera,4, DC1394_CAPTURE_FLAGS_DEFAULT);
	//err=dc1394_capture_setup(camera,4, DC1394_CAPTURE_FLAGS_CHANNEL_ALLOC);
    //if(err!=0 && bVerbose)printf("\n Could not setup camera-\nmake sure that the video mode and framerate are\nsupported by your camera\n ");
	
    /*-----------------------------------------------------------------------
     *  report camera's features
     *-----------------------------------------------------------------------*/
    err=dc1394_feature_get_all(camera,&features);
    if (err!=DC1394_SUCCESS) {
        dc1394_log_warning("Could not get feature set\n ");
    }
    else {
        //dc1394_feature_print_all(&features, stdout);
    }
	
    /*-----------------------------------------------------------------------
     *  have the camera start sending us data
     *-----------------------------------------------------------------------*/
    err=dc1394_video_set_transmission(camera, DC1394_ON);
   	//dc1394_format7_set_image_size(camera,video_mode,768,492);
	
	
	dc1394_get_image_size_from_video_mode(camera, video_mode, &width, &height);
	
	unsigned int min,max;
	dc1394_format7_get_packet_parameters(camera,video_mode,&min,&max);
	
	//dc1394_format7_get_color_filter
	
	//printf("\n packets: min: %i   max: %i \n",min,max);
	//
	
	
	pixels	= new unsigned char[width*height*3];
	image	= new unsigned char[width*height*3];
	
	tex.allocate(width/3,height/3,GL_RGB);
	
	dc1394_capture_dequeue( camera, DC1394_CAPTURE_POLICY_WAIT, &frame ) ;
	
	memcpy( pixels, frame->image, width * height * 3 ) ;
	
	dc1394_capture_enqueue( camera, frame ) ;
	
	ofSleepMillis(100);
	startThread(true, false);
	
	failedToInit = false;
}

//--------------------------------------------------------------------
int of1394VideoGrabberBW::initGrabber(int _width, int _height){
	width = _width;
	height = _height;
	initGrabber();
}

/*-----------------------------------------------------------------------
 *  Prints the type of format to standard out
 *-----------------------------------------------------------------------*/
void of1394VideoGrabberBW::print_format( uint32_t format )
{
#define print_case(A) case A: printf(#A ""); break;
	
    switch( format ) {
			print_case(DC1394_VIDEO_MODE_160x120_YUV444);
			print_case(DC1394_VIDEO_MODE_320x240_YUV422);
			print_case(DC1394_VIDEO_MODE_640x480_YUV411);
			print_case(DC1394_VIDEO_MODE_640x480_YUV422);
			print_case(DC1394_VIDEO_MODE_640x480_RGB8);
			print_case(DC1394_VIDEO_MODE_640x480_MONO8);
			print_case(DC1394_VIDEO_MODE_640x480_MONO16);
			print_case(DC1394_VIDEO_MODE_800x600_YUV422);
			print_case(DC1394_VIDEO_MODE_800x600_RGB8);
			print_case(DC1394_VIDEO_MODE_800x600_MONO8);
			print_case(DC1394_VIDEO_MODE_1024x768_YUV422);
			print_case(DC1394_VIDEO_MODE_1024x768_RGB8);
			print_case(DC1394_VIDEO_MODE_1024x768_MONO8);
			print_case(DC1394_VIDEO_MODE_800x600_MONO16);
			print_case(DC1394_VIDEO_MODE_1024x768_MONO16);
			print_case(DC1394_VIDEO_MODE_1280x960_YUV422);
			print_case(DC1394_VIDEO_MODE_1280x960_RGB8);
			print_case(DC1394_VIDEO_MODE_1280x960_MONO8);
			print_case(DC1394_VIDEO_MODE_1600x1200_YUV422);
			print_case(DC1394_VIDEO_MODE_1600x1200_RGB8);
			print_case(DC1394_VIDEO_MODE_1600x1200_MONO8);
			print_case(DC1394_VIDEO_MODE_1280x960_MONO16);
			print_case(DC1394_VIDEO_MODE_1600x1200_MONO16);
			
		default:
			dc1394_log_error("Unknown format\n");
			//exit(1);
    }
	
}

/*-----------------------------------------------------------------------
 *  Returns the number of pixels in the image based upon the format
 *-----------------------------------------------------------------------*/
uint32_t of1394VideoGrabberBW::get_num_pixels(uint32_t format ) {
    uint32_t w,h;
	
    dc1394_get_image_size_from_video_mode(camera, format,&w,&h);
	
    return w*h;
}

/*-----------------------------------------------------------------------
 *  Prints the type of color encoding
 *-----------------------------------------------------------------------*/
void of1394VideoGrabberBW::print_color_coding( uint32_t color_id )
{
    switch( color_id ) {
		case DC1394_COLOR_CODING_MONO8:
			printf("MONO8");
			break;
		case DC1394_COLOR_CODING_YUV411:
			printf("YUV411");
			break;
		case DC1394_COLOR_CODING_YUV422:
			printf("YUV422");
			break;
		case DC1394_COLOR_CODING_YUV444:
			printf("YUV444");
			break;
		case DC1394_COLOR_CODING_RGB8:
			printf("RGB8");
			break;
		case DC1394_COLOR_CODING_MONO16:
			printf("MONO16");
			break;
		case DC1394_COLOR_CODING_RGB16:
			printf("RGB16");
			break;
		case DC1394_COLOR_CODING_MONO16S:
			printf("MONO16S");
			break;
		case DC1394_COLOR_CODING_RGB16S:
			printf("RGB16S");
			break;
		case DC1394_COLOR_CODING_RAW8:
			printf("RAW8");
			break;
		case DC1394_COLOR_CODING_RAW16:
			printf("RAW16");
			break;
		default:
			dc1394_log_error("Unknown color coding = %d\n",color_id);
			exit(1);
    }
}

/*-----------------------------------------------------------------------
 *  Prints various information about the mode the camera is in
 *-----------------------------------------------------------------------*/
void of1394VideoGrabberBW::print_mode_info(dc1394video_mode_t mode )
{
    int j;
	
    printf("Mode: ");
    print_format(mode);
    printf("\n");
	
    dc1394framerates_t framerates;
    dc1394error_t err;
    err=dc1394_video_get_supported_framerates(camera,mode,&framerates);
    DC1394_ERR(err,"Could not get frame rates");
	
    printf("Frame Rates:\n");
    for( j = 0; j < framerates.num; j++ ) {
        dc1394framerate_t rate = framerates.framerates[j];
        float f_rate;
        dc1394_framerate_as_float(rate,&f_rate);
        printf("  [%d] rate = %f\n",j,f_rate );
    }
	
}
