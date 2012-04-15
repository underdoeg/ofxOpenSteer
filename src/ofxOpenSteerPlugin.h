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
    virtual void setup(ofxOpenSteerProximityDatabaseSettings pdSettings);
	virtual void update();
	virtual void update(float currentTime, float elapsedTime);
	virtual void draw();
	virtual void exit();
	
	// aliases to keep in tune with OpenSteer's naming convention
	void open() { return setup(); };
	void close() { return exit(); };
    
    virtual void addVehicle(ofxOpenSteerVehicle* v, bool reset = true);
    virtual void removeVehicle(ofxOpenSteerVehicle* v);
    virtual vehicleList getVehicles();
	
	virtual void setProximityDatabaseSettings(ofxOpenSteerProximityDatabaseSettings pdSettings);

protected:
	vehicleList vehicles;
	ProximityDatabase* proximityDB;
	ofxOpenSteerProximityDatabaseSettings pdSettings;
    void createProximityDatabase();
};