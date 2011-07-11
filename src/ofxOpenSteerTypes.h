#include "SteerLibrary.h"
#include "Proximity.h"
#include "SimpleVehicle.h"
#include "PolylineSegmentedPathwaySingleRadius.h"

class ofxVehicle;

typedef std::vector<ofxVehicle*> vehicleList;
typedef vehicleList::iterator vehicleIterator;
typedef OpenSteer::AbstractProximityDatabase<OpenSteer::AbstractVehicle*> ProximityDatabase;
typedef OpenSteer::AbstractTokenForProximityDatabase<OpenSteer::AbstractVehicle*> ProximityToken;
typedef OpenSteer::PolylineSegmentedPathwaySingleRadius ofxOpenSteerPath;
typedef OpenSteer::Obstacle ofxOpenSteerObstacle;
typedef OpenSteer::SphereObstacle ofxOpenSteerSphereObstacle;
typedef OpenSteer::RectangleObstacle ofxOpenSteerRectObstacle;