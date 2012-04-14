//
//  Vehicle.h
//  emptyExample
//
//  Created by Paulo Barcelos on 4/12/12.
//  Copyright (c) 2012 Oakwood. All rights reserved.
//

#pragma once

#include "ofxOpenSteer.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

class Vehicle: public ofxOpenSteerVehicle{

   public:
    
    Vehicle(); 
    
    void reset();    
    void draw();    
    Vec3 getSteeringDirection(const float elapsedTime);
    
    Vec3 target;    
    bool isSeeking;
};