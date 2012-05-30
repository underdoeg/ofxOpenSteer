#include "ofxOpenSteerPluginCacher.h"

ofxOpenSteerPluginCacher::ofxOpenSteerPluginCacher(){
    plugin = NULL;
    fps = 0;
    frameDuration = 0;
    startFrame = 0;
    _cache = NULL;
}    
ofxOpenSteerPluginCacher::~ofxOpenSteerPluginCacher(){
    plugin = NULL;
    clear();
}

void ofxOpenSteerPluginCacher::cache(ofxOpenSteerPlugin* plugin, int frameDuration, int startFrame, float fps){
    clear();
    this->plugin = plugin;
    this->frameDuration = frameDuration;
    this->startFrame = startFrame;
    this->fps = fps;
    
    // Reset the plugin
    plugin->exit();
    plugin->setup();
    
    cout << "Caching \"" << plugin->name() << "\" for " << ofToString(frameDuration) << " frames (starting from frame " << ofToString(startFrame) << ") at " << ofToString(fps) << " FPS:" << endl;

    
    float estimated = ofGetElapsedTimef();
    
    stringstream ss;
	ss << plugin->name()
    << "_" << ofToString(frameDuration)
    << "_" << ofToString(startFrame)
    << "_" << ofToString(fps);
    filePath = ss.str();
    
    // Clear the current cache
    clear();
    
    cout << "Checking if the simulation was already saved at " << filePath << "..." << endl;
    
    fstream readFile;
    readFile.open(ofToDataPath(filePath).c_str(), ios::in | ios::binary);
    if(readFile.good()){
		cout << "Simulation found!";
        
        cacheSize = frameDuration * plugin->getVehicles().size() * OFX_OPENSTEER_PLUGIN_CACHER_UNIT_SIZE;
        _cache = new float[cacheSize];
        readFile.read((char*)_cache, sizeof(float) *  cacheSize);
        
	}else{
        
        cout << "Unable to load simulation." << endl;
        fillCache();
        
        ofFile writeFile;
        
        if(writeFile.open(filePath, ofFile::WriteOnly, true)){
            writeFile.write((char*) _cache, sizeof(float) * cacheSize );  
        }
	}
       
    cout << endl << "Caching complete! The process took: ";
    estimated = ofGetElapsedTimef() - estimated;
    if(estimated < 60.f){
        cout << ofToString(estimated) << " seconds." << endl;
    }
    else{
        cout << ofToString(estimated/60.f) << " minutes." << endl;
    }
    
    
    // "Rewind" the plugin
    update(0);
}

void ofxOpenSteerPluginCacher::clear(){
    if(_cache) delete [] _cache;
    _cache = NULL;
    cacheSize = 0;
}

void ofxOpenSteerPluginCacher::update(int frame){
    if(!plugin)return;
    int i = ofClamp(frame, 0, frameDuration - 1);
    VehicleGroup vehicles = plugin->getVehicles();
    int j = 0;
    for (VehicleIterator it = vehicles.begin(); it != vehicles.end(); it++) {
        
        (*it)->setSide(getSide(i,j));        
        (*it)->setUp(getUp(i,j));        
        (*it)->setForward(getForward(i,j));        
        (*it)->setPosition(getPosition(i,j));        
        (*it)->setSpeed(getSpeed(i,j));        
        (*it)->resetSmoothedAcceleration(getSmoothedAcceleration(i,j));
        
        j++;
    }
    
}

void ofxOpenSteerPluginCacher::fillCache(){
    cout << "Recording: " << endl;
    
    float currentTime = 0;
    float elapsedTime = 1.f/fps;
    int lastPercent = -1;
    float lastProcessTime = ofGetElapsedTimef();
    
    // "Forward" to the startFrame
    for (int i = 0; i < startFrame; i++) {
        cout << "Forwarding to start frame: (" << ofToString(i) << "/" << ofToString(startFrame) << ")" << endl;        
        currentTime += elapsedTime;
        plugin->update(currentTime, elapsedTime);
    }
    
    cacheSize = frameDuration * plugin->getVehicles().size() * OFX_OPENSTEER_PLUGIN_CACHER_UNIT_SIZE;
    _cache = new float[cacheSize];
    
    for(int i = 0; i < frameDuration; i++){
        currentTime += elapsedTime;
        plugin->update(currentTime, elapsedTime);

        int j = 0;
        VehicleGroup vehicles = plugin->getVehicles();
        for (VehicleIterator it = vehicles.begin(); it != vehicles.end(); it++) {
            
            setSide(i, j, (*it)->side());
            setUp(i, j, (*it)->up());
            setForward(i, j, (*it)->forward());
            setPosition(i, j, (*it)->position());
            setSmoothedAcceleration(i, j, (*it)->smoothedAcceleration());
            setSpeed(i, j, (*it)->speed());
            
            j ++;
        }
        
        int percent = ((float)i/(float)(frameDuration-1)) * 100;
        if (percent != lastPercent) {
            lastPercent = percent;
            float currentProcessTime = ofGetElapsedTimef();
            float dt = currentProcessTime - lastProcessTime;
            lastProcessTime = currentProcessTime;
            float estimated = dt * (float)(frameDuration-1 - i);
                        
            cout << ofToString(percent) << "% - Estimated time remaining: ";
            if(estimated < 60.f){
               cout << ofToString(estimated) << " seconds" << endl;
            }
            else{
                cout << ofToString(estimated/60.f) << " minutes" << endl;
            }
            
        }        
    }
}

void ofxOpenSteerPluginCacher::setVec3(int frame, int unit, int offset, Vec3 value){
    int valueStartIndex = frame * plugin->getVehicles().size() * OFX_OPENSTEER_PLUGIN_CACHER_UNIT_SIZE
    + unit * OFX_OPENSTEER_PLUGIN_CACHER_UNIT_SIZE
    + offset;

    _cache[valueStartIndex] = value.x;
    _cache[valueStartIndex + 1] = value.y;
    _cache[valueStartIndex + 2] = value.z;
}
void ofxOpenSteerPluginCacher::setFloat(int frame, int unit, int offset, float value){
    int valueStartIndex = frame * plugin->getVehicles().size() * OFX_OPENSTEER_PLUGIN_CACHER_UNIT_SIZE
    + unit * OFX_OPENSTEER_PLUGIN_CACHER_UNIT_SIZE
    + offset;
    
    _cache[valueStartIndex] = value;
}

void ofxOpenSteerPluginCacher::setSide(int frame, int unit, Vec3 value){
    setVec3(frame, unit, OFX_OPENSTEER_PLUGIN_CACHER_SIDE_OFFSET, value);
}
void ofxOpenSteerPluginCacher::setUp(int frame, int unit, Vec3 value){
    setVec3(frame, unit, OFX_OPENSTEER_PLUGIN_CACHER_UP_OFFSET, value);
}
void ofxOpenSteerPluginCacher::setForward(int frame, int unit, Vec3 value){
    setVec3(frame, unit, OFX_OPENSTEER_PLUGIN_CACHER_FORWARD_OFFSET, value);
}
void ofxOpenSteerPluginCacher::setPosition(int frame, int unit, Vec3 value){
    setVec3(frame, unit, OFX_OPENSTEER_PLUGIN_CACHER_POSITION_OFFSET, value);
}
void ofxOpenSteerPluginCacher::setSmoothedAcceleration(int frame, int unit, Vec3 value){
    setVec3(frame, unit, OFX_OPENSTEER_PLUGIN_CACHER_SMOOTHED_ACCELERATION_OFFSET, value);
}
void ofxOpenSteerPluginCacher::setSpeed(int frame, int unit, float value){
    setFloat(frame, unit, OFX_OPENSTEER_PLUGIN_CACHER_SPEED_OFFSET, value);
}

Vec3 ofxOpenSteerPluginCacher::getVec3(int frame, int unit, int offset){
    int valueStartIndex = frame * plugin->getVehicles().size() * OFX_OPENSTEER_PLUGIN_CACHER_UNIT_SIZE
    + unit * OFX_OPENSTEER_PLUGIN_CACHER_UNIT_SIZE
    + offset;
    
    return Vec3(_cache[valueStartIndex], _cache[valueStartIndex + 1], _cache[valueStartIndex + 2]);
}
float ofxOpenSteerPluginCacher::getFloat(int frame, int unit, int offset){
    int valueStartIndex = frame * plugin->getVehicles().size() * OFX_OPENSTEER_PLUGIN_CACHER_UNIT_SIZE
    + unit * OFX_OPENSTEER_PLUGIN_CACHER_UNIT_SIZE
    + offset;
    
    return _cache[valueStartIndex];
}

Vec3 ofxOpenSteerPluginCacher::getSide(int frame, int unit){
    return getVec3(frame, unit, OFX_OPENSTEER_PLUGIN_CACHER_SIDE_OFFSET);
}
Vec3 ofxOpenSteerPluginCacher::getUp(int frame, int unit){
    return getVec3(frame, unit, OFX_OPENSTEER_PLUGIN_CACHER_UP_OFFSET);
}
Vec3 ofxOpenSteerPluginCacher::getForward(int frame, int unit){
    return getVec3(frame, unit, OFX_OPENSTEER_PLUGIN_CACHER_FORWARD_OFFSET);
}
Vec3 ofxOpenSteerPluginCacher::getPosition(int frame, int unit){
    return getVec3(frame, unit, OFX_OPENSTEER_PLUGIN_CACHER_POSITION_OFFSET);
}
Vec3 ofxOpenSteerPluginCacher::getSmoothedAcceleration(int frame, int unit){
    return getVec3(frame, unit, OFX_OPENSTEER_PLUGIN_CACHER_SMOOTHED_ACCELERATION_OFFSET);
}
float ofxOpenSteerPluginCacher::getSpeed(int frame, int unit){
    return getFloat(frame, unit, OFX_OPENSTEER_PLUGIN_CACHER_SPEED_OFFSET);
}