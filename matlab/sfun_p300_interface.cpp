/*  File    : sfun_counter_cpp.cpp
 *  Abstract:
 *
 *      Example of an C++ S-function which stores an C++ object in
 *      the pointers vector PWork.
 *
 *  Copyright 1990-2009 The MathWorks, Inc.
 *  $Revision: 1.1.6.1 $
 */

#include <fstream>
#include <sstream>
#include <windows.h>
#include <winsock.h>
#include <boost/thread.hpp>

#pragma comment (lib, "boost_thread-vc90-mt.lib")
#pragma comment (lib, "ws2_32.lib")

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  sfun_p300_interface

unsigned int IN_result = 0;
bool IN_stop = false;
unsigned int IN_flash = 0;
bool IN_flashing = false;

double IN_mode = 1; /* 1 : training mode ,  2 : free mode */

unsigned int currTarget = 0;
unsigned int currTargetIdx = 0;
unsigned int targets[6] = {1,4,2,3,4,1};

boost::thread * th = 0;

std::ofstream debug("debug.log");

class P300Server
{
public:
    P300Server(unsigned short port) : m_close(false) , m_result(0)
    {
        WSAStartup(0x0202, &w);
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = htonl (INADDR_ANY);
        sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        bind(sSocket, (LPSOCKADDR)&addr, sizeof(addr));
    }

    ~P300Server()
    {
        m_close = true;
        if(th) { th->join(); }
        closesocket(sSocket);
        WSACleanup();
    }

    void GetFlashIDThread()
    {
		listen(sSocket, 1);
        char buffer[100];
        cSocket =  accept(sSocket, 0, 0);
        while(!m_close)
        {
            if(recv(cSocket, buffer, 100, MSG_PEEK))
            {
                recv(cSocket, buffer, 100, 0);
                std::stringstream ss;
                ss << buffer;
                ss >> IN_flash;
            } 
            if(m_result != IN_result)
            {
                m_result = IN_result;
                if(m_result != 0)
                {
                    std::stringstream ss;
                    ss << m_result;
                    send(cSocket, ss.str().c_str(), ss.str().size() + 1, 0); 
                }
            }
        }
    }

private:
    SOCKET sSocket;
    WSADATA w;
    SOCKET cSocket;
    sockaddr_in addr;
    bool m_close;
    unsigned int m_result;
};

P300Server * m_p300server = 0;

/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"

#define IS_PARAM_DOUBLE(pVal) (mxIsNumeric(pVal) && !mxIsLogical(pVal) &&\
!mxIsEmpty(pVal) && !mxIsSparse(pVal) && !mxIsComplex(pVal) && mxIsDouble(pVal))

static void mdlInitializeSizes(SimStruct *S)
{

    ssSetNumSFcnParams(S, 1);  /* Number of expected parameters */

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    if (!ssSetNumInputPorts(S, 1)) return;
	ssSetInputPortWidth(S, 0, 2);
	ssSetInputPortDirectFeedThrough(S, 0, 1);
	ssSetInputPortRequiredContiguous(S,0,1);
    
    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 4);

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0); 
    ssSetNumModes(S, 0); 
    ssSetNumNonsampledZCs(S, 0);

    ssSetSimStateCompliance(S, USE_CUSTOM_SIM_STATE);

    ssSetOptions(S, 0);
}


static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, FIXED_IN_MINOR_STEP_OFFSET);
    ssSetModelReferenceSampleTimeDefaultInheritance(S);
}

#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START) 
static void mdlStart(SimStruct *S)
{
    delete m_p300server;
    m_p300server = new P300Server(4242);
    th = new boost::thread(boost::bind(&P300Server::GetFlashIDThread, m_p300server));

    IN_mode = mxGetPr(ssGetSFcnParam(S,0))[0];
	debug << "MODE " << IN_mode << std::endl;

	IN_stop = false;
	
    IN_result = 0;
    IN_flash  = 0;
	IN_flashing = false;

    if(IN_mode == 1)
    {
        currTargetIdx = 0;
        currTarget = targets[currTargetIdx];
    }
}                                            
#endif /*  MDL_START */


static void mdlOutputs(SimStruct *S, int_T tid)
{
	const real_T* u  = ssGetInputPortRealSignal(S,0);
	unsigned int result = (unsigned int)u[1];
	if( result != IN_result )
	{
		debug << "NEW RESULT " << result << std::endl;
		IN_result = result;
        if(IN_result != 0)
        {
            currTargetIdx++;
            if(currTargetIdx < 6)
            {
                currTarget = targets[currTargetIdx];
            }
            else
            {
                currTarget = 0;
            }
        }
	}
	
	bool stop = (bool)u[0];
	if(stop != IN_stop)
	{
		IN_stop = stop;
		debug << "STOP  " << stop << std::endl;
	}

    if(IN_flash != 0)
    {
        real_T * y = ssGetOutputPortRealSignal(S, 0);
		if(!IN_flashing)
		{
			y[0] = 36;
			for(int i = 1; i < 4; ++i)
			{
				y[i] = 0;
			}
			IN_flashing = true;
		}
		else
		{
			y[0] = 0; /* flash X */
			y[1] = IN_flash; /* flash Y */
			y[2] = IN_flash; /* flash */
			if(IN_mode == 1 && IN_flash == currTarget)
			{
				y[3] = 1;
			}
			debug << "FLASH " << IN_flash << std::endl;
		}
        IN_flash = 0;
    }
    else
    {
        real_T * y = ssGetOutputPortRealSignal(S, 0);
        for(int i = 0; i < 4; ++i)
        {
            y[i] = 0;
        }
    }
	
	
    UNUSED_ARG(tid);                             
}                                                

#ifdef MATLAB_MEX_FILE

static void mdlTerminate(SimStruct *S)
{
}                                              

#endif

#ifdef  MATLAB_MEX_FILE    
#include "simulink.c"      
#else
#include "cg_sfun.h"       
#endif

