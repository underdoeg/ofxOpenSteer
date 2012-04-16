#include "ofxOpenSteerPlugin.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

ofxOpenSteerPlugin::ofxOpenSteerPlugin(){
	proximityDB = NULL;
	
	// default PD settings
	pdSettings.center = Vec3();
	pdSettings.divX = 20.f;
	pdSettings.divY = 20.f;
	pdSettings.divZ = 20.f;
	pdSettings.diameter = ofGetWidth();
	
	setup();
}
ofxOpenSteerPlugin::~ofxOpenSteerPlugin(){
	exit();
}
void ofxOpenSteerPlugin::setup(){
	createProximityDatabase();
}
void ofxOpenSteerPlugin::setup(ofxOpenSteerProximityDatabaseSettings pdSettings){
	setProximityDatabaseSettings(pdSettings);
	setup();
}

void ofxOpenSteerPlugin::update(){
	update(ofGetElapsedTimef(), 1.f/ofGetFrameRate());
}
void ofxOpenSteerPlugin::update(float currentTime, float elapsedTime){
	updatePhaseActive = true; //XXX ideally should be moved to main app's events, kepping it here so app can be cleaner. it's safe while single threaded
	for (vehicleIterator i = vehicles.begin(); i != vehicles.end(); i++) {
		(*i)->update (currentTime, elapsedTime);
	}
	updatePhaseActive = false; //XXX ideally should be moved to main app's events, kepping it here so app can be cleaner. it's safe while single threaded
}
void ofxOpenSteerPlugin::draw(){
	drawPhaseActive = true; //XXX ideally should be moved to main app's events, kepping it here so app can be cleaner. it's safe while single threaded
	for (vehicleIterator i = vehicles.begin(); i != vehicles.end(); i++) {
		(*i)->draw();
	}
	drawPhaseActive = false; //XXX ideally should be moved to main app's events, kepping it here so app can be cleaner. it's safe while single threaded
}
void ofxOpenSteerPlugin::exit(){
	// delete the proximity database
	if(proximityDB) delete proximityDB;
	proximityDB = NULL;
	
	// clear vehicles
	while (vehicles.size() > 0){
		const AbstractVehicle* v = (AbstractVehicle*)vehicles.back();
		vehicles.pop_back();
		delete v;
	}
}

void ofxOpenSteerPlugin::addVehicle(ofxOpenSteerVehicle* v, bool reset){
	v->setProximityDatabase(proximityDB);
	if(reset) v->reset();
	vehicles.push_back(v);
}
void ofxOpenSteerPlugin::removeVehicle(ofxOpenSteerVehicle* v){
	vehicles.erase(std::remove(vehicles.begin(), vehicles.end(), v), vehicles.end());
}
vehicleList ofxOpenSteerPlugin::getVehicles(){
	return vehicles;
}
void ofxOpenSteerPlugin::setProximityDatabaseSettings(ofxOpenSteerProximityDatabaseSettings pdSettings){
	this->pdSettings.center = pdSettings.center;
	this->pdSettings.divX = pdSettings.divX;
	this->pdSettings.divY = pdSettings.divY;
	this->pdSettings.divZ = pdSettings.divZ;
	this->pdSettings.diameter = pdSettings.diameter;
}

void ofxOpenSteerPlugin::createProximityDatabase(){
	const Vec3 divisions (pdSettings.divX, pdSettings.divY, pdSettings.divZ);
	const Vec3 dimensions (pdSettings.diameter, pdSettings.diameter, pdSettings.diameter);
	typedef LQProximityDatabase<AbstractVehicle*> LQPDAV;
	proximityDB = new LQPDAV (pdSettings.center, dimensions, divisions);
}



