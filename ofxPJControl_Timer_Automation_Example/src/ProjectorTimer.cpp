

//Created by Fake Love 8.10.15//

#include "ProjectorTimer.h"


void ProjectorTimer::setup(){
    
    projOnHour.set("Projector_Auto_On_at", 8, 0, 23);
    projOffHour.set("Projector_Auto_Off_at", 19, 0, 23);
    projectorPower.set("Projector_Power", false);
    automaticProjectorPower.set("Auto_Power_Projector", true);
    daysToAutoPower.setName("Days_to_Power_Projector");
    autoPwrMon.set("Monday", false) ;
    daysToAutoPower.add(autoPwrMon);
    autoPwrTues.set("Tuesday", false);
    daysToAutoPower.add(autoPwrTues);
    autoPwrWeds.set("Wednesday", false);
    daysToAutoPower.add(autoPwrWeds);
    autoPwrThurs.set("Thursday", false);
    daysToAutoPower.add(autoPwrThurs);
    autoPwrFri.set("Friday", false);
    daysToAutoPower.add(autoPwrFri);
    autoPwrSat.set("Saturday", false);
    daysToAutoPower.add(autoPwrSat);
    autoPwrSun.set("Sunday", false);
    daysToAutoPower.add(autoPwrSun);
    
    fakeHours.set("Fake_Hours",0,0,23);
    fakeMinutes.set("Fake_Minutes",0,0,60);
    fakeDay.set("Fake_Day",3,0,6);
    useFakeTime.set("Use_Fake_Time",false);
    fakeTime.setName("Fake_Time");
    fakeTime.add(useFakeTime);
    fakeTime.add(fakeHours);
    fakeTime.add(fakeMinutes);
    fakeTime.add(fakeDay);
    
    projectorParamGroup.setName("Projector_Controls");
    projectorParamGroup.add(projectorPower);
    projectorParamGroup.add(automaticProjectorPower);
    projectorParamGroup.add(projOnHour);
    projectorParamGroup.add(projOffHour);
    projectorParamGroup.add(daysToAutoPower);
    
    projectorControls.setup(projectorParamGroup);
    projectorFakeTime.setup(fakeTime);
    projectorFakeTime.setPosition(ofPoint(ofGetWidth() * .45, ofGetHeight() * .03));
    
    
    /// SETUP PROJECTOR CONTROL ///
    pjControl.setup(PROJECTOR_IP, PROJECTOR_PORT, PJLINK_MODE, PROJECTOR_PW);
    
    //PJControl get status getProjectorStatus works based on if you app has started the projector
    
    startupProjectors();
    projectorPower = pjControl.getProjectorStatus();
    cout << "projector ? " << projectorPower << endl;
    
}

void ProjectorTimer::update(){
    
    if (bPowerButtonChanged) {
        automaticProjectorPower = false;
        bPowerButtonChanged = false;
    }
    
    if(projectorPower && !pjControl.getProjectorStatus() && !automaticProjectorPower){
        startupProjectors();
        projectorPower = pjControl.getProjectorStatus();
        bPowerButtonChanged = true;
    } else if (!projectorPower && pjControl.getProjectorStatus() && !automaticProjectorPower) {
        shutdownProjectors();
        projectorPower = pjControl.getProjectorStatus();
        bPowerButtonChanged = true;
        
    } else if( projectorPower != pjControl.getProjectorStatus() &&automaticProjectorPower){
        bPowerButtonChanged = true;
        
    }
    
    /// Check projector status every few seconds ///
    if(automaticProjectorPower && int(ofGetElapsedTimef())%10 == 0)  checkProjectorPower();
    
    
}

void ProjectorTimer::draw() {
    
    ofPushMatrix();
    projectorControls.draw();
    ofPopMatrix();
    
    ofPushMatrix();
    projectorFakeTime.draw();
    ofPopMatrix();
}

void ProjectorTimer::checkProjectorPower(){
    
    int currentHour;
    int currentDay;
    if(useFakeTime){
        currentHour = fakeHours;
        currentDay = fakeDay;
    } else {
        currentHour = ofGetHours();
        currentDay = ofGetWeekday();
    }
    bool dayEnabled = false;
    
    switch (currentDay) { //is today enabled?
        case 0:
            dayEnabled = autoPwrSun;
            break;
        case 1:
            dayEnabled = autoPwrMon;
            break;
        case 2:
            dayEnabled = autoPwrTues;
            break;
        case 3:
            dayEnabled = autoPwrWeds;
            break;
        case 4:
            dayEnabled = autoPwrThurs;
            break;
        case 5:
            dayEnabled = autoPwrFri;
            break;
        case 6:
            dayEnabled = autoPwrSat;
            break;
            
        default:
            break;
    }
    
    //if it's between the on hour and the off hour, and the projector's off, and today is enabled, turn it on. if it's later than the off hour and the projector is on, and today is enabled, turn it off.
    if (currentHour >= projOnHour && currentHour < projOffHour && !pjControl.getProjectorStatus() && dayEnabled) {
        startupProjectors();
        projectorPower = true;
        ofLogWarning()<<"started projector automatically at "<< currentHour<<":"<<ofGetMinutes()<<endl;
    } else if (currentHour >= projOffHour && pjControl.getProjectorStatus() && dayEnabled){
        shutdownProjectors();
        projectorPower = false;
        ofLogWarning()<<"shut down projector automatically at "<< currentHour<<":"<<ofGetMinutes()<<endl;
    }
}