#include "Vehicle.h"

Vehicle::Vehicle(){
    reset();
    target.x = ofGetWidth()/2;
    target.y = ofGetHeight()/2;
    target.z = 0;
    isSeeking = true;
};
    
void Vehicle::reset(){
    ofxOpenSteerVehicle::reset();       

    setMaxSpeed(400);
    setMaxForce(10000); 
    
    setMass(ofRandom(1,5));      
    setSpeed(ofRandom(maxSpeed()));    
    setRadius(ofRandom(5, 9));
    
    setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
    
    setUp (toOpenSteer(ofPoint(0,0,0)));
    setForward (toOpenSteer(ofPoint(ofRandom(-1,1),ofRandom(-1,1),0)));
};

void Vehicle::draw()
{    
    ofSetColor(255);
    ofCircle(toOf(position()), radius());
    Vec3 f = forward() * speed();
    Vec3 p = position();
    Vec3 fp = f + p;
    ofSetColor(0);
    ofLine(p.x, p.y, p.z, fp.x, fp.y, fp.z);
}


Vec3 Vehicle::getSteeringDirection(const float elapsedTime)
{
    Vec3 steeringForce = Vec3(0,0,0);
    
    
    // Wander around!
    //steeringForce += steerForWander (elapsedTime);

    // Seek or flee target
    if(isSeeking){
        steeringForce += steerForSeek(target);
    }
    else{
        steeringForce += steerForFlee(target);
    }
    steeringForce.z = 0; // 2d only
    
    return steeringForce;
}
