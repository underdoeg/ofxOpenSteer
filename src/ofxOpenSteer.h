#ifndef OFXOPENSTEER_H
#define OFXOPENSTEER_H

#include "PolylineSegmentedPathwaySegmentRadii.h"
#include "ofxVehicle.h"
#include "ofMain.h"
#include "ofxOpenSteerTypes.h"

class ofxOpenSteer {

public:
	ofxOpenSteer();
	~ofxOpenSteer();
	ofxVehicle* createVehicle();
	void update();
	void update(float currentTime, float elapsedTime);
	void draw();
	void createProximityDatabase();
	static ofxOpenSteerPath* convertPolyLine(std::vector<ofPoint> points, float radius=3);
	static ofxOpenSteerSphereObstacle* createSphereObstacle(ofPoint center, float radius);
	static ofxOpenSteerRectObstacle* createRectObstacle(ofRectangle r);
	
	static ofVec3f convertOpenSteerVec(OpenSteer::Vec3 v){
		return ofVec3f(v.x, v.y, v.z);
	}
	
	static OpenSteer::Vec3 convertOfVec(ofPoint v){
		return OpenSteer::Vec3(v.x, v.y, v.z);
	}
private:
	vehicleList vehicles;
	float lastTime;
	ProximityDatabase* proximityDB;
	static std::vector<ofxOpenSteerSphereObstacle*> sphereObstacles;
	static std::vector<ofxOpenSteerRectObstacle*> rectObstacles;
};

#endif // OFXOPENSTEER_H
