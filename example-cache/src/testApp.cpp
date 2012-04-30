#include "testApp.h"

void testApp::setup(){
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	glEnable(GL_DEPTH_TEST);
	cam.setDistance(50);
    
    // cache the simulation
    ofxOpenSteerPluginCacherSettings cacherSettings;
    // these are default settings (everything true) and can be ommited.
    // just listed them here so you know you can cache just a few properties too
    cacherSettings.position = true;
    cacherSettings.up = true;
    cacherSettings.side = true;
    cacherSettings.forward = true;
    cacherSettings.smoothedAcceleration = true;
    cacherSettings.speed = true;
    cacher.cache(&plugin, 300, 150, 60.f, cacherSettings); // pointer to simulation, duration (in frames), start frame, fps, settings
    currentFrame = 0;
}

void testApp::update(){
    cacher.update(currentFrame);
}

void testApp::draw(){
	cam.begin();
        plugin.draw();
	cam.end();
    
    ofSetColor(0);
	stringstream ss;
	ss << plugin.name() << "\n";
	ss << "APP FPS: " << ofToString(ofGetFrameRate()) << "\n";
    ss << "Simulation recorded at " << ofToString(cacher.getFPS()) << " FPS\n";
    ss << "Current frame (" << ofToString(currentFrame) << "/" << ofToString(cacher.getFrameDuration() - 1) << ")\n";
	ss << "Press left/right to rewind/forward the playhead.\n";
	ss << "Drag mouse to move camera.\n";
	
	ofDrawBitmapString(ss.str(), 10.f,20.f);

}

void testApp::keyPressed( int key ){
	if(key == OF_KEY_LEFT){
        currentFrame--;
    }
    else if(key == OF_KEY_RIGHT){
        currentFrame++;
    }
    currentFrame = ofClamp(currentFrame, 0, cacher.getFrameDuration() - 1);	
}
