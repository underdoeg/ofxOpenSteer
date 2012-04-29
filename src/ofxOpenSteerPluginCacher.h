/**
* This is totally experimental! Should work fine for simple plugins
* where the number of vehicles is constant during the whole simulation.
* Use at your own risk!
**/ 

#pragma once

#include "ofxOpenSteer.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

struct ofxOpenSteerPluginCacherFrameUnit{
    Vec3 side;
    Vec3 up;
    Vec3 forward;
    Vec3 position;
    Vec3 smoothedAcceleration;
    float speed;
};

struct ofxOpenSteerPluginCacherFrame{
    vector<ofxOpenSteerPluginCacherFrameUnit*> units;
};

class ofxOpenSteerPlugin; //forward declaration
class ofxOpenSteerPluginCacher {

public:
	ofxOpenSteerPluginCacher();
	~ofxOpenSteerPluginCacher();
	
	void cache(ofxOpenSteerPlugin* plugin, int frameDuration, int startFrame = 0, float = 60.f);
    void clear(); 
    void update(int frame);
    
    int getFrameDuration(){ return frameDuration; };
    float getFPS(){ return fps; };

protected:
    
    void loadSettings();
    void saveSettings();
    
    vector<ofxOpenSteerPluginCacherFrame*> frames;
	ofxOpenSteerPlugin* plugin;
    int frameDuration;
    int startFrame;
    float fps;
    ofFile settings;
    string settingsPath;
};