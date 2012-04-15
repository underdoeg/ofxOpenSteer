#pragma once

#include "ofMain.h"
#include "Flocking.h"
class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed( int key );
    
		ofEasyCam cam;
	
        Flocking flocking;
};
