#include "ofxOpenSteerVehicle.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

AVGroup ofxOpenSteerVehicle::neighbors;

ofxOpenSteerVehicle::ofxOpenSteerVehicle(){
    proximityDB = NULL;
	proximityToken = NULL;
	path = NULL;
    reset();
}
ofxOpenSteerVehicle::~ofxOpenSteerVehicle(){
}

void ofxOpenSteerVehicle::reset(){
	SimpleVehicle::reset();    
}
void ofxOpenSteerVehicle::update(){
	update(ofGetElapsedTimef(), 1.f/ofGetFrameRate());
}
void ofxOpenSteerVehicle::update(float curTime, float elapsedTime){
	applySteeringForce (getSteeringDirection (elapsedTime), elapsedTime);
	recordTrailVertex (curTime, position());
	proximityToken->updateForNewPosition (position());
}
void ofxOpenSteerVehicle::draw(){    
	ofSetColor(255);
	ofCircle(getPosition(), radius());
	Vec3 f = forward() * radius();
	Vec3 p = position();
	Vec3 fp = f + p;
	ofSetColor(0);
	ofLine(p.x, p.y , fp.x, fp.y);
}
Vec3 ofxOpenSteerVehicle::getSteeringDirection(const float elapsedTime){
	return Vec3(0, 0, 0);
}

void ofxOpenSteerVehicle::setPosition(ofPoint position){
	setPosition(position.x, position.y, position.z);
}
void ofxOpenSteerVehicle::setPosition(float x, float y, float z){
	SimpleVehicle::setPosition(Vec3(x, y, z));
}
ofPoint ofxOpenSteerVehicle::getPosition(){
	return toOf(position());
}

void ofxOpenSteerVehicle::setProximityDatabase(ProximityDatabase* db){
	proximityDB=db;
	proximityToken = proximityDB->allocateToken (this);
}
ProximityDatabase* ofxOpenSteerVehicle::getProximityDatabase(){
	return proximityDB;
}
ProximityToken* ofxOpenSteerVehicle::getProximityToken(){
	return proximityToken;
}

void ofxOpenSteerVehicle::setPath(ofxOpenSteerPath* p){
	path = p;
}
ofxOpenSteerPath* ofxOpenSteerVehicle::getPath(){
	return path;
}
void ofxOpenSteerVehicle::unsetPath(){
	path = NULL;
}

void ofxOpenSteerVehicle::addObstacle(ofxOpenSteerObstacle* o){
	obstacles.push_back(o);
}
void ofxOpenSteerVehicle::removeObstacle(ofxOpenSteerObstacle* o){
    obstacles.erase(std::remove(obstacles.begin(), obstacles.end(), o), obstacles.end());
}
ObstacleGroup ofxOpenSteerVehicle::getObstacles(){
    return obstacles;
}