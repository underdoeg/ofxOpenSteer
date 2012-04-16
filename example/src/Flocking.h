#pragma once

#include "ofxOpenSteer.h"
#include "Boid.h"


using namespace OpenSteer;
using namespace ofxOpenSteer;

class Flocking: public ofxOpenSteerPlugin {

    public:
		string name(){ return "Flocking"; };
        
		void setup(){
			ofxOpenSteerPlugin::setup();
			
			ofBackground(255, 255, 0);
			
            for(unsigned int i=0;i<300;i++){
                Boid* v = new Boid();
                v->reset();
                addVehicle(v);
            }
        };

};