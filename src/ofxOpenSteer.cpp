#include "ofxOpenSteer.h"

using namespace OpenSteer;

std::vector<ofxOpenSteerSphereObstacle*> ofxOpenSteer::sphereObstacles;
std::vector<ofxOpenSteerRectObstacle*> ofxOpenSteer::rectObstacles;

ofxOpenSteer::ofxOpenSteer()
{
	createProximityDatabase();
}

ofxOpenSteer::~ofxOpenSteer()
{
}

ofxVehicle* ofxOpenSteer::createVehicle()
{
	ofxVehicle* v=new ofxVehicle();
	v->setProximityDatabase(proximityDB);
	vehicles.push_back(v);
	return v;
}

void ofxOpenSteer::update()
{
	float curTime=ofGetElapsedTimef();
	update(curTime, curTime-lastTime);
}

void ofxOpenSteer::update(float currentTime, float elapsedTime)
{
	for (vehicleIterator i = vehicles.begin(); i != vehicles.end(); i++) {
		(*i)->update (currentTime, elapsedTime);
	}
	lastTime=currentTime;
}

void ofxOpenSteer::draw()
{
	for (vehicleIterator i = vehicles.begin(); i != vehicles.end(); i++) {
		(*i)->draw ();
	}
	ofSetColor(120, 120, 120);
	for(unsigned int i=0;i<sphereObstacles.size();i++){
		ofCircle(convertOpenSteerVec(sphereObstacles[i]->center), sphereObstacles[i]->radius);
	}
}

void ofxOpenSteer::createProximityDatabase()
{
	const Vec3 center;
	const float div = 20.0f;
	const Vec3 divisions (div, 1.0f, div);
	const float diameter = ofGetWidth(); //XXX need better way to get this
	const Vec3 dimensions (diameter, diameter, diameter);
	typedef LQProximityDatabase<AbstractVehicle*> LQPDAV;
	proximityDB = new LQPDAV (center, dimensions, divisions);
}

ofxOpenSteerPath* ofxOpenSteer::convertPolyLine(std::vector<ofPoint> points, float radius)
{
	Vec3 pathPoints[points.size()];
	for(unsigned int i=0; i<points.size(); i++) {
		pathPoints[i]=convertOfVec(points[i]);
	}
	//return new ofxOpenSteerPath();
	return new PolylineSegmentedPathwaySingleRadius (points.size(),pathPoints,radius,false);
}

ofxOpenSteerSphereObstacle* ofxOpenSteer::createSphereObstacle(ofPoint center, float radius)
{
	ofxOpenSteerSphereObstacle* ret = new ofxOpenSteerSphereObstacle();
	ret->center = convertOfVec(center);
	ret->radius=radius;
	sphereObstacles.push_back(ret);
	return ret;
}
