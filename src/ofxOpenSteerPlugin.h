#pragma once

#include "ofxOpenSteer.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;


class ofxOpenSteerPlugin {

public:
	ofxOpenSteerPlugin();
	~ofxOpenSteerPlugin();
	
	virtual string name(){ return "ofxOpenSteerPlugin"; };
	virtual void setup();
	virtual void update();
	virtual void update(float currentTime, float elapsedTime);
	virtual void draw();
	virtual void exit();
	
	// aliases to keep in tune with OpenSteer's naming convention
	void open() { return setup(); };
	void close() { return exit(); };
    
    virtual void addVehicle(ofxOpenSteerVehicle* v);
    virtual void removeVehicle(ofxOpenSteerVehicle* v);
    virtual VehicleGroup getVehicles();

protected:
	VehicleGroup vehicles;
};