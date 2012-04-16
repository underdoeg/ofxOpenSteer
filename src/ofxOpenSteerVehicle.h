#pragma once

#include "ofxOpenSteer.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

class ofxOpenSteerVehicle: public OpenSteer::SimpleVehicle{

    public:
        ofxOpenSteerVehicle();
        ~ofxOpenSteerVehicle();
        
        virtual void reset();
        virtual void update();
        virtual void update(float curTime, float elapsedTime);
        virtual void draw();    
        virtual Vec3 getSteeringForce(const float elapsedTime);
        
        virtual void setProximityDatabase(ProximityDatabase* db);
        virtual ProximityDatabase* getProximityDatabase();
        virtual ProximityToken* getProximityToken();
        
        virtual void setPath(SteerPath* path);
        virtual SteerPath* getPath();
        virtual void unsetPath();
            
        virtual void addObstacle(Obstacle* o);
        virtual void removeObstacle(Obstacle* o);
        virtual ObstacleGroup getObstacles();
        
    protected:
        Color color; // XXX Just a convenience in case we want to use the default drawing method
        static AVGroup neighbors;
        ProximityDatabase* proximityDB;
        ProximityToken* proximityToken;
        SteerPath* path;
        ObstacleGroup obstacles;
};