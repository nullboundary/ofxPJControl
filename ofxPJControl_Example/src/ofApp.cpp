#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(ofColor::black);
    
    projector.setup();
    string myProjectorIP = "172.16.1.201";

    //Plug an ethernet into your projector and check it settings to find this IP
    projector.setProjectorIP(myProjectorIP);
    
    //You can set a password on your projector to access PJ Link
    projector.setProjectorPassword("somePassword");
    projector.setProjectorType(PJLINK_MODE);
    
    
    addressDebug = myProjectorIP;
    colorDebug = ofColor::red;
    ofSetCircleResolution(60);
}

//--------------------------------------------------------------
void ofApp::update(){

    

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(ofColor::green);
    ofDrawBitmapString("Projector IP Address: " + ofToString(addressDebug), ofPoint(ofGetWidth() * .10, ofGetHeight() * .15));
     ofDrawBitmapString("Press '1' ON / Press '2' OFF", ofPoint(ofGetWidth() * .10, ofGetHeight() * .25));
    
    ofSetColor(colorDebug);
    ofCircle(ofPoint(ofGetWidth() * .5, ofGetHeight() * .5), 30);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    //Now turn it off and on//
    
    if(key == '1') {
        projector.On();
        cout << projector.getProjectorStatus() << endl;
        colorDebug = ofColor::green;

    }
    
    if (key == '2') {
        projector.Off();
        cout << projector.getProjectorStatus() << endl;
        colorDebug = ofColor::crimson;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
