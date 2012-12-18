#include "ofxOpenSteerVehicle.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

AVGroup ofxOpenSteerVehicle::neighbors;

ofxOpenSteerVehicle::ofxOpenSteerVehicle(){
    color = gGray30;
    reset();
}
ofxOpenSteerVehicle::~ofxOpenSteerVehicle(){
}

void ofxOpenSteerVehicle::reset(){
	SimpleVehicle::reset();  
	clearTrailHistory();
}
void ofxOpenSteerVehicle::update(){
	update(ofGetElapsedTimef(), 1.f/ofGetFrameRate());
}
void ofxOpenSteerVehicle::update(float curTime, float elapsedTime){
	applySteeringForce (getSteeringForce (elapsedTime), elapsedTime);
	recordTrailVertex (curTime, position());
}
void ofxOpenSteerVehicle::draw(){    
	drawBasic3dSphericalVehicle (*this, color);
	annotationVelocityAcceleration ();
	drawTrail();
}
Vec3 ofxOpenSteerVehicle::getSteeringForce(const float elapsedTime){
	return Vec3(0, 0, 0);
}

ofVec3f ofxOpenSteerVehicle::getPosition()
{
	return toOf(position());
}
