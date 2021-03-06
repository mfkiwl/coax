/************************************************************
* 
* API and Communication library for the Skybotix AG
* helicopters, and particularly COAX
*
* Developed by Cedric Pradalier: cedric.pradalier@skybotix.ch
* Send modification or corrections as patches (diff -Naur)
* Copyright: Skybotix AG, 2009-2012
* 
* All rights reserved.
* 
* Skybotix API is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* Skybotix API is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public License
* along with Skybotix API. If not, see <http://www.gnu.org/licenses/>.
* 
************************************************************/
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

//IO stuff
#include <sys/ioctl.h>
//#include <sys/time.h>
//#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef WIN32
#define sscanf sscanf_s
#define strcpy strcpy_s
#define strncpy strncpy_s
#endif

#include <string>
#include <vector>

#include <com/sbapi.h>
#include <com/sbsimple.h>

//#define DEBUG(c) printf("Executing "#c"\n")
//#define DEBUG(c) res=0;printf("Executing "#c"\n");c;printf("Result %d\n",res)
#define DEBUG(c) res=0;c;if (res) printf("Result of "#c": %d\n",res)
#define CRITICAL(c) res=0;c;if (res) {printf("Result of "#c": %d\n",res); return res;}

static int end = 0;

void sighdl(int n) {
	end ++;
}


class SBController
{
	protected:
		SBApiSimpleContext *simple;
		unsigned long sensorList;
		int res;
	public:
		SBController(SBApiSimpleContext *s) : simple(s), sensorList(0), res(0) {
			sbSimpleDefaultContext(simple);
		}
		~SBController() {
		}

		int initialise(const std::string & devname) {
			res = 0;
			sbSimpleParseChannel(simple,devname.c_str(),NULL);
            simple->masterMode = 0;
			CRITICAL(res = sbSimpleInitialise(simple));
			printf("Channel connected, continuing\n");
            simple->control.ackMode = 1;
            sbRegisterRepeatCallback(&simple->control,repeatCB,this);
			return res;
		}

		int terminate() {
			res = 0;
			DEBUG(res = sbSimpleTerminate(simple));
			return res;
		}


		int sendMessage(const unsigned char *msg, unsigned int len) {
			sbLockCommunication(&simple->control);
			DEBUG(res = sbSendRepeatMessage(&simple->control,msg,len));
			sbUnlockCommunication(&simple->control);
			return res;
		}

        static void repeatCB(const unsigned char *msg, unsigned int len, void * userData) {
            SBController*ctrller = (SBController*)userData;
            ctrller->process(msg,len);
        }

        void process(const unsigned char *msg, unsigned int len) {
            char buf[len + 1];
            memcpy(buf,msg,len);
            buf[len] = 0;
            printf("Received: %s\n",buf);
		}


};

// #define SIMULATION

int main(int argc, const char *argv[])
{
	int res;
	const SBVersionStatus * compiled = sbGetCompiledVersion(); 
	unsigned int objSizeStatus = sbValidateObjectSize(compiled);
	printf("Object size status: %04x\n",objSizeStatus);
	assert(objSizeStatus == 0);


	SBApiSimpleContext simple;
	SBController api(&simple);

	if (argc < 2) {
		printf("Usage:\n\t%s [message]\n",argv[0]);
		return -1;
	}
	CRITICAL(res = api.initialise(argv[1]));

    if (argc > 2) {
        printf("Repeat: Acting as sender\n");
        unsigned int i=0;
        for (i=0;i<10;i++) {
            if (*(simple.endP)) {
                break;
            }
            char msg[128];
            sprintf(msg,"%d: %f",i,sbGetCurrentTime());
            api.sendMessage((const unsigned char*)msg,strlen(msg));
            printf("Sent: '%s'\n",msg);
            sleep(1);
        }
    } else {
        printf("Repeat: Acting as listener\n");
        while (!*(simple.endP)) {
            // nothing to do, the receiving thread is doing the job
            sleep(1);
        }
    }

	DEBUG(res = api.terminate());

	return 0;
}

		
