#pragma once

#include "ofxOpenSteer.h"

namespace ofxOpenSteer {
    
    using namespace OpenSteer;
    
	static ofVec3f toOf(Vec3 v){
		return ofVec3f(v.x, v.y, v.z);
	};
	
	static Vec3 toOpenSteer(ofPoint v){
		return OpenSteer::Vec3(v.x, v.y, v.z);
	};
    
    static SteerPath* createPath(std::vector<ofPoint> points, float radius = 3){
        Vec3* pathPoints = new Vec3 [points.size()];
        for(unsigned int i = 0; i<points.size(); i++) {
            pathPoints[i] = toOpenSteer(points[i]);
        }
        return new SteerPath (points.size(),pathPoints,radius,false);
    };
	static SteerPath* createPath(ofPolyline polyline, float radius = 3){
        return createPath(polyline.getVertices(), radius);
    };
    
    static ProximityDatabase* createProximityDatabase( ProximityDatabaseSettings pdSettings ){
        const Vec3 divisions (pdSettings.divX, pdSettings.divY, pdSettings.divZ);
        const Vec3 dimensions (pdSettings.diameter, pdSettings.diameter, pdSettings.diameter);
        typedef LQProximityDatabase<AbstractVehicle*> LQPDAV;
        return new LQPDAV (pdSettings.center, dimensions, divisions);
    };
    static ProximityDatabase* createProximityDatabase(){
        ProximityDatabaseSettings pdSettings;
        return createProximityDatabase(pdSettings);
    };
    
    static ProximityToken* allocateProximityToken(ProximityDatabase* pd, AbstractVehicle* v){
        return pd->allocateToken (v);
    };
};