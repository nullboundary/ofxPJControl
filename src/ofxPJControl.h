/*
 *  pjControl.h
 *
 *  Created by Noah Shibley on 8/9/10.
 *  Updated by Martial GALLORINI on 19/03/2015
 *
	Video projector control class.
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
const int PJLINK_PORT = 4352; //PJLink projector protocol port
const int CHRISTIE_PORT = 3002; //CHRISTIE projector protocol port
const int SANYO_PORT = 100; //SANYO projector protocol port
const int PJDESIGN_PORT = 1025; //Projectino Design projector protocol port

const int PJLINK_MODE = 0;
const int NEC_MODE = 1;
const int CHRISTIE_MODE = 2;
const int SANYO_MODE = 3;
const int PJDESIGN_MODE = 4;


class ofxPJControl
{
public:
	ofxPJControl();
	~ofxPJControl();

	//methods
	bool On(); //command to turn the projector off
	bool Off(); //command to turn the projector on
	bool sendPJLinkCommand(std::string command); //send any PJLink command to the projector
	void setup(std::string IP_add="192.168.0.100",int port = 4352, int protocol=PJLINK_MODE, std::string password=""); //default
	void setProjectorType(int protocol); //NEC_MODE or PJLINK_MODE
	void setProjectorIP(std::string IP_add); //the network IP of the projector
    void setProjectorPassword(std::string passwd); //password for PJLink authentication
	bool getProjectorStatus(); //return whether projector is on (true) or off (false)
	void setProjectorPort(int port); //the network port of the projector
	bool sendCommand(std::string command); //send any string command to the projector without password authentication

private:

	bool nec_On();
	bool nec_Off();
	bool pjLink_On();
	bool pjLink_Off();
	bool sanyo_On();
	bool sanyo_Off();
	bool christie_On();
	bool christie_Off();
    bool pjDesign_On();
    bool pjDesign_Off();

	ofxTCPClient pjClient;

	std::string IPAddress;
	int pjPort;
	std::string password;

	bool projStatus;
	std::string msgTx;
	std::string msgRx;
	bool connected;
	int commMode;

};

#endif

