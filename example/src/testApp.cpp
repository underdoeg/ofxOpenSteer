#include "testApp.h"

void testApp::setup(){
	
	ofEnableAlphaBlending();
	
	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	glEnable(GL_DEPTH_TEST);
	
	// the sample simulations are quite small in scale, so
	// we wanna make sure our camera is close enough
	cam.setDistance(50);
	
	
	// add the simulations to the vector and initialize the first one
	simulations.push_back(&flocking);
	simulations.push_back(&pathFollowing);
	simulations.push_back(&obstacleAvoidance);
    simulations.push_back(&pursuitAndEvade);
	
	currentSimulation = NULL;
	setSimulation(0);
}

void testApp::update(){
	currentSimulation->update();
}

void testApp::draw(){
	cam.begin();
		currentSimulation->draw();
	cam.end();
	
	ofSetColor(0);
	stringstream ss;
	ss << "Simulation (" << ofToString(simulationIndex + 1) << "/" << ofToString(simulations.size()) <<"): " << currentSimulation->name() << "\n";
	ss << "FPS: " << ofToString(ofGetFrameRate()) << "\n";
	ss << "Press left/right to switch between simulations.\n";
	ss << "Drag mouse to move camera.\n";
	
	ofDrawBitmapString(ss.str(), 10.f,20.f);
}

void testApp::keyPressed( int key ){
	
	if( key == OF_KEY_LEFT ){
		simulationIndex--;
		if(simulationIndex < 0) simulationIndex = simulations.size() - 1;
		setSimulation(simulationIndex);
	}
	else if( key == OF_KEY_RIGHT ){
		simulationIndex++;
		if(simulationIndex > simulations.size() - 1) simulationIndex = 0;
		setSimulation(simulationIndex);
	}
}

void testApp::setSimulation( int simulationIndex ){
	// just make sure we are inside out vector
	this->simulationIndex = ofClamp(simulationIndex, 0, simulations.size() - 1);
	
	// exit the current simulation
	if(currentSimulation) currentSimulation->exit();
	
	// define the current simulation
	currentSimulation = simulations[simulationIndex];
	
	// initialize the current simulation
	currentSimulation->setup();
}