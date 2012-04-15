#pragma once

#include "ofxOpenSteer.h"
#include "Boid.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

// Extend the flocking boid adding path following behaviour
class PathBoid: public Boid {
	
	void reset(){
		// reset the vehicle
		Boid::reset ();
				
		// randomize initial position
		setPosition (RandomVectorInUnitRadiusSphere() * 100);
		
		// notify proximity database that our position has changed
		if(proximityToken) proximityToken->updateForNewPosition (position());
	};
	
	Vec3 getSteeringForce(const float elapsedTime){
		// Inherit the flocking force
		Vec3 flock = Boid::getSteeringForce(elapsedTime);
		
		// Get path following forces
		Vec3 followPath = steerToFollowPath (1, 1.f, *path);		
		Vec3 stayOnPath = steerToStayOnPath(1.f, *path);
		
		return followPath + stayOnPath + flock * 0.3; // reduce flocking force 
	}
	
};

class PathFollowing: public ofxOpenSteerPlugin {
	
public:
	
	ofPath circle;
	ofxOpenSteerPath* path;
	
	string name(){ return "Path Following"; };
	
	void setup(){
		ofxOpenSteerPlugin::setup();
		
		ofBackground(0, 255, 255);
		
		// Create a path
		circle.arc(ofPoint(), 20, 20, 0, 360);
		circle.setFilled(false);
		
		path = createPath(circle.getOutline()[0], 1.5); // polyline, radius
		
		for(unsigned int i=0;i<100;i++){
			PathBoid* v = new PathBoid();
			v->setPath(path);
			addVehicle(v);
		}
	};
	
	void draw(){
		ofxOpenSteerPlugin::draw();
		circle.draw();
	}
	
	void exit(){
		ofxOpenSteerPlugin::exit();
		if(path) delete path;
		path = NULL;
	}
	
};
