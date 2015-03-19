openFrameworks addon to turn ON or OFF video projectors on TCP network :

- PJLink (most projectors)
- NEC projector
- CHRISTIE, SANYO and EPSON with raw string commands

Original Author  : Noah Shibley, http://socialhardware.net                       

#### PJLink Specification

http://pjlink.jbmia.or.jp/english/data/5-1_PJLink_eng_20131210.pdf

#### Example:

Step 1. in the testApp.h: 
```cpp	
#include "ofxPJControl.h"	
ofxPJControl projector1;
```

Step 2. in the testApp.cpp:

```cpp	
//Your projectors IP, and if its not PJLINK try NEC
projector1.setup("192.168.1.281",PJLINK_MODE,"mypassword"); 
projector1.On();
projector1.Off(); 
```
