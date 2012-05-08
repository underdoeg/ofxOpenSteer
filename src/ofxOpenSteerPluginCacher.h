/**
* This is totally experimental! Should work fine for  plugins where
* the number of vehicles is constant during the whole simulation and
* there aren't external inputs. Use at your own risk!
**/ 

#pragma once

#include "ofxOpenSteer.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

struct ofxOpenSteerPluginCacherUnit{
    float side[3];
    float up[3];
    float forward[3];
    float position[3];
    float smoothedAcceleration[3];
    float speed;
};

struct ofxOpenSteerPluginCacherFrame{
    int size;
    ofxOpenSteerPluginCacherUnit* units;
};

struct ofxOpenSteerPluginCacherCache{
    int size;
    ofxOpenSteerPluginCacherFrame* frames;
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
    
    void fillCache();
    
    ofxOpenSteerPluginCacherCache _cache;
	ofxOpenSteerPlugin* plugin;
    int frameDuration;
    int startFrame;
    float fps;
    
    string filePath;
};