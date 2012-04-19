#include "ofxOpenSteerPluginCacher.h"

ofxOpenSteerPluginCacher::ofxOpenSteerPluginCacher(){
    plugin = NULL;
    fps = 0;
    frameDuration = 0;
}
ofxOpenSteerPluginCacher::~ofxOpenSteerPluginCacher(){
    plugin = NULL;
    clear();
}

void ofxOpenSteerPluginCacher::cache(ofxOpenSteerPlugin* plugin, int frameDuration, float fps){
    clear();
    this->plugin = plugin;
    this->frameDuration = frameDuration;
    this->fps = fps;
    
    // Reset the plugin
    plugin->exit();
    plugin->setup();
    
    cout << "Caching \"" << plugin->name() << "\" for " << ofToString(frameDuration) << " frames at " << ofToString(fps) << " FPS:" << endl;
    
    float estimated = ofGetElapsedTimef();
    
    stringstream ss;
	ss << plugin->name() << "_" << ofToString(frameDuration) << "_" << ofToString(fps);    
    settingsPath = ss.str();
    
    cout << "Checking if the simulation was already saved at " << settingsPath << "..." << endl;
        
    if( settings.open(settingsPath, ofFile::ReadWrite) ){
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
        (*it)->setSide(unit->side);
        (*it)->setUp(unit->up);
        (*it)->setForward(unit->forward);
        (*it)->setPosition(unit->position);
        (*it)->setSpeed(unit->speed);
        (*it)->resetSmoothedAcceleration(unit->smoothedAcceleration);
        unitIndex++;
    }
    
}

void ofxOpenSteerPluginCacher::loadSettings(){
    cout << "Loading: " << endl;
    
	vector <string> frameData;
	while (!settings.eof()) {
		string frameLine;
		getline(settings, frameLine);
		frameData.push_back(frameLine);
	}
	settings.close();
	
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
            
            vector <string> sideData = ofSplitString(cordinateData[0], ",");            
            unit->side.x = ofToFloat(sideData[0]);
            unit->side.y = ofToFloat(sideData[1]);
            unit->side.z = ofToFloat(sideData[2]);
            
            vector <string> upData = ofSplitString(cordinateData[1], ",");            
            unit->up.x = ofToFloat(upData[0]);
            unit->up.y = ofToFloat(upData[1]);
            unit->up.z = ofToFloat(upData[2]);
            
            vector <string> forwardData = ofSplitString(cordinateData[2], ",");            
            unit->forward.x = ofToFloat(forwardData[0]);
            unit->forward.y = ofToFloat(forwardData[1]);
            unit->forward.z = ofToFloat(forwardData[2]);
            
            vector <string> positionData = ofSplitString(cordinateData[3], ",");            
            unit->position.x = ofToFloat(positionData[0]);
            unit->position.y = ofToFloat(positionData[1]);
            unit->position.z = ofToFloat(positionData[2]);
            
            vector <string> smoothedAccelerationData = ofSplitString(cordinateData[4], ",");            
            unit->smoothedAcceleration.x = ofToFloat(smoothedAccelerationData[0]);
            unit->smoothedAcceleration.y = ofToFloat(smoothedAccelerationData[1]);
            unit->smoothedAcceleration.z = ofToFloat(smoothedAccelerationData[2]);
            
            vector <string> speedData = ofSplitString(cordinateData[5], ",");            
            unit->speed = ofToFloat(speedData[0]);        
            
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
    
    settings.create();
    settings.open(settingsPath, ofFile::ReadWrite);
    settings.clear();
       
    float currentTime = 0;
    float elapsedTime = 1.f/fps;
    int lastPercent = -1;
    float lastProcessTime = ofGetElapsedTimef();
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
            settings << unit->side.x << ",";
            settings << unit->side.y << ",";
            settings << unit->side.z << ",";
            settings << "|";
            
            // up
            settings << unit->up.x << ",";
            settings << unit->up.y << ",";
            settings << unit->up.z << ",";
            settings << "|";
            
            // forward
            settings << unit->forward.x << ",";
            settings << unit->forward.y << ",";
            settings << unit->forward.z << ",";
            settings << "|";
            
            // position
            settings << unit->position.x << ",";
            settings << unit->position.y << ",";
            settings << unit->position.z << ",";
            settings << "|";
            
            // smoothedAcceleration
            settings << unit->smoothedAcceleration.x << ",";
            settings << unit->smoothedAcceleration.y << ",";
            settings << unit->smoothedAcceleration.z << ",";
            settings << "|";
            
            // speed
            settings << unit->speed << ",";
            settings << "|";
            
            
            // unit delimiter
            settings << "_";
            
        }
        frames.push_back(frame);
        
        // frame delimiter
        settings << "\n";
        
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
    settings.close();
}