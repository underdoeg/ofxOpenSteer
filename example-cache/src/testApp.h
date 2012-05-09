#pragma once

#include "ofMain.h"
#include "ofxOpenSteer.h"
#include "ObstacleAvoidance.h"


struct Unit{
    float number[3];
};

struct Frame{
    Unit* units;
    int size;
};

struct Cache{
    Frame* frames;
    int size;
};


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
