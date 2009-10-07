#include "circles.h"

circles::circles(int x, int y, int r, int maxnodes)
{


	maxNodes    = maxnodes;

	radius          = r;
	radiusTo        = r;


    centerP.setup(.99,ofxVec3f(x,y,0));

    deformPoint = 0.f;


    for( int i=0;i<maxNodes;i++)
    {
        nodesNow.push_back(ofxVec2f(0,0));
        nodesTo.push_back(ofxVec2f(0,0));

        xParticle p;
        p.setup(.9, ofxVec3f(0,0,0));
        particleNodes.push_back(p);


    }


}

circles::~circles()
{
}

void circles::update()
{



	deformPoint += .01;

	ofxVec2f defPt;
	defPt.x = centerP.getX() + radius * cos(deformPoint);
	defPt.y = centerP.getY() + radius * sin(deformPoint);


    centerP.update();

    ofxVec2f dir = ofxVec2f(defPt -ofxVec2f(centerP.getX(),centerP.getY() ) ).normalized();
	deformNodes( defPt, 80, 0.25f, dir);


    for(unsigned int i = 0; i < particleNodes.size(); i++)
    {
        particleNodes[i].setTarget( ofxVec3f(nodesTo[i].x,nodesTo[i].y), 10, .3, false);
        particleNodes[i].update();

        nodesNow[i].x = particleNodes[i].pos.x;
        nodesNow[i].y = particleNodes[i].pos.y;

    }

    setCirclePositions( false );

}

void circles::setRadiusTarget( float r )
{
   radiusTo = r;

}

void circles::setAlphaTarget( float alph )
{
    alphaTo = alph;
}

void circles::setPosTarget( ofPoint loc )
{
    centerP.setTarget( ofxVec3f(loc.x,loc.y,0), 10, 1.1,false);
}

void circles::setPosition( ofPoint loc )
{
    centerP.pos = ofxVec3f(loc.x,loc.y,0);
}


void circles::setCirclePositions( bool bSetAll )
{

    ofxVec2f center = ofxVec2f(centerP.pos.x,centerP.pos.y);

    // calculate circumference
    float C = (2*radius)*PI;

    // divide circumfernce by nodes
    //C *= (radius*.0025)+spiralOverlap;//1.65;

    float s = C / (float)maxNodes;

    float theta = s / radius;



    for( int i = 0; i < maxNodes; i++)
    {

        ofxVec2f diff = nodesTo[i];

        nodesTo[i].x = center.x + radius * cos(theta*i);
        nodesTo[i].y = center.y + radius * sin(theta*i);

        // move particle the displacement amount of rotation
        //diff = nodesTo[i] - diff;
        //particleNodes[i].pos.x += .9*diff.x;
        //particleNodes[i].pos.y += .9*diff.y;



        if(bSetAll)
        {
            nodesNow[i] =  nodesTo[i];
            particleNodes[i].pos = nodesTo[i];

        }


    }




}


void circles::deformNodes( ofxVec2f pt, float dist, float force, ofxVec2f direction)
{

    for( int i = 0; i < maxNodes; i++)
    {

        float d = (pt - nodesNow[i]).length();

        if(d < dist)
        {
            float scl = 1 - (d / dist);
            particleNodes[i].vel += (scl*force)*direction;
        }


    }
}

void circles::draw()
{
     for( int i = 0; i < maxNodes; i++)
    {
        particleNodes[i].draw();
    }
}
