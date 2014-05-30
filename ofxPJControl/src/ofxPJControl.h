/*
 *  pjControl.h
 *   
 *  Created by Noah Shibley on 8/9/10.
 *  Copyright 2010 Noah Shibley. All rights reserved.
 *
	PJLink and NEC projector control class. 
	Allows you to turn on or off a projector through the network.

	
	in the testApp.h 
	
	#include "ofxPJControl.h"
	
	ofxPJControl projector1;
	
	in the testApp.cpp
	
	projector1.setup("192.168.1.281",PJLINK_MODE); //Your projectors IP, and if its not NEC try PJLINK
	projector1.On();
	projector1.Off(); 
 *
 *
 */

#ifndef OFXPJCONTROL_H
#define OFXPJCONTROL_H

#include "ofxNetwork.h"
#include "Poco/MD5Engine.h"
#include "Poco/DigestStream.h"
#include "Poco/StreamCopier.h"

using Poco::DigestEngine;
using Poco::MD5Engine;
using Poco::DigestOutputStream;
using Poco::StreamCopier;


const int NEC_PORT = 7142; //NEC projector port
const int PJLINK_PORT = 4352; //PJ Link projector protocol port

const int PJLINK_MODE = 0;
const int NEC_MODE = 1;

class ofxPJControl
{
public:		
	ofxPJControl();
	~ofxPJControl();
	
	//methods
	void On(); //command to turn the projector off
	void Off(); //command to turn the projector on
	void sendPJLinkCommand(string command); //send any PJLink command to the projector
	void setup(string IP_add="192.168.0.100",int protocol=PJLINK_MODE, string password=""); //default
	void setProjectorType(int protocol); //NEC_MODE or PJLINK_MODE
	void setProjectorIP(string IP_add); //the network IP of the projector
    void setProjectorPassword(string passwd); //password for PJLink authentication
	bool getProjectorStatus(); //return whether projector is on (true) or off (false)  
		
private:
	
	void nec_On();
	void nec_Off();
	void pjLink_On();
	void pjLink_Off();
	
	ofxTCPClient pjClient;

	string IPAddress;
    string password;
	
	bool projStatus;
	string msgTx;
	string msgRx;	
	bool connected;
	int commMode;
	
};

#endif

