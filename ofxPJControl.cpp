/*
 *  pjControl.cpp
 *  
 *
 *  Created by Noah Shibley on 8/9/10.
 *  Copyright 2010 Noah Shibley. All rights reserved.
 *
 */

#include "ofxpjControl.h"

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

void ofxPJControl::setProjectorType(int protocol) //NEC_MODE or PJLINK_MODE
{
		commMode = protocol;
}

void ofxPJControl::setup(string IP_add, int protocol)
{
	setProjectorIP(IP_add);
	setProjectorType(protocol);
}	

void ofxPJControl::setProjectorIP(string IP_add)
{
	IPAddress = IP_add;
}

void ofxPJControl::On()
{
	if(commMode == NEC_MODE)
	{	
		nec_On();
	}
	else if (commMode == PJLINK_MODE)
	{	
		pjLink_On();	
	}	
		
}

void ofxPJControl::Off()
{
	
	if(commMode == NEC_MODE)
	{	
		nec_Off();
	}
	else if (commMode == PJLINK_MODE)
	{	
		pjLink_Off();	
	}	
	
	
}

void ofxPJControl::nec_On()
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
	if(!pjClient.isConnected())
	{	
		connected = pjClient.setup(IPAddress, NEC_PORT);
		cout << "connection established: " << IPAddress << ":" << NEC_PORT << endl;  
	}
	cout << "sending command: ON" << endl; 
	
	pjClient.sendRawBytes(buffer, 6);
	
	printf("send: %x %x %x %x %x %x\n",buffer[0] , buffer[1] , buffer[2] , buffer[3] , buffer[4] , buffer[5] );

	char* rxBuffer = new char[6];
	
	pjClient.receiveRawBytes(rxBuffer, 6);
	
	printf("receive: %x %x %x %x %x %x\n",rxBuffer[0] , rxBuffer[1] , rxBuffer[2] , rxBuffer[3] , rxBuffer[4] , rxBuffer[5] );
	
	projStatus = true;
	
	delete rxBuffer;
	delete buffer;
	
	
}	

void ofxPJControl::nec_Off()
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
	
	if(!pjClient.isConnected())
	{	
		connected = pjClient.setup(IPAddress, NEC_PORT);
		cout << "connection established: " << IPAddress << ":" << NEC_PORT << endl; 

	}
	
	cout << "sending command: OFF " << endl; 
	
	pjClient.sendRawBytes(buffer, 6);
	printf("send: %x %x %x %x %x %x\n",buffer[0] , buffer[1] , buffer[2] , buffer[3] , buffer[4] , buffer[5] );

	
	char* rxBuffer = new char[6];
	
	pjClient.receiveRawBytes(rxBuffer, 6);
	
	printf("receive: %x %x %x %x %x %x\n",rxBuffer[0] , rxBuffer[1] , rxBuffer[2] , rxBuffer[3] , rxBuffer[4] , rxBuffer[5] );
	
	projStatus = false;
	
	delete rxBuffer;
	delete buffer;
	
	
	
}

void ofxPJControl::pjLink_On()
{

	string command = "%1POWR 1\r";
	string msgRx;
	
	if(!connected)
	{	
		pjClient.setVerbose(true);
		connected = pjClient.setup(IPAddress, PJLINK_PORT);
		cout << "connection established: " << IPAddress << ":" << PJLINK_PORT << endl;
		
		while (msgRx.length() < 8) 
		{
			msgRx = pjClient.receiveRaw();	
			if(msgRx.length() > 6)
			{	
				cout<< msgRx << endl;
			}	
		}	
	
	}
	cout << "sending command: ON " << endl; 
	
	pjClient.sendRaw(command);
	msgRx = "";
	while (msgRx.length() < 8) 
	{
		msgRx = pjClient.receiveRaw();	
	
		cout<< msgRx << endl;

	}
	pjClient.close();
	connected = false;
	projStatus = true; //projector on
}

void ofxPJControl::pjLink_Off()
{
	

	string command = "%1POWR 0\r";
	string msgRx;
	
	if(!connected)
	{	
		pjClient.setVerbose(true);
		connected = pjClient.setup(IPAddress, PJLINK_PORT);
		cout << "connection established: " << IPAddress << ":" << PJLINK_PORT << endl;
		
		while (msgRx.length() < 8) 
		{
			msgRx = pjClient.receiveRaw();
			if(msgRx.length() > 6)
			{	
				cout<< msgRx << msgRx.length() << endl;
			}	
		}	
	}
	cout << "sending command: OFF" << endl; 
	
	pjClient.sendRaw(command);
	msgRx = "";
	while (msgRx.length() < 8) 
	{
		msgRx = pjClient.receiveRaw();	
		
		cout<< msgRx << endl;
	
	}
	pjClient.close();
	connected = false;
	projStatus = false; //projector off
	
}

