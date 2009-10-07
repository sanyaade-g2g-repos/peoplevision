#ifndef _TEST_APP
#define _TEST_APP

#define		WIDTH			640
#define		HEIGHT			480
#define		NUM_CAMERAS					5 								// total number of cameras

#define		CAM_VELOCITY_BLUR			15								// how much to blur the cam velocity
#define		CAM_DISPLAY_WIDTH			(WIDTH / NUM_CAMERAS)
#define		CAM_DISPLAY_HEIGHT			(HEIGHT / NUM_CAMERAS)

#include "ofMain.h"
#include "ofxLABnet.h"
#include "ofxOpenCv.h"
#include "ofxVectorMath.h"
#include "ofxXmlSettings.h"
#include "ofxOsc.h"
#include "coordWarping.h"
#include "ofxEasyGui.h"

#include "guiQuad.h"

class testApp : public ofSimpleApp{
	
	public:
	
		ofxLABnet labNet;
		
		void setup();
		void update();
		void draw();
		
		void keyPressed  (int key);
		void keyReleased (int key);
		
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();
		
		void buildCameras();
		bool newFrame;
		int numCameras;
		int cameraDisplayWidth, cameraDisplayHeight;
	
		ofVideoGrabber		camera [NUM_CAMERAS];
		ofxCvContourFinder		contourFinder[NUM_CAMERAS];
		
		ofxCvGrayscaleImage		cvGray [NUM_CAMERAS];
		ofxCvGrayscaleImage		cvGrayWarped [NUM_CAMERAS];
		ofxCvColorImage			cvColor [NUM_CAMERAS];
		ofxCvGrayscaleImage		cvGrayBg [NUM_CAMERAS];
		ofxCvGrayscaleImage		cvGrayDiff [NUM_CAMERAS];
			
		guiQuad cvWarpQuad [NUM_CAMERAS];
		
		ofxPoint2f quadSrc[NUM_CAMERAS][4];
		ofxPoint2f quadDst[NUM_CAMERAS][4];
		
		coordWarping coordWarp[NUM_CAMERAS];
		
		ofxPoint2f inputPts[NUM_CAMERAS];
		ofxPoint2f outputPts[NUM_CAMERAS];
				
		int cam_w, cam_h;
		
		//blob things
	
		int cameraWidth[NUM_CAMERAS];
		int cameraHeight[NUM_CAMERAS];
		int oldCameraWidth[NUM_CAMERAS];
		int oldCameraHeight[NUM_CAMERAS];
	
		int threshold[NUM_CAMERAS];
		int numBlobs[NUM_CAMERAS];
		int minBlobSize[NUM_CAMERAS];
		int maxBlobSize[NUM_CAMERAS];
		bool bLearnBakground[NUM_CAMERAS];
		bool flipX[NUM_CAMERAS];
		bool flipY[NUM_CAMERAS];
		int	 rotate[NUM_CAMERAS];
		
		bool sendContours;
	
		//gui stuffs
	
		GuiHandler*		gui;
		ofxXmlSettings	settings[NUM_CAMERAS];
	
		ofxXmlSettings	oscSettings;
		string	oscHost;
		int		oscPort;
		
		bool drawDebug;
		bool mouseIsDown;
		GuiPanel*  Panel2;
	
	private:
		void loadSettings();
		void saveSettings();
};

#endif
	
