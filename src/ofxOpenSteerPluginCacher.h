/**
* This is totally experimental! Should work fine for  plugins where
* the number of vehicles is constant during the whole simulation and
* there aren't external inputs. Use at your own risk!
**/ 

#pragma once

#include "ofxOpenSteer.h"

using namespace OpenSteer;
using namespace ofxOpenSteer;

#define OFX_OPENSTEER_PLUGIN_CACHER_UNIT_SIZE 16
#define OFX_OPENSTEER_PLUGIN_CACHER_SIDE_OFFSET 0
#define OFX_OPENSTEER_PLUGIN_CACHER_UP_OFFSET 3
#define OFX_OPENSTEER_PLUGIN_CACHER_FORWARD_OFFSET 6
#define OFX_OPENSTEER_PLUGIN_CACHER_POSITION_OFFSET 9
#define OFX_OPENSTEER_PLUGIN_CACHER_SMOOTHED_ACCELERATION_OFFSET 12
#define OFX_OPENSTEER_PLUGIN_CACHER_SPEED_OFFSET 15

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
    
    void setVec3(int frame, int unit, int offset, Vec3 value);
    void setFloat(int frame, int unit, int offset, float value);
    void setSide(int frame, int unit, Vec3 value);
    void setUp(int frame, int unit, Vec3 value);
    void setForward(int frame, int unit, Vec3 value);
    void setPosition(int frame, int unit, Vec3 value);
    void setSmoothedAcceleration(int frame, int unit, Vec3 value);
    void setSpeed(int frame, int unit, float value);
    
    Vec3 getVec3(int frame, int unit, int offset);
    float getFloat(int frame, int unit, int offset);
    Vec3 getSide(int frame, int unit);
    Vec3 getUp(int frame, int unit);
    Vec3 getForward(int frame, int unit);
    Vec3 getPosition(int frame, int unit);
    Vec3 getSmoothedAcceleration(int frame, int unit);
    float getSpeed(int frame, int unit);    
    
    void fillCache();
    unsigned int cacheSize;
    float* _cache;
    
	ofxOpenSteerPlugin* plugin;
    int frameDuration;
    int startFrame;
    float fps;
    
    string filePath;
};