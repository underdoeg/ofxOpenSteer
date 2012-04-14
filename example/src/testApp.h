#pragma once

#include "ofMain.h"
#include "Plugin.h"
#include "Vehicle.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        Plugin plugin;
};
