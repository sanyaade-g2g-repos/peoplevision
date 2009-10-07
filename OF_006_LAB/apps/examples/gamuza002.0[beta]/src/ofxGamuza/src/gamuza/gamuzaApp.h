#ifndef _GAMUZA_APP
#define _GAMUZA_APP

#include "ofMain.h"

// official addons
#include "ofxVectorMath.h"			// vectorial math structures library
#include "ofxOpenCv.h"				// computer vision library
#include "ofxOsc.h"					// Open Sound Control communication library
#include "ofxXmlSettings.h"			// read/write on/to xml file library
#include "ofxDirList.h"				// directory listing util library
#include "ofxThread.h"				// thread library

// experimental addons
#include "ofxDsp.h"					// DSP signal vector library [from Stefan Kirch]
#include "ofxGui.h"					// graphical user interface library [from Stefan Kirch]
#include "ofxMidi.h"                // midi library [from Arturo & Chris O'Shea]
#include "ofxPachube.h"             // dowloading/uploading pachube feeds [from Carl Johan Rosen]
#include "ofxSniffer.h"             // Carnivore-style sniffer using libpcap library [from Emanuele Mazza]
#include "ofxTapTools.h"			// TapTools Audio Signal Processing wrapper for OF [from Emanuele Mazza]
#include "ofxWordnet.h"				// Simple access to the Wordnet [http://wordnet.princeton.edu/] ontology wrapper for OF [from Emanuele Mazza]
#include "ofxXmlExtended.h"			// capture xml from the net
#include "SimpleWebScraper.h"

///////////////////////////////////////// experimental code

// audioAnalysis
#include "fft.h"					// microphone input FFT
// imageAnalysis
#include "ofxCvOpticalFlowLK.h"     // cvOpticalFlowLK wrapper for OF [from Takashi Maekawa]
#include "ofxCvOpticalFlowBM.h"     // cvOpticalFlowBM wrapper for OF [from Takashi Maekawa]
#include "ofxCvHaarFinder.h"		// face detection [by Charlie & Stefanix]
#include "ofRecursiveCentroids.h"	// Recursive centroid analysis [from Zachary Lieberman & Golan levin]
#include "ofContourAnalysis.h"		// contour geometry analysis [from Chris Sugrue]
#include "contourSimplify.h"		// contour simplify [from Theodore Watson]
#include "fingerDetector.h"			// finger/hand detector [from Dani Quilez]
// FBO texture
#include "ofTextureAdv.h"			// FBO texture manager [from Zach Gage]
// graphicsUtils
#include "laserTracking.h"			// hsv color viewer utils [from G.R.L.]
// imageFilters
#include "ImageFilters.h"			// Fast Blur util [by Mario Klingemann]
// frameGrabber
#include "FrameGrabber.h"			// grab frame class
// randomUtils
#include "randomUtils.h"			// [by Emanuele Mazza]
// statistics
#include "statsRecorder.h"			// [by Chris sugrue]
// videoEffects
#include "videoDelayEffect.h"		// [by Damian Stewart]
// advanced math
#include "adaptiveSimpson.h"		// compute integral using Adaptive Simpson Method [by Emanuele Mazza]
// physics/Fluid
#include "FluidTexture.h"			// [by Memo Akten]
// physics/Flocking
#include "xParticle.h"				// [by Chris Sugrue]
#include "circles.h"				// [by Chris Sugrue]

#include "gamuzaConstants.h"		// constants

class gamuzaApp : public ofBaseApp, public ofxGuiListener{
	enum
	{
		kParameter_Panel1,				// ID 0
		kParameter_Panel2,				// ID 1
		kParameter_Panel3,				// ID 2
		kParameter_Panel4,				// ID 3

		kParameter_Hue,					// ID 4
		kParameter_HueWidth,			// ID 5
		kParameter_Sat,					// ID 6
		kParameter_SatWidth,			// ID 7
		kParameter_Val,					// ID 8
		kParameter_ValWidth,			// ID 9
		kParameter_Threshold,			// ID 10
		kParameter_MotionThreshold,		// ID 11
		kParameter_MNoiseComp,			// ID 12
		kParameter_Blur1,				// ID 13
		kParameter_Blur2,				// ID 14
		kParameter_HSVBlur,				// ID 15
		kParameter_Balance1,			// ID 16
		kParameter_Balance2,			// ID 17
		kParameter_DecreaseRate,		// ID 18

		kParameter_Point1,				// ID 19
		kParameter_Point2,				// ID 20
		kParameter_Point3,				// ID 21
		kParameter_Point4,				// ID 22
		kParameter_CPoint1,				// ID 23
		kParameter_CPoint2,				// ID 24
		kParameter_CPoint3,				// ID 25
		kParameter_CPoint4,				// ID 26

		kParameter_SaveXml,				// ID 27
		kParameter_File,				// ID 28

		kParameter_Panel5,				// ID 29
		kParameter_ContoursDetail,		// ID 30
		kParameter_ShowEllipse,			// ID 31
		kParameter_ShowAngle,			// ID 32
		kParameter_ShowLines,			// ID 33

		kParameter_Panel6,				// ID 34
		kParameter_computeMH,			// ID 35
		kParameter_computeCF,			// ID 36
		kParameter_computeRC,			// ID 37
		kParameter_computeOF,			// ID 38
		kParameter_computeFT,			// ID 39
		kParameter_computeFHT,			// ID 40
		kParameter_lightGui,			// ID 41

		kParameter_Panel7,				// ID 42
		kParameter_SendMD,				// ID 43
		kParameter_SendCF,				// ID 44
		kParameter_SendRC,				// ID 45
		kParameter_SendOF,				// ID 46
		kParameter_SendFT,				// ID 47
		kParameter_SendFHT,				// ID 48

		kParameter_Panel8,				// ID 49
		kParameter_BGSubT,				// ID 50

		kParameter_Panel9,				// ID 51
		kParameter_Empty,				// ID 52

		kParameter_RGBhSV				// ID 53

};

	public:

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		void calculateColorDifference();									// background subtraction using rgb image
		void calculateGrayscaleDifference();								// background subtraction using grayscale image
		void calculateColorDifferenceHSV();									// tracking color difference over time using HSV color information
		void balanceTracking();												// filter information from the two different algorithms
		void calculateMotion();												// motion detection
		void generateHistory(unsigned char * input, float decreaseRate);	// image history
		void computeContourAnalysis();										// compute contour analysis
		void drawContourAnalysis();											// draw contour analysis
		void drawIntoFBO();													// draw final projection

		// optical flow utils function
		void getVelAtPixel(int x, int y, float *u, float *v);               // get optical_flow pixel velocity
		void getVelAtNorm(float x, float y, float *u, float *v);            // get optical_flow normal velocity

		// utils functions
		void getXmlInfo();													// get some xml info to update warping point GUI positions
		void smoothingValues();												// smoothing numerical data
		void normalizeValues();												// normalize numerical data

		// OSC function
		void getIPfromXML();
		void sendOscData();

		// GUI function
		void handleGui(int parameterId, int task, void* data, int length);


        ofVideoGrabber 		    vidGrabber;

        ofxCvColorImage			colorImg;			// Live cam input image
		ofxCvColorImage			colorImgWarped;		// warping input for adjust tracking area
		ofxCvColorImage			colorImgHSV;		// HSV image
		ofxCvColorImage			colorBg;			// color background static image
		ofImage					savedBG;			// saving color background on hard drive
		ofImage					loadBG;				// load color background from hard drive

		ofxCvGrayscaleImage		grayImg;			// B&W Live cam input image
		ofxCvGrayscaleImage		grayBg;				// gray background static image
		ofxCvGrayscaleImage		grayThresh;			// background subtraction over b&w input image
		ofImage					savedGrayBG;		// saving gray background on hard drive
		ofImage					loadGrayBG;			// load gray background from hard drive

		ofxCvGrayscaleImage		hueImg;				// Hue image
        ofxCvGrayscaleImage		satImg;				// Saturation image
        ofxCvGrayscaleImage		valImg;				// Value image
		unsigned char			*huePixels;			// hue pixel info
		unsigned char			*satPixels;			// sat pixel info
		unsigned char			*valPixels;			// val pixel info
		unsigned char			*hsvTrackedPixels;	// HSV tracked image
		ofxCvGrayscaleImage		hsvTracking;		// hsv tracking result image

		unsigned char			*pixels1;
		unsigned char			*pixels2;
		unsigned char			*resPixels;
		ofxCvGrayscaleImage 	colorDiff;			// background subtraction over RGB input image

		// balanced tracking
		ofxCvGrayscaleImage		balancedTracking;	// balanced tracking from back subtraction over color & HSV tracking together
		unsigned char			*alg1;
		unsigned char			*alg2;
		unsigned char			*balPixels;

		// motion detection
		ofxCvGrayscaleImage 	grayPrev;
		ofxCvGrayscaleImage 	grayNow;
		ofxCvGrayscaleImage 	motionImg;
		unsigned char			*motion;
		int						frameCounter;
		int						numPixelsChanged;	// motion quantity
		int						mCX, mCY, mCX2, mCY2;
		float					meanX, meanY;

		// history
		ofxCvGrayscaleImage		historyImg;
		float					*mhiImageFloat;
		unsigned char			*mhiImageByte;

		// contour finder
        ofxCvContourFinder		contourFinder;
		contourSimplify			contourS;
		vector<ofxPoint2f>		*contourReg;
		vector<ofxPoint2f>		*contourSmooth;
		vector<ofxPoint2f>		*contourSimple;
		vector<ofxPoint2f>		*contourHull;
		CvBox2D32f				*box;
		float					*blobAngle;
		float					smoothPct;
		float					tolerance;
		int						runningBlobs;

		// recursive centroid analysis
		ofRecursiveCentroids	*recursiveCentroid;
		int						depth;				// recursive controid depth
		int						centroidNum;

		// optical flow analysis
		ofxCvOpticalFlowLK		opticalFlow;

		// face tracking
		ofxCvHaarFinder			haarFinder;
		int						numFace;

		// finger/hand tracking
		fingerDetector			fingerFinder;
		vector<ofxPoint2f>		fingerPos;			// fingers positions
		bool					fFound, hFound;

		// final texture
		ofTextureAdv			finalTexture;

		// GUI
		ofxGui					*gui;
		ofxXmlSettings			guiXml;
		ofImage					backgroundGraphics;
		ofTrueTypeFont			automatFont;
		laserTracking			hsvViewer;

		// OSC vars
		ofxXmlSettings			host_data;
		string					host_number;
		string					host_port;

		// smoothed vars
		float					_s_MDCX1, _s_MDCX2, _s_MDCY1, _s_MDCY2;			// smoothed motion detection centers mass
		float					_s_MDQ;											// smoothed motion quantity
		vector<ofPoint>			_s_centPos;										// smoothed centroid positions
		CvBox2D32f				*_s_blobInfo;									// smoothed blobs info
		vector<ofxPoint4f>      _s_blobGeom;									// smoothed blobs geometry
		ofxPoint2f				*_s_opfVel;										// smoothed optical flow pixel velocity
		vector<ofxPoint4f>		_s_ftInfo;										// smoothed face tracking blobs info
		vector<ofxPoint2f>		_s_fingerPos;									// smoothed fingers positions

		// OSC sending vars
		float					_osc_MDCX1, _osc_MDCX2, _osc_MDCY1, _osc_MDCY2; // normalized motion detection centers mass
		float					_osc_MDQ;										// normalized motion quantity
		vector<ofPoint>			_osc_centPos;									// normalized centroid positions
		CvBox2D32f				*_osc_blobInfo;									// normalized blobs info
		vector<ofxPoint4f>      _osc_blobGeom;									// normalized blobs geometry
		ofxPoint2f				*_osc_opfVel;									// normalized optical flow pixel velocity
		vector<ofxPoint4f>		_osc_ftInfo;									// normalized face tracking blobs info
		vector<ofxPoint2f>		_osc_fingerPos;									// normalized fingers positions

		/////////////////////////////
		// GUI control variables

		float					hue, hueWidth, sat, satWidth, val, valWidth;	// HSV control vars
		float					threshold;										// threshold for background subtraction
		float					mThreshold, mNoiseComp;							// threshold for motion detection
		float					dRate;											// history image decrease rate
		float					blur1, blur2, HsvBlur;							// blur quantity for background subtraction & HSV tracking
		float					balance1, balance2;								// balance between the 2 source algorihms
		int						bgSubTech;										// background subtraction technique selector
		int						cfDetail;										// contour detection detail selector
		int						rgbORhsv;                                       // 0 RGB, 1 HSV
		ofxPoint2f				*sourcePoints;									// input image warping points
		ofxPoint2f				*destPoints;									// destination image warping points
		ofxPoint2f				*inputPoints;									// final texture start points
		ofxPoint2f				*cropPoints;									// final texture crop points
		vector<ofxPoint4f>      geomLines;										// geometry lines find into countour

		bool					lockWarpCrop;
		bool					saveXML;
		bool					showGui;
		bool					lightGui;
        bool					bShowEllipse;
        bool					bShowAngle;
		bool					bShowLines;
		bool					computeMotionHistory;
		bool					computeContourFinder;
		bool					computeRecursiveCentroid;
		bool					computeOpticalFlow;
		bool					computeFaceTracking;
		bool					computeFingerTracking;

		bool					sendOsc_MD;
		bool					sendOsc_CF;
		bool					sendOsc_RC;
		bool					sendOsc_OF;
		bool					sendOsc_FT;
		bool					sendOsc_FHT;

		/////////////////////////////


		unsigned char			*blackPixels;

		ofxPoint2f				*warpMult;
		ofxPoint2f				*cropMult;

		int						workingW;
		int						workingH;
		int						totPixels;


		int						firstScreenW;
		int						firstScreenH;
		int						secondScreenW;
		int						secondScreenH;

		#ifdef TARGET_LINUX
		bool                    linuxFullScreen;
		#endif

		bool					bLearnBackground;	// background saving boolean var
		bool					sendingSocketReady;

		// HSV range variables
		int						minHue, maxHue, minSat, maxSat, minVal, maxVal;

		///////////////////////////////////////////////////////////////
		// FRAME GRABBER
		FrameGrabber			*frameGrab;
		bool					startGrab;
		///////////////////////////////////////////////////////////////
		

	private:

		ofxOscSender			sender;	// sending OSC data object

};

#endif
