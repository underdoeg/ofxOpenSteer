#include "SteerLibrary.h"
#include "Proximity.h"
#include "SimpleVehicle.h"
#include "PolylineSegmentedPathwaySingleRadius.h"

using namespace OpenSteer;

class ofxOpenSteerVehicle;

typedef std::vector<ofxOpenSteerVehicle*> vehicleList;
typedef vehicleList::iterator vehicleIterator;
typedef OpenSteer::AbstractProximityDatabase<OpenSteer::AbstractVehicle*> ProximityDatabase;
typedef OpenSteer::AbstractTokenForProximityDatabase<OpenSteer::AbstractVehicle*> ProximityToken;
typedef OpenSteer::PolylineSegmentedPathwaySingleRadius ofxOpenSteerPath;

struct ofxOpenSteerProximityDatabaseSettings{
	Vec3 center;
	float divX;
	float divY;
	float divZ;
	float diameter;
};