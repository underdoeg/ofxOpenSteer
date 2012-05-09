#pragma once

#include "ofMain.h"
#include "ofxOpenSteer.h"
#include "ObstacleAvoidance.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed( int key );
    
		ofEasyCam cam;
	
		ObstacleAvoidance plugin;
        ofxOpenSteerPluginCacher cacher;
        int currentFrame;
};
