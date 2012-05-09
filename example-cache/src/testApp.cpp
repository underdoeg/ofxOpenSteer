#include "testApp.h"

void testApp::setup(){
    /*ifstream ifs(ofToDataPath("original").c_str(), ios::in | ios::binary);    
     ifs.read((char *)&copy, sizeof(Cache) + sizeof(Frame)*nFrames + sizeof(Unit)*nUnits);*/
    
    /*int nUnits = 10;
    int nFrames = 5;    
    Cache original;
    original.size = nFrames;
    original.frames = new Frame[nFrames];
    for(int i = 0; i < original.size; i++){
        original.frames[i].size = nUnits;
        original.frames[i].units = new Unit[nUnits];
        for(int j = 0; j < original.frames[i].size; j++){
            original.frames[i].units[j].number[0] = i+j+1;
            original.frames[i].units[j].number[1] = i+j+2;
            original.frames[i].units[j].number[2] = i+j+3;
        }
    }
    
    ofFile file;
    if(file.open("original", ofFile::WriteOnly, true)){
        file.write((char*) &original, sizeof(Cache) + sizeof(Frame)*nFrames + sizeof(Unit) * nUnits * nFrames);  
    }
    file.close();
    
    Cache copy;    
    ofFile copyFile;
    if(copyFile.open("original", ofFile::ReadOnly, true)){
        memcpy( &copy, copyFile.readToBuffer().getBinaryBuffer(), copyFile.getSize() );
    }
    
    cout << "Num Frames " << copy.size <<  endl;
    
    
    for(int i = 0; i < copy.size; i++){
        cout << "----------- Frame " << i <<  endl;
        cout << "Num Units " << copy.frames[i].size <<  endl;
        for(int j = 0; j < copy.frames[i].size; j++){
            cout << "--- Unit " << j <<  endl;
            cout  << copy.frames[i].units[j].number[0] <<  endl;
            cout  << copy.frames[i].units[j].number[1] <<  endl;
            cout  << copy.frames[i].units[j].number[2] <<  endl;
        }
    }*/
    
    
    
    
    
    
    
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	glEnable(GL_DEPTH_TEST);
	cam.setDistance(50);
    
    // cache the simulation
    cacher.cache(&plugin, 300, 150, 60.f); // pointer to simulation, duration (in frames), start frame, fps
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
