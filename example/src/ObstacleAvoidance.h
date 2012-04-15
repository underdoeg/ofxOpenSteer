#pragma once

#include "ofxOpenSteer.h"
#include "Boid.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

// Extend the flocking boid adding obstacle avoidance behaviour
class ObstacleBoid: public Boid {
	
	void reset(){
		// reset the vehicle
		Boid::reset ();
				
		// randomize initial position
		setPosition (Vec3(0, 15, 0) - RandomVectorInUnitRadiusSphere() * 1);
		
		// notify proximity database that our position has changed
		if(proximityToken) proximityToken->updateForNewPosition (position());
	};
	
	Vec3 getSteeringForce(const float elapsedTime){
		// reset if target is found
		if( Vec3::distance(position(), Vec3(0, -15, 0)) < 2.5 ){
			reset();
			return Vec3();
		}
		
		// Inherit the flocking force
		Vec3 flock = Boid::getSteeringForce(elapsedTime);
		
		// Avoid the obstacle
		Vec3 avoidObstacle = steerToAvoidObstacles (1.f, obstacles);
		
		// seek for the target
		Vec3 seek = steerForSeek(Vec3(0, -15, 0));
		
		return seek + avoidObstacle + flock * 0.3; // reduce flocking force 
	}
	
};

class ObstacleAvoidance: public ofxOpenSteerPlugin {
	
public:
	
	float radius;
	ofxOpenSteerSphereObstacle* obstacle;
	
	string name(){ return "Obstacle Avoidance"; };
	
	void setup(){
		ofxOpenSteerPlugin::setup();
		
		ofBackground(255, 0, 255);
		
		// Create the obstacle
		radius = 10;		
		obstacle = createSphereObstacle(ofPoint(), radius);
		
		for(unsigned int i=0;i<100;i++){
			ObstacleBoid* v = new ObstacleBoid();
			v->addObstacle(obstacle);
			addVehicle(v);
		}
	};
	
	void draw(){
		ofxOpenSteerPlugin::draw();
		ofSetColor(100, 100, 100, 100);
		ofSphere(ofPoint(), radius);
	}
	
	void exit(){
		ofxOpenSteerPlugin::exit();
		if(obstacle) delete obstacle;
		obstacle = NULL;
	}
	
};
