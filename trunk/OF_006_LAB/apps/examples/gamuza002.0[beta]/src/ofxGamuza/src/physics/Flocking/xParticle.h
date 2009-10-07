#ifndef _X_PARTICLE_H
#define _X_PARTICLE_H


#include "ofMain.h"
#include "ofxVectorMath.h"

#include <vector>

typedef struct
{

	bool 	    bSeekingTarget;
	ofxVec3f    pos;

	float	    nearDist;
	float	    nearForce;
	float	    distToTarget;

} xPTarget;


typedef struct
{
    int countCoh;
    int countSep;
    int countAlign;

    ofxVec3f sumCoh;
    ofxVec3f sumSep;
    ofxVec3f sumAlign;

    float sepDist;
    float alnDist;
    float cohDist;

    float separation;
    float cohesion;
    float alignment;

} xPFlockData;


class xParticle{

	public:

		xParticle();
		~xParticle();


		ofxVec3f    getPosition(){ return pos; }
		float       getX(){ return pos.x; }
		float       getY(){ return pos.y; }
		float       getZ(){ return pos.z; }


		void    setup(float damping, ofxVec3f loc_, bool bRepeling = true);
		void	reset();
		void	resetForces();

		void	update();
		void	updateDamping();
		void    updateToroidal();

		void	updateNearness();
		void	nearness( xPTarget * target, float nearDist, float nearForce );

        	int 	setTarget( ofxVec3f targ, float dist, float force, int id = 0);
        	void    giveUpOnTarget(int id=0);

		void	repelFrom( ofxVec3f pos, float radius, float force  );
		void	repelFrom( xParticle * sister, float radius, float force);
		void    addForce( ofxVec2f frc );

        	void    resetFlocking();
        	void    addForFlocking( xParticle * sister);
        	void    applyFlockingForces();

		void	draw();
		void	drawForces();
		void	drawTarget();


		// forces
		float damping, dampingLen;
		float scaleFlockForce, scaleVecFForce;                      // use these to reduce influence of vec field or flocking

        // properties
        ofxVec3f pos;
        ofxVec3f vel;

        // binning
        int 	bitFlagW;
        int 	bitFlagH;	// to avoid numerous collision tests

        //states
        bool    bRepeling;

        // targets
		vector<xPTarget>	targets;

		// flocking
		xPFlockData flock;



	protected:




};

#endif

