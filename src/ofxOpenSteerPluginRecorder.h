/**
* This is totally experimental! Should work fine for simple plugins
* where the number of vehicles is constant during the whole simualtion.
* Use at your own risk!
**/ 

#pragma once

#include "ofxOpenSteer.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

struct ofxOpenSteerPluginRecorderFrameUnit{
    Vec3 side;
    Vec3 up;
    Vec3 forward;
    Vec3 position;
    Vec3 smoothedAcceleration;
    float speed;
};

struct ofxOpenSteerPluginRecorderFrame{
    vector<ofxOpenSteerPluginRecorderFrameUnit*> units;
};

class ofxOpenSteerPlugin; //forward declaration
class ofxOpenSteerPluginRecorder {

public:
	ofxOpenSteerPluginRecorder();
	~ofxOpenSteerPluginRecorder();
	
	void record(ofxOpenSteerPlugin* plugin, int frameDuration, float = 60.f);
    void clear(); 
    void update(int frame);
    
    int getFrameDuration(){ return frameDuration; };
    float getFPS(){ return fps; };

protected:
    
    void loadSettings();
    void saveSettings();
    
    vector<ofxOpenSteerPluginRecorderFrame*> frames;
	ofxOpenSteerPlugin* plugin;
    int frameDuration;
    float fps;
    ofFile settings;
    string settingsPath;
};