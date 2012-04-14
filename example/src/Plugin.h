#pragma once

#include "ofxOpenSteer.h"
#include "Vehicle.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

class Plugin: public ofxOpenSteerPlugin {

    public:
        void setup(){
            ofRegisterMouseEvents(this);
            
            for(unsigned int i=0;i<300;i++){
                Vehicle* v = new Vehicle();
                addVehicle((ofxOpenSteerVehicle*)v);
                //v->setPosition(ofGetWidth()*ofRandom(1),ofGetHeight()*ofRandom(1));
            }
        };
    
    void mouseMoved(ofMouseEventArgs& args){
        for (vehicleIterator i = vehicles.begin(); i != vehicles.end(); i++) {
            Vehicle* v = (Vehicle*)(*i);
            v->target.x = args.x;
            v->target.y = args.y;
        }
    };
    void mouseDragged(ofMouseEventArgs& args){};
    void mousePressed(ofMouseEventArgs& args){
        for (vehicleIterator i = vehicles.begin(); i != vehicles.end(); i++) {
            Vehicle* v = (Vehicle*)(*i);
            v->isSeeking = true;
        }
    };
    void mouseReleased(ofMouseEventArgs& args){
        for (vehicleIterator i = vehicles.begin(); i != vehicles.end(); i++) {
            Vehicle* v = (Vehicle*)(*i);
            v->isSeeking = false;
        }
    };
};