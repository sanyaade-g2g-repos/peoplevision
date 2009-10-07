#ifndef _OF_FRAME_GRABBER
#define _OF_FRAME_GRABBER

#include "ofMain.h"
#include "ofxThread.h"

class FrameGrabber: public ofxThread{
	public:
		string fileName;
		int frameCounter;
		int maxFrames;
		bool close;
		ofImage img;
		
		int		x,y,w,h;

		float seconds, minutes;

		FrameGrabber(string _fileName, int _numFrames, bool _close, int _x, int _y, int _w, int _h);
		void grabFrame();
		
		void threadedFunction();
		void grabFrameThreaded();
};
#endif