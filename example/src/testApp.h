#pragma once

#include "ofMain.h"
#include "Flocking.h"
#include "PathFollowing.h"
#include "ObstacleAvoidance.h"
#include "PursuitAndEvade.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed( int key );
		void setSimulation( int simulationIndex );
    
		ofEasyCam cam;
	
        Flocking flocking;
		PathFollowing pathFollowing;
		ObstacleAvoidance obstacleAvoidance;
        PursuitAndEvade pursuitAndEvade;
	
		vector<ofxOpenSteerPlugin*> simulations;
		ofxOpenSteerPlugin* currentSimulation;
		int simulationIndex;
};
