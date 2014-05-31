```
Name    : ofxPJControl Library                         
Author  : Noah Shibley, http://socialhardware.net                       
Date    : Aug 9th 2010                                 
Version : 0.1                                               
Notes   : PJLink and NEC projector control addon.
Dependencies:	ofxNetwork addon
```
Allows you to turn on or off and control a PJLink(most projectors) or NEC projector through the network.

PJLink Specification:
http://pjlink.jbmia.or.jp/english/data/PJLink%20Specifications100.pdf

"PJLink enables central control of projectors manufactured by different 
vendors and projectors can be operated by a controller."

------------------------------------------------------

Function list:
```cpp
        void On(); //command to turn the projector off
	void Off(); //command to turn the projector on
	void sendPJLinkCommand(string command); //send any PJLink command to the projector
	void setup(string IP_add="192.168.0.100",int protocol=PJLINK_MODE, string password=""); //default
	void setProjectorType(int protocol); //NEC_MODE or PJLINK_MODE
	void setProjectorIP(string IP_add); //the network IP of the projector
    	void setProjectorPassword(string passwd); //password for PJLink authentication
	bool getProjectorStatus(); //return whether projector is on (true) or off (false)  
```
------------------------------------------------------

 Example:

Step 1. in the testApp.h: 
```cpp	
#include "ofxPJControl.h"	
ofxPJControl projector1;

Step 2. in the testApp.cpp:

//Your projectors IP, and if its not NEC try PJLINK
projector1.setup("192.168.1.281",PJLINK_MODE,"mypassword"); 
projector1.On();
projector1.Off(); 
```
