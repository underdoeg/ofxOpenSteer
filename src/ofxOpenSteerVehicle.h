#pragma once

#include "ofxOpenSteer.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

class ofxOpenSteerVehicle: public SimpleVehicle{

    public:
        ofxOpenSteerVehicle();
        ~ofxOpenSteerVehicle();
        
        virtual void reset();
        virtual void update();
        virtual void update(float curTime, float elapsedTime);
        virtual void draw();    
        virtual Vec3 getSteeringForce(const float elapsedTime);
		ofVec3f getPosition();

    protected:
        Color color; // XXX Just a convenience in case we want to use the default drawing method
        static AVGroup neighbors;
};