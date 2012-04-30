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

void ofxOpenSteerPluginCacher::cache(ofxOpenSteerPlugin* plugin, int frameDuration, int startFrame, float fps, ofxOpenSteerPluginCacherSettings settings){
    clear();
    this->plugin = plugin;
    this->frameDuration = frameDuration;
    this->startFrame = startFrame;
    this->fps = fps;
    
    this->settings.up = settings.up;
    this->settings.side = settings.side;
    this->settings.forward = settings.forward;
    this->settings.position = settings.position;
    this->settings.smoothedAcceleration = settings.smoothedAcceleration;
    this->settings.speed = settings.speed;
    
    // Reset the plugin
    plugin->exit();
    plugin->setup();
    
    cout << "Caching \"" << plugin->name() << "\" for " << ofToString(frameDuration) << " frames (starting from frame " << ofToString(startFrame) << ") at " << ofToString(fps) << " FPS," << endl;
    
    cout << "with settings: \""
    << ((settings.up) ? "up " : "")
    << ((settings.side) ? "side " : "")
    << ((settings.forward) ? "forward " : "")
    << ((settings.position) ? "position " : "")
    << ((settings.smoothedAcceleration) ? "smoothed acceleration " : "")
    << ((settings.speed) ? "speed " : "") << endl;
    
    float estimated = ofGetElapsedTimef();
    
    stringstream ss;
	ss << plugin->name()
    << "_" << ofToString(frameDuration)
    << "_" << ofToString(startFrame)
    << "_" << ofToString(fps)
    << ((settings.up) ? "_up" : "")
    << ((settings.side) ? "_sd" : "")
    << ((settings.forward) ? "_frw" : "")
    << ((settings.position) ? "_pos" : "")
    << ((settings.smoothedAcceleration) ? "_sa" : "")
    << ((settings.speed) ? "_spd" : "");
    filePath = ss.str();
    
    cout << "Checking if the simulation was already saved at " << filePath << "..." << endl;
        
    if( file.open(filePath, ofFile::ReadWrite) ){
		cout << "Simulation found!" << endl;
        loadSettings();
	}else{
        cout << "Unable to load simulation." << endl;
        saveSettings();
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
    while(frames.size() > 0){
        ofxOpenSteerPluginCacherFrame* frame = frames.back();
        
        while (frame->units.size() > 0) {
            ofxOpenSteerPluginCacherFrameUnit* unit = frame->units.back();
            delete unit;
            unit = NULL;
            frame->units.pop_back();
        }
        delete frame;
        frame = NULL;
        
        frames.pop_back();
    }
}

void ofxOpenSteerPluginCacher::update(int frame){
    if(!plugin)return;
    int f = ofClamp(frame, 0, frameDuration - 1);
    VehicleGroup vehicles = plugin->getVehicles();
    int unitIndex = 0;
    float curTime = (float)f * fps;
    for (VehicleIterator it = vehicles.begin(); it != vehicles.end(); it++) {
        ofxOpenSteerPluginCacherFrameUnit* unit = frames[f]->units[unitIndex];
        if(settings.side) (*it)->setSide(unit->side);
        if(settings.up)(*it)->setUp(unit->up);
        if(settings.forward)(*it)->setForward(unit->forward);
        if(settings.position)(*it)->setPosition(unit->position);
        if(settings.speed)(*it)->setSpeed(unit->speed);
        if(settings.smoothedAcceleration)(*it)->resetSmoothedAcceleration(unit->smoothedAcceleration);
        unitIndex++;
    }
    
}

void ofxOpenSteerPluginCacher::loadSettings(){
    cout << "Loading: " << endl;
    
	vector <string> frameData;
	while (!file.eof()) {
		string frameLine;
		getline(file, frameLine);
		frameData.push_back(frameLine);
	}
	file.close();
	
    int lastPercent = -1;
    float lastProcessTime = ofGetElapsedTimef();
    for (int i = 0; i < frameDuration; i++) {
        
        string frameLine = frameData[i];
        vector <string> unitData = ofSplitString(frameLine, "_");
        
        ofxOpenSteerPluginCacherFrame* frame = new ofxOpenSteerPluginCacherFrame();
        
        VehicleGroup vehicles = plugin->getVehicles();
        int j = 0;
        for (VehicleIterator it = vehicles.begin(); it != vehicles.end(); it++) {
            ofxOpenSteerPluginCacherFrameUnit* unit = new ofxOpenSteerPluginCacherFrameUnit();
            
            // Get from to xml
            string unitLine = unitData[j];            
            vector <string> cordinateData = ofSplitString(unitLine, "|");
            
            if(settings.side){
                vector <string> sideData = ofSplitString(cordinateData[0], ",");            
                unit->side.x = ofToFloat(sideData[0]);
                unit->side.y = ofToFloat(sideData[1]);
                unit->side.z = ofToFloat(sideData[2]);
            }
            
            if(settings.up){
                vector <string> upData = ofSplitString(cordinateData[1], ",");            
                unit->up.x = ofToFloat(upData[0]);
                unit->up.y = ofToFloat(upData[1]);
                unit->up.z = ofToFloat(upData[2]);
            }
            
            if(settings.forward){
                vector <string> forwardData = ofSplitString(cordinateData[2], ",");            
                unit->forward.x = ofToFloat(forwardData[0]);
                unit->forward.y = ofToFloat(forwardData[1]);
                unit->forward.z = ofToFloat(forwardData[2]);
            }
            
            if(settings.position){
                vector <string> positionData = ofSplitString(cordinateData[3], ",");            
                unit->position.x = ofToFloat(positionData[0]);
                unit->position.y = ofToFloat(positionData[1]);
                unit->position.z = ofToFloat(positionData[2]);
            }
            
            if(settings.smoothedAcceleration){
                vector <string> smoothedAccelerationData = ofSplitString(cordinateData[4], ",");            
                unit->smoothedAcceleration.x = ofToFloat(smoothedAccelerationData[0]);
                unit->smoothedAcceleration.y = ofToFloat(smoothedAccelerationData[1]);
                unit->smoothedAcceleration.z = ofToFloat(smoothedAccelerationData[2]);
            }
            
            if(settings.speed){
                vector <string> speedData = ofSplitString(cordinateData[5], ",");            
                unit->speed = ofToFloat(speedData[0]);
            }
            
            frame->units.push_back(unit);
            
            j++;
        }
        frames.push_back(frame);
        
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

void ofxOpenSteerPluginCacher::saveSettings(){
    cout << "Recording: " << endl;
    
    file.create();
    file.open(filePath, ofFile::ReadWrite);
    file.clear();
       
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
    
    for (int i = 0; i < frameDuration; i++) {
        currentTime += elapsedTime;
        plugin->update(currentTime, elapsedTime);
        
        ofxOpenSteerPluginCacherFrame* frame = new ofxOpenSteerPluginCacherFrame();
        
        VehicleGroup vehicles = plugin->getVehicles();
        for (VehicleIterator it = vehicles.begin(); it != vehicles.end(); it++) {
            ofxOpenSteerPluginCacherFrameUnit* unit = new ofxOpenSteerPluginCacherFrameUnit();
            
            // Save to memory
            unit->side = (*it)->side();
            unit->up = (*it)->up();
            unit->forward = (*it)->forward();
            unit->position = (*it)->position();
            unit->smoothedAcceleration = (*it)->smoothedAcceleration();
            unit->speed = (*it)->speed();
            frame->units.push_back(unit);
            
            // Save to file
            
            // side
            if(settings.side){
                file << unit->side.x << ",";
                file << unit->side.y << ",";
                file << unit->side.z << ",";
                file << "|";
            }
            // up
            if(settings.up){  
                file << unit->up.x << ",";
                file << unit->up.y << ",";
                file << unit->up.z << ",";
                file << "|";
            }
            if(settings.forward){
                // forward
                file << unit->forward.x << ",";
                file << unit->forward.y << ",";
                file << unit->forward.z << ",";
                file << "|";
            }
            if(settings.position){
                // position
                file << unit->position.x << ",";
                file << unit->position.y << ",";
                file << unit->position.z << ",";
                file << "|";
            }
            if(settings.smoothedAcceleration){
                // smoothedAcceleration
                file << unit->smoothedAcceleration.x << ",";
                file << unit->smoothedAcceleration.y << ",";
                file << unit->smoothedAcceleration.z << ",";
                file << "|";
            }
            if(settings.speed){
                // speed
                file << unit->speed << ",";
                file << "|";
            }
            
            // unit delimiter
            file << "_";
            
        }
        frames.push_back(frame);
        
        // frame delimiter
        file << "\n";
        
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
    file.close();
}