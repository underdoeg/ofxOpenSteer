#include "testApp.h"

void testApp::setup(){
	
	ofEnableAlphaBlending();
	
	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	glEnable(GL_DEPTH_TEST);
	
	// the sample simulations are quite small in scale, so
	// we wanna make sure our camera is close enough
	cam.setDistance(50);
	
	flocking.setup();
}

void testApp::update(){
	flocking.update();
}

void testApp::draw(){
	cam.begin();
		flocking.draw();
	cam.end();
	
	ofSetColor(0);
	stringstream ss;
	ss << "Simulation: " << flocking.name() << "\n";
	ss << "FPS: " << ofToString(ofGetFrameRate()) << "\n";
	ss << "Drag mouse to move camera\n";
	
	ofDrawBitmapString(ss.str(), 10.f,20.f);
}

void testApp::keyPressed( int key ){
	
}