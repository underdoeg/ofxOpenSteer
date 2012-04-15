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
    
   static ofxOpenSteerPath* createPath(std::vector<ofPoint> points, float radius=3){
        Vec3* pathPoints = new Vec3 [points.size()];
        for(unsigned int i = 0; i<points.size(); i++) {
            pathPoints[i] = toOpenSteer(points[i]);
        }
        return new ofxOpenSteerPath (points.size(),pathPoints,radius,false);
    };
	static ofxOpenSteerPath* createPath(ofPolyline points, float radius=3){
        Vec3* pathPoints = new Vec3 [points.size()];
        for(unsigned int i = 0; i<points.size(); i++) {
            pathPoints[i] = toOpenSteer(points[i]);
        }
        return new ofxOpenSteerPath (points.size(),pathPoints,radius,false);
    };
    
    static ofxOpenSteerSphereObstacle* createSphereObstacle(ofPoint center, float radius){
        ofxOpenSteerSphereObstacle* ret = new ofxOpenSteerSphereObstacle();
        ret->center = toOpenSteer(center);
        ret->radius=radius;
        return ret;
    }
	//static ofxOpenSteerRectObstacle* createRectObstacle(ofRectangle r);  

};