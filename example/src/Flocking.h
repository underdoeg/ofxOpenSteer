#pragma once

#include "ofxOpenSteer.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

class Boid: public ofxOpenSteerVehicle{
	
public:
    
    Boid(){
		reset();
	}; 
    
    void reset(){
		// reset the vehicle
		ofxOpenSteerVehicle::reset ();
		
		// steering force is clipped to this magnitude
		setMaxForce (27);
		
		// velocity is clipped to this magnitude
		setMaxSpeed (9);
		
		// initial slow speed
		setSpeed (maxSpeed() * 0.3f);
		
		// randomize initial orientation
		regenerateOrthonormalBasisUF (RandomUnitVector ());
		
		// randomize initial position
		setPosition (RandomVectorInUnitRadiusSphere() * 20);
		
		// notify proximity database that our position has changed
		if(proximityToken) proximityToken->updateForNewPosition (position());
	};
	
    Vec3 getSteeringDirection(const float elapsedTime){
		const float separationRadius =  5.0f;
		const float separationAngle  = -0.707f;
		const float separationWeight =  12.0f;
		
		const float alignmentRadius = 7.5f;
		const float alignmentAngle  = 0.7f;
		const float alignmentWeight = 8.0f;
		
		const float cohesionRadius = 50.0f;
		const float cohesionAngle  = -0.15f;
		const float cohesionWeight = 8.0f;
		
		const float maxRadius = maxXXX (separationRadius,
										maxXXX (alignmentRadius,
												cohesionRadius));
		
		// find all flockmates within maxRadius using proximity database
		neighbors.clear();
		proximityToken->findNeighbors (position(), maxRadius, neighbors);
		
		
		// determine each of the three component behaviors of flocking
		const Vec3 separation = steerForSeparation (separationRadius,
													separationAngle,
													neighbors);
		const Vec3 alignment  = steerForAlignment  (alignmentRadius,
													alignmentAngle,
													neighbors);
		const Vec3 cohesion   = steerForCohesion   (cohesionRadius,
													cohesionAngle,
													neighbors);
		
		// apply weights to components (save in variables for annotation)
		const Vec3 separationW = separation * separationWeight;
		const Vec3 alignmentW = alignment * alignmentWeight;
		const Vec3 cohesionW = cohesion * cohesionWeight;
		
		return separationW + alignmentW + cohesionW;	
	};
};

class Flocking: public ofxOpenSteerPlugin {

    public:
		string name(){ return "Flocking"; };
        void setup(){       
			ofBackground(255, 255, 0);
			
            for(unsigned int i=0;i<300;i++){
                Boid* v = new Boid();
                addVehicle(v);
            }
        };

};