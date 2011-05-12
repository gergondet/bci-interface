/*  File    : sfun_ssvep_trigger.cpp
 *  Abstract:
 *
 *     Interface between a remote ssvep interface and matlab processing laptop
 *     putting trigger when a specific event happens
 *
 */

#include <fstream>
#include <sstream>
#include <windows.h>
#include <winsock.h>
#include <boost/thread.hpp>

#pragma comment (lib, "boost_thread-vc90-mt.lib")
#pragma comment (lib, "ws2_32.lib")

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  sfun_ssvep_trigger

unsigned int IN_result = 0;
unsigned int OUT_result = 0;
unsigned int IN_flash = 0;
bool IN_stop = false;

double IN_mode = 1; /* 1 : training mode ,  2 : free mode */

boost::thread * th = 0;

std::ofstream debug("debug.log");

class SSVEPServer
{
public:
    SSVEPServer(unsigned short port) : has_client(false), m_close(false) , m_result(0)
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
        if(th) { th->interrupt(); }
        closesocket(cSocket);
        closesocket(sSocket);
        WSACleanup();
    }

    void SendFlashID(unsigned int flashID)
    {
		if(has_client)
		{
			std::stringstream ss;
			ss << flashID;
			int err = send(cSocket, ss.str().c_str(), ss.str().size() + 1, 0);
			debug << "send return " << err << " for flashID " << flashID << std::endl;
		}
    }

    void SendResult(unsigned int result)
    {
		if(has_client)
		{
			std::stringstream ss;
			ss << result;
			int err = send(cSocket, ss.str().c_str(), ss.str().size() + 1, 0);
			debug << "send return " << err << " for result " << result << std::endl;
		}
    }

    void Resume()
    {
		debug << "Resume waiting for client" << std::endl;
		while(!has_client) { Sleep(100); }
        debug << "Waiting for resume message" << std::endl;
        char buffer[256];
        int err = recv(cSocket, buffer, 256, 0);
        debug << "Got resume message " << buffer << std::endl;
        std::string stop(buffer);
        if(stop == "resume")
        {
            debug << "Switching IN_stop to false" << std::endl;
            IN_stop = false;
            OUT_result = 0;
        }
    }

    void GetClient()
    {
        listen(sSocket, 1);
        cSocket =  accept(sSocket, 0, 0);
        debug << "Got a client" << std::endl;
		has_client = true;
    }

private:
    SOCKET sSocket;
    WSADATA w;
    SOCKET cSocket;
    sockaddr_in addr;
	bool has_client;
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

    if (!ssSetNumInputPorts(S, 3)) return;
    for(int i = 0; i < 3; ++i)
    {
        ssSetInputPortWidth(S, i, 1);
        ssSetInputPortDirectFeedThrough(S, i, 1);
        ssSetInputPortRequiredContiguous(S,i,1);
    }

    if (!ssSetNumOutputPorts(S, 2)) return;
    for(int i = 0; i < 2; ++i)
    {
        ssSetOutputPortWidth(S, i, 1);
    }

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
    OUT_result = 0;
    IN_flash  = 0;
    IN_stop = false;

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
            th = new boost::thread(boost::bind(&P300Server::Resume, m_p300server));
        }
    }
    if(IN_mode == 1)
    {
        /* In training copy the input to the output */
        OUT_result = IN_result;
    }
    else
    {
        /* Else hold on to the non-zero value until system unpaused */
        if(IN_result != 0)
        {
            OUT_result = IN_result;
        }
    }
    real_T * y = ssGetOutputPortRealSignal(S,1);
    y[0] = OUT_result;

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

    if(IN_mode == 2)
    {
        u = ssGetInputPortRealSignal(S, 2);
        bool stop = (bool)u[0];
        if( IN_stop != stop && stop )
        {
            IN_stop = stop;
        }
        if( IN_stop )
        {
            real_T * y = ssGetOutputPortRealSignal(S,0);
            y[0] = true;
        }
        else
        {
            real_T * y = ssGetOutputPortRealSignal(S,0);
            y[0] = false;
        }
    }
    else
    {
        u = ssGetInputPortRealSignal(S, 2);
        bool stop = (bool)u[0];
        IN_stop = stop;
        real_T * y = ssGetOutputPortRealSignal(S,0);
        y[0] = IN_stop;
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

