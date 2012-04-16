#pragma once

#include "ofxOpenSteer.h"
#include "Boid.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

// Extend the flocking boid adding obstacle avoidance behaviour
class ObstacleBoid: public Boid {
    public:
    
        // Origin and target will be set by the ObstacleAvoidance plugin
        Vec3* origin;
        Vec3* target;        
        
        void reset(){
            // reset the vehicle
            Boid::reset ();
                    
            // Set to origin, and randomize slightly
            setPosition (*origin - RandomVectorInUnitRadiusSphere());
            
            // notify proximity database that our position has changed
            if(proximityToken) proximityToken->updateForNewPosition (position());
        };
        
        Vec3 getSteeringForce(const float elapsedTime){
            // reset if close enought to target
            if( Vec3::distance(position(), *target) < 2 ){
                reset();
                return Vec3();
            }
            
            // Inherit the flocking force
            Vec3 flock = Boid::getSteeringForce(elapsedTime);
            
            // Avoid the obstacle
            Vec3 avoidObstacle = steerToAvoidObstacles (1.f, obstacles);
            
            // seek for the target
            Vec3 seek = steerForSeek(*target);
            
            return seek + avoidObstacle + flock * 0.2; // reduce flocking force 
        }

};

class ObstacleAvoidance: public ofxOpenSteerPlugin {
	
public:
    Vec3 origin;
    Vec3 target;
	
	float radius;
	SphereObstacle* obstacle;
	
	string name(){ return "Obstacle Avoidance"; };
	
	void setup(){
		ofxOpenSteerPlugin::setup();
		
		ofBackground(255, 0, 255);
        
        // Origin and target points
        origin = Vec3(0,20,0);
        target = Vec3(0,-20,0);
		
		// Create the obstacle
		radius = 10;		
		obstacle = new SphereObstacle(radius, Vec3::zero);
		
		for(unsigned int i=0;i<100;i++){
			ObstacleBoid* v = new ObstacleBoid();
            v->origin = &origin;
            v->target = &target;
			v->addObstacle(obstacle);
            v->reset();
			addVehicle(v);
		}
	};
	
	void draw(){
		ofxOpenSteerPlugin::draw();
		ofSetColor(100, 100, 100, 100);
		ofSphere(ofPoint(), radius);
        
        ofSetColor(255, 255, 255, 100);
        ofSphere(toOf(origin), 2);
        ofSphere(toOf(target), 2);
	}
	
	void exit(){
		ofxOpenSteerPlugin::exit();
		if(obstacle) delete obstacle;
		obstacle = NULL;
	}
	
};
