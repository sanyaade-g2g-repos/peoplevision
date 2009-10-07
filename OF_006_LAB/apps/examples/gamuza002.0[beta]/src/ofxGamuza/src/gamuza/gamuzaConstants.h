// SCREENS DEFINITION
#define		FSWIDTH						1440 // first screen width
#define		FSHEIGHT					900	 // first screen height
#define		SSWIDTH						1024 // second screen width
#define		SSHEIGHT					768	 // second screen height

#define		INV_WIDTH					(1.0f/SSWIDTH)
#define		INV_HEIGHT					(1.0f/SSHEIGHT)

// BACKGROUND SUBTRACTION
#define		TRACKING_BG					"trackingBG.jpg"
#define		TRACKING_BW_BG				"trackingBG_bw.jpg"

// FPS (set it to 25 if you want to grab frames of your work)
#define		FPS							30

// COUNTOUR FINDER
#define		MAX_NUM_CONTOURS_TO_FIND	20

// RECURSIVE CENTROID
#define		CENTROID_DEPTH				5

// OPTICAL FLOW
#define		OPTICAL_FLOW_GRID_X			(int)(320/OPTICAL_FLOW_COLS_STEP)
#define		OPTICAL_FLOW_GRID_Y			(int)(240/OPTICAL_FLOW_ROWS_STEP)

#define		CAM_VELOCITY_BLUR			15 // how much to blur the cam velocity

// AUDIO INPUT FFT
#define		BUFFER_SIZE					256
#define		NUM_WINDOWS					80

// FLOCKING PARTICLES
#define		NUM_PARTICLES				40

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////// PHYSICS

// FLUID
#define		FLUID_NX					120
#define		FLUID_NY					(FLUID_NX * SSHEIGHT / SSWIDTH)
#define		FLUID_VISC					0.0002f
#define		FLUID_DT					2.0f
#define		FLUID_FADE_SPEED			0.0001f
#define		FLUID_INV_NX				(1.0f/(FLUID_NX+2))
#define		FLUID_INV_NY				(1.0f/(FLUID_NY+2))
#define		FLUID_INJECT_STEP			1

#define		NUM_CAMERAS					1									// total number of cameras
#define		CAM_FLUID_WIDTH				(FLUID_NX / NUM_CAMERAS)			// how many fluid units does each camera cover
#define		CAM_FLUID_INV_WIDTH			(1.0f / CAM_FLUID_WIDTH)
#define		CAM_DISPLAY_WIDTH			(SSWIDTH / NUM_CAMERAS)

// PARTICLE
#define		BALL_MAX_COUNT				40
#define		BALL_FLUID_VEL_MULT			150.0f
#define		BALL_BALL_ATTRACTION		100.0f
#define		BALL_GRAVITY				0.05f
#define		BALL_CORE_SIZE_MULT			0.1f
#define		BALL_MAX_SPEED				20.0f
#define		BALL_DRAG_AMOUNT			0.0001f
#define		BALL_LINE_THRESH			(SSHEIGHT * 0.3f * SSHEIGHT * 0.3f)	// threshold for when balls start showing lines (distance squared)
#define		BALL_FLUID_CREATE			0.0f
#define		BALL_FLUID_SPEED			4.0f
#define		BALL_MIN_SIZE				50.0f
#define		BALL_MAX_SIZE				150.0f
#define		BALL_SIZE_LIFE_MULT			0.2f		// life span of the ball (depends on size - size 10 lasts 10 seconds etc.)
#define		BALL_TRIGGER_THRESH			20.0f
#define		BALL_FRAMES_BEFORE_TRIGGER	30			// dont trigger another one for another xxx frames

// GLITTER
#define		GLITTER_MAX_COUNT			1000
#define		GLITTER_MIN_SIZE			3.0f		// min size
#define		GLITTER_MAX_SIZE			10.0f		// max size
#define		GLITTER_TRAIL_LENGTH		0.3f		// length of trail (in frames) * size
#define		GLITTER_TRAIL_OPACITY		0.3f
#define		GLITTER_TRAIL_RADIUS		0.4f		// start radius ratio to particle ratio
#define		GLITTER_DIST_RADIUS			50.0f		// how far from birth point can glitter be created
#define		GLITTER_MAX_SPEED			5.0f
#define		GLITTER_LIFE_MIN			5.0f
#define		GLITTER_LIFE_MAX			10.0f
#define		GLITTER_FLUID_VEL_MULT		80.0f		// how much fluid velocity affects glitter
#define		GLITTER_TRIGGER_THRESH		15.0f
#define		GLITTER_WIGGLE_FORCE		2.0f		// amount of force coming from noisemap
#define		GLITTER_NOISE_SIZE			400			// how much the noisemap is scaled down (the bigger the number, the more wiggly)

/////////////////////////////////////////////////////////////////////////////////////////////////////
