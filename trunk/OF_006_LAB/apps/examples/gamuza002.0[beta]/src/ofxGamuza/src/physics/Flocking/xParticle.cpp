#include "xParticle.h"

xParticle::xParticle()
{

	memset(&targets,0,sizeof(xPTarget));

	bRepeling = false;

	pos.set(0,0,0);
	vel.set(0,0,0);


	damping         = .99f;
	dampingLen      = 100.f;



}
//----------------------------------------------------------------------------------------------------------------
xParticle::~xParticle()
{
    targets.clear();
}

//----------------------------------------------------------------------------------------------------------------
void xParticle::reset()
{
    targets.clear();
}
//----------------------------------------------------------------------------------------------------------------
void xParticle::update()
{


        updateNearness();

		updateDamping();

        pos += vel;


}

//----------------------------------------------------------------------------------------------------------------
void xParticle::updateDamping()
{

	float damp      = damping;

	float velLen = vel.length();

    if(velLen > dampingLen)
            damp *= sqrt(dampingLen) / sqrt(velLen);

    vel.x *= damp;
    vel.y *= damp;
    vel.z *= damp;


}
//----------------------------------------------------------------------------------------------------------------
void xParticle::repelFrom( ofxVec3f pos_, float radius, float force  )
{

	if(!bRepeling) return;

	float repelForce = force;

    ofxVec3f diff = pos-pos_;

    float distance = diff.length();


    if (distance < radius)
    {

        float pct = 1 - (distance / radius);

        diff.normalize();

		vel -= diff * pct * repelForce;
    }


}
//----------------------------------------------------------------------------------------------------------------
void xParticle::repelFrom( xParticle * sister, float radius, float force)
{

	float pct = 1.0f;

	if ( (bitFlagW & sister->bitFlagW) ||
         (bitFlagH & sister->bitFlagH))
    {

        ofxVec3f diff = pos - sister->pos;

        float distance = diff.length();

        // update collision forces
        if (distance < radius)
        {

            pct = 1 - (distance / radius);
            diff.normalize();

            if( bRepeling && sister->bRepeling)
            {
				vel += diff * pct * force;
            }

            if(sister->bRepeling)
                sister->vel -= diff * pct * force;

        }

    }

}
//----------------------------------------------------------------------------------------------------------------
void xParticle::updateNearness()
{
	for(unsigned int i = 0; i < targets.size(); i++)
	{
	    if(targets[i].bSeekingTarget)
            nearness( &targets[i], targets[i].nearDist, targets[i].nearForce);
	}
}
//----------------------------------------------------------------------------------------------------------------
void xParticle::nearness( xPTarget * target, float nearDist, float nearForce)
{

        ofxVec3f targ  	= target->pos;
		ofxVec3f diff  	= targ-pos;

		float length = diff.length();

		if (length > nearDist) length = nearDist;

		float scale = length / nearDist;

		diff.normalize();

		vel += diff * scale * nearForce;

		target->distToTarget = (targ-pos).length();



}
//----------------------------------------------------------------------------------------------------------------
void xParticle::updateToroidal()
{
    if(pos.x < 0){ pos.x = ofGetWidth(); }
    if(pos.x > ofGetWidth()){ pos.x = 0; }
    if(pos.y < 0){ pos.y = ofGetHeight();}
    if(pos.y > ofGetHeight()){ pos.y = 0;}

}
//----------------------------------------------------------------------------------------------------------------
void xParticle::setup(float damping_, ofxVec3f loc_, bool repel)
{

    pos = loc_;
	vel.set(0,0,0);

	bitFlagW = 1;
	bitFlagH = 1;

    reset();

	bRepeling = repel;

	damping          = damping_;
	dampingLen       = 100;

    flock.sepDist = 80;
	flock.alnDist = 100;
	flock.cohDist = 100;

	flock.separation = .01;
    flock.cohesion  =  .015;
    flock.alignment =  .015;
}
//----------------------------------------------------------------------------------------------------------------
void xParticle::resetForces()
{

	// relate to size??
	damping         = .80f;
	dampingLen      = 10;

}
//----------------------------------------------------------------------------------------------------------------
int xParticle::setTarget( ofxVec3f targ, float dist, float force, int id)
{
	xPTarget target;

	target.pos 	          = targ;
	target.nearDist 	  = dist;
	target.nearForce 	  = force;
	target.bSeekingTarget = true;
	target.distToTarget   = (targ-pos).length();

	if((unsigned int)id >= 0 && targets.size() > (unsigned int)id )
    {
        memcpy(&targets[id],&target,sizeof(xPTarget));
        return id;
    }
    else
    {
        targets.push_back(target);
        return targets.size() - 1;
    }


}
//----------------------------------------------------------------------------------------------------------------
void xParticle::giveUpOnTarget(int id)
{
	if(targets.size() > (unsigned int)id)
        targets[id].bSeekingTarget = false;
}
//----------------------------------------------------------------------------------------------------------------
void xParticle::addForce( ofxVec2f frc )
{
    vel += frc;
}
//----------------------------------------------------------------------------------------------------------------
void xParticle::resetFlocking()
{
    flock.countCoh 	    = 0;
	flock.countSep	    = 0;
	flock.countAlign	= 0;

	flock.sumCoh.set(0,0,0);
	flock.sumSep.set(0,0,0);
	flock.sumAlign.set(0,0,0);
}
 //----------------------------------------------------------------------------------------------------------------
void xParticle::addForFlocking( xParticle * sister)
{

		float distSep   = flock.sepDist;
		float distAlign = flock.alnDist;
		float distCoh   = flock.cohDist;

		ofxVec3f diff, direction;
		float d, attention_factor;


		// add for seperation
		diff = sister->pos - pos;
		d 	 = diff.length();
		//diff.normalize();


		// don't use this for now
		/*
		// attention angle factor
		direction = sister->vel;
		direction.normalize();
		attention_factor = diff.dot( direction );


		const float CUTOFF = .65f;
		attention_factor -= CUTOFF;
		attention_factor /= (1.0f-CUTOFF);
        */

        attention_factor = 1;

		if( attention_factor > 0.0f )
		{
			if( d > 0 && d < distSep )
			{
			flock.sumSep += (diff / d) * attention_factor;
			flock.countSep++;
			}

			// add for alignment
			if( d > 0 && d < distAlign )
			{
			flock.sumAlign += sister->vel.normalized()  * attention_factor;
			flock.countAlign++;
			}

			//add for cohesion
			if( d > 0 && d < distCoh )
			{
			flock.sumCoh += sister->pos * attention_factor;
			flock.countCoh++;
			}
		}
}
//----------------------------------------------------------------------------------------------------------------
void xParticle::applyFlockingForces()
{// seperation
	if(flock.countSep > 0)
	{
		flock.sumSep /= (float)flock.countSep;
	}


	// alignment
	if(flock.countAlign > 0)
	{
		flock.sumAlign /= (float)flock.countAlign;
	}

	// cohesion
	if(flock.countCoh > 0)
	{

		flock.sumCoh /= (float)flock.countCoh;
		flock.sumCoh -= pos;
	}


	float sepFrc 	= flock.separation;
	float cohFrc 	= flock.cohesion;
	float alignFrc 	= flock.alignment;

	vel += (flock.sumSep.normalized() 	   * sepFrc);
    vel += (flock.sumAlign.normalized()    * alignFrc);
    vel += (flock.sumCoh.normalized() 	   * cohFrc);

}
//----------------------------------------------------------------------------------------------------------------
void xParticle::draw()
{

	ofCircle(pos.x, pos.y, 2);


}
//----------------------------------------------------------------------------------------------------------------
void xParticle::drawForces()
{

	ofNoFill();

}
//----------------------------------------------------------------------------------------------------------------
void xParticle::drawTarget()
{
	ofNoFill();
	for(unsigned int i = 0; i < targets.size(); i++)
	{
	    ofCircle(targets[i].pos.x, targets[i].pos.y,10);
    }
}
//----------------------------------------------------------------------------------------------------------------

/*
//----------------------------- check bounding box intersection:
	ofxVec3f pt0;  pt0.set(boundingBoxBottom);
	ofxVec3f pt1;  pt1.set(boundingBoxTop);
	ofxVec3f pt2;  pt2.set(sister->boundingBoxBottom);
	ofxVec3f pt3;  pt3.set(sister->boundingBoxTop);

	bool bIntersect = true;

	if(pt1.y < pt2.y)		bIntersect = false;
	if(pt0.y > pt3.y)		bIntersect = false;
	if(pt1.x < pt2.x)		bIntersect = false;
	if(pt0.x > pt3.x)		bIntersect = false;

	//if (!bIntersect) return;



//----------------------------------------------------------------------------------------------------------------
void xParticle::bounceOffWalls()
{


	ofxVec3f minBB = wallMin;
	ofxVec3f maxBB = wallMax;


    //states.bHitWall = false;

	bool bHitEdge = false;

    if (pos.x < minBB.x){
        pos.x = minBB.x;
		vel.x *= -1;
        bHitEdge = true;
    }
    if (pos.x > maxBB.x){
        pos.x = maxBB.x;
        vel.x *= -1;
        bHitEdge = true;
    }
    if (pos.y < minBB.y){
        pos.y = minBB.y;
        vel.y *= -1;
        bHitEdge = true;
    }
    if (pos.y > maxBB.y){
        pos.y = maxBB.y;
        vel.y *= -1;
        bHitEdge = true;
    }
    if (pos.z < minBB.z){
        pos.z = minBB.z;
        vel.z *= -1;
        bHitEdge = true;
    }
    if (pos.z > maxBB.z){
        pos.z = maxBB.z;
        vel.z *= -1;
        bHitEdge = true;
    }


}


//--------------------------------------------------------------------------
void xParticle::calcBoundingBox(){

	boundingBoxBottom 	= pos;
	boundingBoxTop 		= pos;

	if ( boundingBoxBottom.x > pos.x){
			boundingBoxBottom.x = pos.x;
    }

    if ( boundingBoxBottom.y > pos.y){
			boundingBoxBottom.y = pos.y;
    }


    if ( boundingBoxTop.x < pos.x){
			boundingBoxTop.x = pos.x;
    }

    if ( boundingBoxTop.y < pos.y){
			boundingBoxTop.y = pos.y;
    }


	boundingBoxBottom.x -= 10;
	boundingBoxBottom.y -= 10;
	boundingBoxTop.x += 10;
	boundingBoxTop.y += 10;

}

*/
