#include "testApp.h"

void testApp::setup(){
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	glEnable(GL_DEPTH_TEST);
	cam.setDistance(50);
    
    // record the simulation
    recorder.record(&plugin, 300, 60.f); // pointer to simulation, duration (in frames), fps
    currentFrame = 0;
}

void testApp::update(){
    recorder.update(currentFrame);
}

void testApp::draw(){
	cam.begin();
        plugin.draw();
	cam.end();
    
    ofSetColor(0);
	stringstream ss;
	ss << plugin.name() << "\n";
	ss << "APP FPS: " << ofToString(ofGetFrameRate()) << "\n";
    ss << "Simulation recorded at " << ofToString(recorder.getFPS()) << " FPS\n";
    ss << "Current frame (" << ofToString(currentFrame) << "/" << ofToString(recorder.getFrameDuration() - 1) << ")\n";
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
    currentFrame = ofClamp(currentFrame, 0, recorder.getFrameDuration() - 1);	
}
