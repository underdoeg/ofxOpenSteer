#include "ofxOpenSteerPluginCacher.h"

ofxOpenSteerPluginCacher::ofxOpenSteerPluginCacher(){
    plugin = NULL;
    fps = 0;
    frameDuration = 0;
    startFrame = 0;
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
    
    cout << "Caching \"" << plugin->name() << "\" for " << ofToString(frameDuration) << " frames (starting from frame " << ofToString(startFrame) << ") at " << ofToString(fps) << " FPS," << endl;

    
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
    ofFile readFile;
    if( readFile.open(filePath, ofFile::ReadOnly, true) ){
		cout << "Simulation found!" << endl;
        
        memcpy( &_cache, readFile.readToBuffer().getBinaryBuffer(), readFile.getSize() );

        for (int i = 0; i < _cache.size; i++) {
            cout << "Frame size" << _cache.frames[i].size << endl;
        }
        
	}else{
        
        cout << "Unable to load simulation." << endl;
        fillCache();
        
        ofFile writeFile;
        
        if(writeFile.open(filePath, ofFile::WriteOnly, true)){
            writeFile.write((char*) &_cache, 
                       sizeof(ofxOpenSteerPluginCacherCache)
                       + sizeof(ofxOpenSteerPluginCacherFrame) * 1000
                       + sizeof(ofxOpenSteerPluginCacherUnit) * 100000 * 1000 );  
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
    _cache = ofxOpenSteerPluginCacherCache();
}

void ofxOpenSteerPluginCacher::update(int frame){
    if(!plugin)return;
    int i = ofClamp(frame, 0, frameDuration - 1);
    VehicleGroup vehicles = plugin->getVehicles();
    int j = 0;
    for (VehicleIterator it = vehicles.begin(); it != vehicles.end(); it++) {
        
        (*it)->setSide(Vec3(_cache.frames[i].units[j].side[0],
                            _cache.frames[i].units[j].side[1],
                            _cache.frames[i].units[j].side[2]));
        
        (*it)->setUp(Vec3(_cache.frames[i].units[j].up[0],
                          _cache.frames[i].units[j].up[1],
                          _cache.frames[i].units[j].up[2]));
        
        (*it)->setForward(Vec3(_cache.frames[i].units[j].forward[0],
                               _cache.frames[i].units[j].forward[1],
                               _cache.frames[i].units[j].forward[2]));
        
        (*it)->setPosition(Vec3(_cache.frames[i].units[j].position[0],
                                _cache.frames[i].units[j].position[1],
                                _cache.frames[i].units[j].position[2]));
        
        (*it)->setSpeed(_cache.frames[i].units[j].speed);
        
        (*it)->resetSmoothedAcceleration(Vec3(_cache.frames[i].units[j].side[0],
                                              _cache.frames[i].units[j].side[1],
                                              _cache.frames[i].units[j].side[2]));
        
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
    
    _cache.size = frameDuration;
    _cache.frames = new ofxOpenSteerPluginCacherFrame[_cache.size];
    
    for(int i = 0; i < _cache.size; i++){
        currentTime += elapsedTime;
        plugin->update(currentTime, elapsedTime);

        _cache.frames[i].size = plugin->getVehicles().size();
        _cache.frames[i].units = new ofxOpenSteerPluginCacherUnit[_cache.frames[i].size];
        
        int j = 0;
        VehicleGroup vehicles = plugin->getVehicles();
        for (VehicleIterator it = vehicles.begin(); it != vehicles.end(); it++) {
            
            _cache.frames[i].units[j].side[0] = (*it)->side().x;
            _cache.frames[i].units[j].side[1] = (*it)->side().y;
            _cache.frames[i].units[j].side[2] = (*it)->side().z;
            
            _cache.frames[i].units[j].up[0] = (*it)->up().x;
            _cache.frames[i].units[j].up[1] = (*it)->up().y;
            _cache.frames[i].units[j].up[2] = (*it)->up().z;
            
            _cache.frames[i].units[j].forward[0] = (*it)->forward().x;
            _cache.frames[i].units[j].forward[1] = (*it)->forward().y;
            _cache.frames[i].units[j].forward[2] = (*it)->forward().z;
            
            _cache.frames[i].units[j].position[0] = (*it)->position().x;
            _cache.frames[i].units[j].position[1] = (*it)->position().y;
            _cache.frames[i].units[j].position[2] = (*it)->position().z;
            
            _cache.frames[i].units[j].smoothedAcceleration[0] = (*it)->smoothedAcceleration().x;
            _cache.frames[i].units[j].smoothedAcceleration[1] = (*it)->smoothedAcceleration().y;
            _cache.frames[i].units[j].smoothedAcceleration[2] = (*it)->smoothedAcceleration().z;
            
            _cache.frames[i].units[j].speed = (*it)->speed();
            
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