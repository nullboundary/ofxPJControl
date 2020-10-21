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

ofxPJControl::ofxPJControl()
{
	connected = false;
	projStatus = false;
}

ofxPJControl::~ofxPJControl()
{
}

bool ofxPJControl::getProjectorStatus()
{
	return projStatus;
}

void ofxPJControl::setProjectorType(int protocol)
{
	//NEC_MODE or PJLINK_MODE
	commMode = protocol;
}

void ofxPJControl::setup(string IP_add, int port, int protocol, string password)
{
	setProjectorIP(IP_add);
	setProjectorType(protocol);
	setProjectorPort(port);
	setProjectorPassword(password);
}

void ofxPJControl::setProjectorIP(string IP_add)
{
	IPAddress = IP_add;
}

void ofxPJControl::setProjectorPort(int port)
{
	pjPort = port;
}

void ofxPJControl::setProjectorPassword(string passwd)
{
	password = passwd;
}


bool ofxPJControl::On()
{
	if (commMode == NEC_MODE)
	{
		return nec_On();
	}
	else if (commMode == PJLINK_MODE)
	{
		return pjLink_On();
	}
	else if (commMode == CHRISTIE_MODE)
	{
		return christie_On();
	}
	else if (commMode == SANYO_MODE)
	{
		return sanyo_On();
	}
	else if (commMode == PJDESIGN_MODE)
	{
		return pjDesign_On();
	}
	return false;
}

bool ofxPJControl::Off()
{
	if (commMode == NEC_MODE)
	{
		return nec_Off();
	}
	else if (commMode == PJLINK_MODE)
	{
		return pjLink_Off();
	}
	else if (commMode == CHRISTIE_MODE)
	{
		return christie_Off();
	}
	else if (commMode == SANYO_MODE)
	{
		return sanyo_Off();
	}
	else if (commMode == PJDESIGN_MODE)
	{
		return pjDesign_Off();
	}
	return false;
}

bool ofxPJControl::sendPJLinkCommand(string command)
{
	string msgRx = "";

	if (!pjClient.isConnected())
	{
		//pjClient.setVerbose(true);
		connected = pjClient.setup(IPAddress, pjPort, true);
		if (connected)
		{
			ofLogNotice("", "connection established to %s:%i", IPAddress.c_str(), pjPort);
			string response = "";
			while (msgRx.length() < 8)
			{
				msgRx = pjClient.receiveRaw();
			}
			ofLogNotice("", "received response: %s", msgRx.c_str());
		}
	}

	if (connected)
	{
		string authToken = "";

		//eg. PJLINK 1 604cc14d
		if (msgRx[7] == '1')
		{
			ofLogNotice("", "try with authentication");
			MD5Engine md5;
			md5.reset();
			string hash = msgRx.substr(9, 8);
			ofLogNotice() << hash << endl;
			md5.update(hash + password);
			authToken = DigestEngine::digestToHex(md5.digest());
		}
		ofLogNotice("sending command: %s %s", authToken.c_str(), command.c_str());
		pjClient.sendRaw(authToken + command);
		msgRx = "";
		while (msgRx.length() < 8)
		{
			msgRx = pjClient.receiveRaw();
		}
		ofLogNotice("", "received response: %s ", msgRx.c_str());

		pjClient.close();
		return true;
	}

	ofLogError("", "couldn't connect");
	pjClient.close();
	return false;
}

bool ofxPJControl::sendCommand(string command)
{
	if (!pjClient.isConnected())
	{
		pjClient.setVerbose(true);
		ofLogNotice() << "connecting to : " << IPAddress << ":" << pjPort << endl;
		connected = pjClient.setup(IPAddress, pjPort, true);
		ofLogNotice() << "connection state : " << connected;
	}
	ofLogNotice() << "sending command : " << command << endl;
	bool result = pjClient.sendRaw(command);
	ofLogNotice() << "Response length (Bytes) : " << pjClient.getNumReceivedBytes() << endl;
	msgRx = "";
	msgRx = pjClient.receiveRaw();
	ofLogNotice() << "received response : " << msgRx << endl;

	pjClient.close();
	return result;
}

bool ofxPJControl::nec_On()
{
	pjClient.close(); //close any open connections first
	char* buffer = new char[6]; //02H 00H 00H 00H 00H 02H (the on command in hex)
	buffer[0] = 2;
	buffer[1] = 0;
	buffer[2] = 0;
	buffer[3] = 0;
	buffer[4] = 0;
	buffer[5] = 2;

	pjClient.setVerbose(true);
	if (!pjClient.isConnected())
	{
		connected = pjClient.setup(IPAddress, NEC_PORT);
		ofLogNotice() << "connection established: " << IPAddress << ":" << NEC_PORT << endl;
	}
	ofLogNotice() << "sending command: ON" << endl;

	bool result = pjClient.sendRawBytes(buffer, 6);

	printf("sent: %x %x %x %x %x %x\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);

	char* rxBuffer = new char[6];

	pjClient.receiveRawBytes(rxBuffer, 6);

	printf("received: %x %x %x %x %x %x\n", rxBuffer[0], rxBuffer[1], rxBuffer[2], rxBuffer[3], rxBuffer[4],
	       rxBuffer[5]);

	projStatus = true;

	delete rxBuffer;
	delete buffer;
	return result;
}

bool ofxPJControl::nec_Off()
{
	char* buffer = new char[6]; //02H 01H 00H 00H 00H 03H (the off command in hex)
	buffer[0] = 2;
	buffer[1] = 1;
	buffer[2] = 0;
	buffer[3] = 0;
	buffer[4] = 0;
	buffer[5] = 3;

	projStatus = true;

	pjClient.setVerbose(true);

	if (!pjClient.isConnected())
	{
		connected = pjClient.setup(IPAddress, NEC_PORT);
		ofLogNotice() << "connection established: " << IPAddress << ":" << NEC_PORT << endl;
	}

	ofLogNotice() << "sending command: OFF " << endl;

	bool result = pjClient.sendRawBytes(buffer, 6);
	printf("send: %x %x %x %x %x %x\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);


	char* rxBuffer = new char[6];

	pjClient.receiveRawBytes(rxBuffer, 6);

	printf("receive: %x %x %x %x %x %x\n", rxBuffer[0], rxBuffer[1], rxBuffer[2], rxBuffer[3], rxBuffer[4],
	       rxBuffer[5]);

	projStatus = false;

	delete rxBuffer;
	delete buffer;
	return result;
}

bool ofxPJControl::pjLink_On()
{
	string command = "%1POWR 1\r";
	projStatus = sendPJLinkCommand(command);
	return projStatus;
}

bool ofxPJControl::pjLink_Off()
{
	string command = "%1POWR 0\r";
	projStatus = !sendPJLinkCommand(command);
	return !projStatus;
}

bool ofxPJControl::sanyo_On()
{
	string command = "PWR ON\r";
	projStatus = sendCommand(command);
	return projStatus;
}

bool ofxPJControl::sanyo_Off()
{
	string command = "PWR OFF\r";
	projStatus = !sendCommand(command);
	return !projStatus;
}

bool ofxPJControl::christie_On()
{
	string command = "(PWR1)";
	projStatus = sendCommand(command);
	return projStatus;
}

bool ofxPJControl::christie_Off()
{
	string command = "(PWR0)";
	projStatus = !sendCommand(command);
	return !projStatus;
}

bool ofxPJControl::pjDesign_On()
{
	string command = ":POWR 1\r";
	projStatus = sendCommand(command);
	return projStatus;
}

bool ofxPJControl::pjDesign_Off()
{
	string command = ":POWR 0\r";
	projStatus = !sendCommand(command);
	return !projStatus;
}
