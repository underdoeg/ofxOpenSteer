/**
* This is totally experimental! Should work fine for  plugins where
* the number of vehicles is constant during the whole simulation and
* there aren't external inputs. Use at your own risk!
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

class ofxOpenSteerPluginCacherSettings{
    public:
    ofxOpenSteerPluginCacherSettings(){
        side = true;
        up = true;
        forward = true;
        position = true;
        smoothedAcceleration = true;
        speed = true;
    }
    bool side;
    bool up;
    bool forward;
    bool position;
    bool smoothedAcceleration;
    bool speed;
};

struct ofxOpenSteerPluginCacherFrame{
    vector<ofxOpenSteerPluginCacherFrameUnit*> units;
};


class ofxOpenSteerPlugin; //forward declaration
class ofxOpenSteerPluginCacher {

public:
	ofxOpenSteerPluginCacher();
	~ofxOpenSteerPluginCacher();
    
    
	void cache(ofxOpenSteerPlugin* plugin, int frameDuration, int startFrame = 0, float = 60.f, ofxOpenSteerPluginCacherSettings settings = ofxOpenSteerPluginCacherSettings());
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
    ofFile file;
    string filePath;
    
    ofxOpenSteerPluginCacherSettings settings;
};