#pragma once

#include "ofxOpenSteer.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

class ofxOpenSteerPlugin {

public:
	ofxOpenSteerPlugin();
	~ofxOpenSteerPlugin();
    
    virtual void setup(){};
	virtual void update();
	virtual void update(float currentTime, float elapsedTime);
	virtual void draw();
    
    virtual void addVehicle(ofxOpenSteerVehicle* v);
    virtual void removeVehicle(ofxOpenSteerVehicle* v);
    virtual vehicleList getVehicles();

protected:
	vehicleList vehicles;
	ProximityDatabase* proximityDB;
    void createProximityDatabase();
};