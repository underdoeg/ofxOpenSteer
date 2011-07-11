#ifndef OFXOPENSTEERVEHICLE_H
#define OFXOPENSTEERVEHICLE_H

#include "SteerLibrary.h"
#include "ofMain.h"
#include "ofxOpenSteerTypes.h"

class ofxVehicle: public OpenSteer::SimpleVehicle
{

public:
	ofxVehicle();
	~ofxVehicle();
	void followPath(ofxOpenSteerPath* path);
	void update();
	void setProximityDatabase(ProximityDatabase* db);
	bool isFollowingPath;
	bool hasObstacles;
	float setRadius(float r);
	float getRadius();
	
	void reset();
	void update(float curTime, float elapsedTime);
	void setWandering(bool wand);
	virtual void onReset();

	void setPosition(float x, float y, float z=0);
	virtual OpenSteer::Vec3 getSteeringDirection(const float elapsedTime);

	ofVec3f getPosition();

	virtual void draw();
	
	virtual void addObstacle(ofxOpenSteerObstacle* o);
	
private:
	static OpenSteer::AVGroup neighbors;
	ProximityDatabase* proximityDB;
	ProximityToken* proximityToken;
	ofxOpenSteerPath* path;
	bool isWandering;
	OpenSteer::ObstacleGroup obstacles;
};

#endif // OFXOPENSTEERVEHICLE_H
