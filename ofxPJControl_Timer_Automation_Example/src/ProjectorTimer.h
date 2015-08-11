
//  Created by Fake Love 8.11.15
//  ProjectorTimer Class


#pragma once

#include "ofMain.h"
#include "ofxPjControl.h"
#include "ofxGui.h"

//DEFINE PROJECTOR IP, PORT, PASSWORD//

#define PROJECTOR_IP "127.0.0.1"
#define PROJECTOR_PORT 4352
#define PROJECTOR_PW "panasonic"

/*
 NEC PORT 7142
 PJLINK PORT 4352
 CHRISTIE PORT 3002
 SAYNO PORT 100
 */

class ProjectorTimer {
    
public:

    void setup();
    void checkProjectorPower();
    void update();
    void draw();

private:
    
    ofxPJControl pjControl;
    
    inline void startupProjectors(){
        ofLogWarning()<<"PJ On!"<<endl;
        pjControl.On();
    }
    
    inline void shutdownProjectors(){
        ofLogWarning()<<"PJ Off!"<<endl;
        pjControl.Off();
    }
    ofxPanel projectorControls;
    ofxPanel projectorFakeTime;
    
    ofParameterGroup projectorParamGroup;
    ofParameterGroup fakeTime;
    
    ofParameter<int> projOnHour;
    ofParameter<int> projOffHour;
    
    ofParameter<bool> automaticProjectorPower;
    ofParameter<bool> projectorPower;
    
    ofParameterGroup daysToAutoPower;
    ofParameter<bool> autoPwrMon;
    ofParameter<bool> autoPwrTues;
    ofParameter<bool> autoPwrWeds;
    ofParameter<bool> autoPwrThurs;
    ofParameter<bool> autoPwrFri;
    ofParameter<bool> autoPwrSat;
    ofParameter<bool> autoPwrSun;
    
    ofParameter<int> fakeHours;
    ofParameter<int> fakeMinutes;
    ofParameter<int> fakeDay;
    ofParameter<bool> useFakeTime;
    bool bPowerButtonChanged = false;
    
};
