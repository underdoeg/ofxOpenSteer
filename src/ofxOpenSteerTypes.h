#include "SteerLibrary.h"
#include "Proximity.h"
#include "SimpleVehicle.h"
#include "PolylineSegmentedPathwaySingleRadius.h"

using namespace OpenSteer;

class ofxOpenSteerVehicle;

namespace ofxOpenSteer{
    typedef std::vector<ofxOpenSteerVehicle*> VehicleGroup;
    typedef VehicleGroup::iterator VehicleIterator;
    typedef OpenSteer::AbstractProximityDatabase<OpenSteer::AbstractVehicle*> ProximityDatabase;
    typedef OpenSteer::AbstractTokenForProximityDatabase<OpenSteer::AbstractVehicle*> ProximityToken;
    typedef OpenSteer::PolylineSegmentedPathwaySingleRadius SteerPath; // XXX Just a convenience for the long name, but is it really necessary?
    
    struct ProximityDatabaseSettings{
        ProximityDatabaseSettings(){
            center = Vec3();
            divX = 20.f;
            divY = 20.f;
            divZ = 20.f;
            diameter = ofGetWidth();
        };
        Vec3 center;
        float divX;
        float divY;
        float divZ;
        float diameter;
    };
};

