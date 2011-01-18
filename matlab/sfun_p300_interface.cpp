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
unsigned int IN_flash = 0;

double IN_mode = 1; /* 1 : training mode ,  2 : free mode */

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
        closesocket(cSocket);
        closesocket(sSocket);
        WSACleanup();
    }

    void SendFlashID(unsigned int flashID)
    {
        std::stringstream ss;
        ss << flashID;
        send(cSocket, ss.str().c_str(), ss.str().size() + 1, 0);
    }

    void SendResult(unsigned int result)
    {
        std::stringstream ss;
        ss << result;
        send(cSocket, ss.str().c_str(), ss.str().size() + 1, 0);
    }

    void GetClient()
    {
		listen(sSocket, 1);
        cSocket =  accept(sSocket, 0, 0);
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

    if (!ssSetNumInputPorts(S, 2)) return;
    for(int i = 0; i < 2; ++i)
    {
    	ssSetInputPortWidth(S, i, 1);
	    ssSetInputPortDirectFeedThrough(S, i, 1);
    	ssSetInputPortRequiredContiguous(S,i,1);
    }
    
    if (!ssSetNumOutputPorts(S, 0)) return;

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
    th = new boost::thread(boost::bind(&P300Server::GetClient, m_p300server));

    IN_mode = mxGetPr(ssGetSFcnParam(S,0))[0];
	debug << "MODE " << IN_mode << std::endl;

    IN_result = 0;
    IN_flash  = 0;

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
	unsigned int result = (unsigned int)u[0];
	if( result != IN_result )
	{
		debug << "NEW RESULT " << result << std::endl;
		IN_result = result;
        if(IN_result != 0)
        {
            m_p300server->SendResult(IN_result);
        }
	}
	
    u = ssGetInputPortRealSignal(S, 1);
    unsigned int flash = (unsigned int)u[0];
    if( flash != IN_flash )
    {
        IN_flash = flash;
        if( flash != 0 )
        {
            debug << "FLASH " << flash << std::endl;
            m_p300server->SendFlashID(flash);
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

