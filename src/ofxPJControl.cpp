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

#include "ofxPJControl.h"
#include "ofMain.h"

ofxPJControl::ofxPJControl() {
	connected = false;
	projStatus = false;
}

ofxPJControl::~ofxPJControl() {

}

bool ofxPJControl::getProjectorStatus() {
	return projStatus;
}

void ofxPJControl::setProjectorType(int protocol) { //NEC_MODE or PJLINK_MODE
		commMode = protocol;
}

void ofxPJControl::setup(string IP_add, int port, int protocol, string password) {
	setProjectorIP(IP_add);
	setProjectorType(protocol);
	setProjectorPort(port);
    setProjectorPassword(password);
}

void ofxPJControl::setProjectorIP(string IP_add) {
	IPAddress = IP_add;
}

void ofxPJControl::setProjectorPort(int port) {
	pjPort = port;
}

void ofxPJControl::setProjectorPassword(string passwd) {
	password = passwd;
}


void ofxPJControl::On() {
	if(commMode == NEC_MODE) {
		nec_On();
	}
	else if (commMode == PJLINK_MODE) {
		pjLink_On();
	}
    else if (commMode == CHRISTIE_MODE) {
		christie_On();
	}
    else if (commMode == SANYO_MODE) {
		sanyo_On();
	}
    else if (commMode == PJDESIGN_MODE) {
        pjDesign_On();
    }
}

void ofxPJControl::Off(){
	if(commMode == NEC_MODE) {
		nec_Off();
	}
	else if (commMode == PJLINK_MODE) {
		pjLink_Off();
	}
	else if (commMode == CHRISTIE_MODE) {
		christie_Off();
	}
    else if (commMode == SANYO_MODE) {
		sanyo_Off();
	}
    else if (commMode == PJDESIGN_MODE) {
        pjDesign_Off();
    }
}

void ofxPJControl::sendPJLinkCommand(string command) {
		string msgRx="";

        if(!pjClient.isConnected()) {
            pjClient.setVerbose(true);
            connected = pjClient.setup(IPAddress, pjPort,true);
            if(connected){
                ofLogNotice() << "connection established: " << IPAddress << ":" << pjPort << endl;
                string response = "";
                while (msgRx.length() < 8) {
                    msgRx = pjClient.receiveRaw();
                }
                ofLogNotice() << "received response: " << msgRx << endl;
            } else {
                ofLogError() << "faled to connect."<<endl;
            }
		}
    
    if(connected){
        string authToken = "";

        //eg. PJLINK 1 604cc14d
        if(msgRx[7] == '1') {
            ofLogNotice() << "with authentication" << endl;
            MD5Engine md5;
            md5.reset();
            string hash = msgRx.substr(9,8);
            ofLogNotice() << hash << endl;
            md5.update(hash + password);
            authToken = DigestEngine::digestToHex(md5.digest());
        }
		ofLogNotice() << "sending command: " << authToken+command << endl;
		pjClient.sendRaw(authToken+command);
		msgRx = "";
		while (msgRx.length() < 8) {
			msgRx = pjClient.receiveRaw();
		}
        ofLogNotice() << "received response: " << msgRx << endl;

        pjClient.close();
		//connected = false;
    } else {
        ofLogError()<< "still not connected."<<endl;
        pjClient.close();

    }
}

void ofxPJControl::sendCommand(string command){
        if(!pjClient.isConnected()) {
			pjClient.setVerbose(true);
			ofLogNotice() << "connecting to : " << IPAddress << ":" << pjPort << endl;
			connected = pjClient.setup(IPAddress, pjPort, true);
			ofLogNotice() << "connection state : " << connected;
		}
        ofLogNotice() << "sending command : " << command << endl;
        pjClient.sendRaw(command);
        ofLogNotice() << "Response length (Bytes) : " << pjClient.getNumReceivedBytes() << endl;
        msgRx = "";
        msgRx = pjClient.receiveRaw();
        ofLogNotice() << "received response : " << msgRx << endl;

        pjClient.close();
}

void ofxPJControl::nec_On(){

	pjClient.close(); //close any open connections first
	char* buffer = new char[6]; //02H 00H 00H 00H 00H 02H (the on command in hex)
	buffer[0] = 2;
	buffer[1] = 0;
	buffer[2] = 0;
	buffer[3] = 0;
	buffer[4] = 0;
	buffer[5] = 2;

	pjClient.setVerbose(true);
	if(!pjClient.isConnected()) {
		connected = pjClient.setup(IPAddress, NEC_PORT);
		ofLogNotice() << "connection established: " << IPAddress << ":" << NEC_PORT << endl;
	}
	ofLogNotice() << "sending command: ON" << endl;

	pjClient.sendRawBytes(buffer, 6);

	printf("sent: %x %x %x %x %x %x\n",buffer[0] , buffer[1] , buffer[2] , buffer[3] , buffer[4] , buffer[5] );

	char* rxBuffer = new char[6];

	pjClient.receiveRawBytes(rxBuffer, 6);

	printf("received: %x %x %x %x %x %x\n",rxBuffer[0] , rxBuffer[1] , rxBuffer[2] , rxBuffer[3] , rxBuffer[4] , rxBuffer[5] );

	projStatus = true;

	delete rxBuffer;
	delete buffer;
}

void ofxPJControl::nec_Off() {

	char* buffer = new char[6]; //02H 01H 00H 00H 00H 03H (the off command in hex)
	buffer[0] = 2;
	buffer[1] = 1;
	buffer[2] = 0;
	buffer[3] = 0;
	buffer[4] = 0;
	buffer[5] = 3;

	projStatus = true;

	pjClient.setVerbose(true);

	if(!pjClient.isConnected()) {
		connected = pjClient.setup(IPAddress, NEC_PORT);
		ofLogNotice() << "connection established: " << IPAddress << ":" << NEC_PORT << endl;
	}

	ofLogNotice() << "sending command: OFF " << endl;

	pjClient.sendRawBytes(buffer, 6);
	printf("send: %x %x %x %x %x %x\n",buffer[0] , buffer[1] , buffer[2] , buffer[3] , buffer[4] , buffer[5] );


	char* rxBuffer = new char[6];

	pjClient.receiveRawBytes(rxBuffer, 6);

	printf("receive: %x %x %x %x %x %x\n",rxBuffer[0] , rxBuffer[1] , rxBuffer[2] , rxBuffer[3] , rxBuffer[4] , rxBuffer[5] );

	projStatus = false;

	delete rxBuffer;
	delete buffer;
}

void ofxPJControl::pjLink_On() {
	string command = "%1POWR 1\r";
    sendPJLinkCommand(command);
    projStatus = true; //projector on

}

void ofxPJControl::pjLink_Off() {
	string command = "%1POWR 0\r";
	sendPJLinkCommand(command);
	projStatus = false; //projector off
}

void ofxPJControl::sanyo_On() {
	string command = "PWR ON\r";
	sendCommand(command);
	projStatus = true; //projector on
}

void ofxPJControl::sanyo_Off() {
	string command = "PWR OFF\r";
	sendCommand(command);
	projStatus = false; //projector off
}

void ofxPJControl::christie_On() {
	string command = "(PWR1)";
	sendCommand(command);
	projStatus = true; //projector on
}

void ofxPJControl::christie_Off() {
	string command = "(PWR0)";
	sendCommand(command);
	projStatus = false; //projector off
}

void ofxPJControl::pjDesign_On() {
    string command = ":POWR 1\r";
    sendCommand(command);
    projStatus = true; //projector on
}

void ofxPJControl::pjDesign_Off() {
    string command = ":POWR 0\r";
    sendCommand(command);
    projStatus = false; //projector off
}



