#include "ofxOpenSteerPlugin.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

ofxOpenSteerPlugin::ofxOpenSteerPlugin(){
	drawPhaseActive = true;
	createProximityDatabase();
}
ofxOpenSteerPlugin::~ofxOpenSteerPlugin(){
}

void ofxOpenSteerPlugin::update(){
	update(ofGetElapsedTimef(), 1.f/ofGetFrameRate());
}
void ofxOpenSteerPlugin::update(float currentTime, float elapsedTime){
	for (vehicleIterator i = vehicles.begin(); i != vehicles.end(); i++) {
		(*i)->update (currentTime, elapsedTime);
	}
}
void ofxOpenSteerPlugin::draw(){
	for (vehicleIterator i = vehicles.begin(); i != vehicles.end(); i++) {
		(*i)->draw();
	}
}

void ofxOpenSteerPlugin::addVehicle(ofxOpenSteerVehicle* v){
	v->setProximityDatabase(proximityDB);
	vehicles.push_back(v);
}
void ofxOpenSteerPlugin::removeVehicle(ofxOpenSteerVehicle* v){
	vehicles.erase(std::remove(vehicles.begin(), vehicles.end(), v), vehicles.end());
}
vehicleList ofxOpenSteerPlugin::getVehicles(){
	return vehicles;
}

void ofxOpenSteerPlugin::createProximityDatabase(){
	const Vec3 center;
	const float div = 20.0f;
	const Vec3 divisions (div, 1.0f, div);
	const float diameter = ofGetWidth(); //XXX need better way to get this
	const Vec3 dimensions (diameter, diameter, diameter);
	typedef LQProximityDatabase<AbstractVehicle*> LQPDAV;
	proximityDB = new LQPDAV (center, dimensions, divisions);
}



