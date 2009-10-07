#ifndef _MY_CIRCLES_H
#define _MY_CIRCLES_H

#include "ofMain.h"
#include "xParticle.h"


class circles{


	public:

		circles(){};
		circles(int x, int y, int radius, int maxnodes);
		~circles();

		void update();
		void updateRadius();
		void updateAlpha();
        void setCirclePositions( bool bSetAll=false );
        void deformNodes( ofxVec2f pt, float dist, float force, ofxVec2f direction);

        void setRadiusTarget( float radius );
        void setAlphaTarget( float alph );
        void setPosTarget( ofPoint loc );
        void setPosition( ofPoint loc );

        float getRadius(){ return radius; }
		float getAlpha(){ return alpha; }
		float getX(){ return centerP.pos.x; }
		float getY(){ return centerP.pos.y; }
        ofPoint getPosition(){ return ofPoint(centerP.pos.x,centerP.pos.y) ;}

        void draw();


		int maxNodes;

		float   alpha;
		float   alphaTo;

		float   radius;
		float   radiusTo;


		xParticle         centerP;
		vector<ofxVec2f>    nodesNow;
		vector<ofxVec2f>    nodesTo;
		vector<xParticle>   particleNodes;


		float	deformPoint;


};


#endif
