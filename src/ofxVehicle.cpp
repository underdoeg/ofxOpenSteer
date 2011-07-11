#include "PolylineSegmentedPathwaySingleRadius.h"
#include "ofxVehicle.h"
#include "ofxOpenSteer.h"
#include "Draw.h"
using namespace OpenSteer;

AVGroup ofxVehicle::neighbors;


ofxVehicle::ofxVehicle()
{
	reset();
}

ofxVehicle::~ofxVehicle()
{
}

void ofxVehicle::reset()
{
	SimpleVehicle::reset();
	onReset();
}

void ofxVehicle::onReset()
{
	isFollowingPath=false;
	isWandering=false;
	setSpeed(0);
	setMaxForce(30000);      // steering force is clipped to this magnitude
	setMaxSpeed(50);

	setUp(Vec3(0,0,1));
	setForward(Vec3(0,1,0));
	//Vec3 rVec=RandomVectorInUnitRadiusSphere();
	//rVec.z=0;
	setForward (Vec3(1,0,0));
	setSide (localRotateForwardToSide (forward()));
	setTrailParameters (50, 100);
}

void ofxVehicle::followPath(ofxOpenSteerPath* p)
{
	path = p;
	isFollowingPath=true;
}

void ofxVehicle::update()
{
	//float curTime=ofGetElapsedTimef();
	//update(curTime, curTime-lastTime);
}

void ofxVehicle::update(float curTime, float elapsedTime)
{
	applySteeringForce (getSteeringDirection (elapsedTime), elapsedTime);
	recordTrailVertex (curTime, position());
	proximityToken->updateForNewPosition (position());
}

Vec3 ofxVehicle::getSteeringDirection(const float elapsedTime)
{
	Vec3 steeringForce = Vec3(0, 0, 0);

	//check for neighbours
	// otherwise consider avoiding collisions with others
	Vec3 collisionAvoidance;
	const float caLeadTime = 3;

	const float leakThrough = 0.1f;

	// find all neighbors within maxRadius using proximity database
	// (radius is largest distance between vehicles traveling head-on
	// where a collision is possible within caLeadTime seconds.)
	const float maxRadius = caLeadTime * maxSpeed() * 2;
	neighbors.clear();
	proximityToken->findNeighbors (position(), maxRadius, neighbors);

	Vec3 obstacleAvoidance;
	if (leakThrough < frandom01()) {
		obstacleAvoidance = steerToAvoidObstacles (6, obstacles);
	}

	// if obstacle avoidance is needed, do it
	if (obstacleAvoidance != Vec3::zero) {
		steeringForce += obstacleAvoidance;
	} else {

		//CHECK FOR COLLISION WITH NEIGHBORS
		if (leakThrough < frandom01())
			collisionAvoidance = steerToAvoidNeighbors (caLeadTime, neighbors) * 2000;

		if (collisionAvoidance != Vec3::zero) {
			steeringForce += collisionAvoidance;
		} else {
			if(isWandering)
				steeringForce += steerForWander (elapsedTime);
			if(isFollowingPath) { //KEEP ON PATH IF NECESSARY
				//steeringForce += steerForWander (elapsedTime);
				// do (interactively) selected type of path following
				const float pfLeadTime = 7;
				const Vec3 pathFollow =steerToFollowPath(1, pfLeadTime, *path);

				// add in to steeringForce
				steeringForce += pathFollow;
			}
		}
	}
	steeringForce.z=0;
	return steeringForce;
}

void ofxVehicle::draw()
{
	drawBasic2dCircularVehicle(*this, Color(120));
	ofNoFill();
	ofColor(120);
	ofCircle(getPosition(), getRadius());
}

ofVec3f ofxVehicle::getPosition()
{
	return ofxOpenSteer::convertOpenSteerVec(position());
}

float ofxVehicle::setRadius(float r)
{
	return SimpleVehicle::setRadius(r);
}

float ofxVehicle::getRadius()
{
	return SimpleVehicle::radius();
}

void ofxVehicle::setPosition(float x, float y, float z)
{
	SimpleVehicle::setPosition(Vec3(x, y, z));
}

void ofxVehicle::setProximityDatabase(ProximityDatabase* db)
{
	proximityDB=db;
	proximityToken = proximityDB->allocateToken (this);
}

void ofxVehicle::addObstacle(ofxOpenSteerObstacle* o)
{
	obstacles.push_back(o);
}
