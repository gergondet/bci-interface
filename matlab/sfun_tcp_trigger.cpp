/*  File    : sfun_udp_trigger.cpp
 *  Abstract:
 *
 *    Used to add a remote trigger channel to bci datas 
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
#define S_FUNCTION_NAME  sfun_udp_trigger

unsigned int OUT_trigger = 0;

boost::thread * th = 0;

std::ofstream debug("debug.log");

class UDPServer 
{
public:
    UDPServer(unsigned short port) : has_client(false), m_close(false) 
    {
        WSAStartup(0x0202, &w);
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = htonl (INADDR_ANY);
        sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        bind(sSocket, (LPSOCKADDR)&addr, sizeof(addr));
    }

    ~UDPServer()
    {
        m_close = true;
        if(th) { th->interrupt(); }
        closesocket(cSocket);
        closesocket(sSocket);
        WSACleanup();
    }

    void GetTrigger()
    {
        debug << "GetTrigger() waiting for client" << std::endl;
        debug << "Waiting for trigger message" << std::endl;
        char buffer[256];
        int err = recv(cSocket, buffer, 256, 0);
        OUT_trigger = 1;        
    }

    void GetClient()
    {
        listen(sSocket, 1);
        cSocket =  accept(sSocket, 0, 0);
        debug << "Got a client" << std::endl;
        while(!m_close)
        {
            GetTrigger();
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

UDPServer * m_udpserver = 0;

/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"

#define IS_PARAM_DOUBLE(pVal) (mxIsNumeric(pVal) && !mxIsLogical(pVal) &&\
!mxIsEmpty(pVal) && !mxIsSparse(pVal) && !mxIsComplex(pVal) && mxIsDouble(pVal))

static void mdlInitializeSizes(SimStruct *S)
{

    ssSetNumSFcnParams(S, 0);  /* Number of expected parameters */

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    if (!ssSetNumInputPorts(S, 0)) return;

    if (!ssSetNumOutputPorts(S, 1)) return;
    for(int i = 0; i < 1; ++i)
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
    delete m_udpserver;
    m_udpserver = new UDPServer(4242);
    th = new boost::thread(boost::bind(&UDPServer::GetClient, m_udpserver));

    OUT_trigger = 0;

}                                            
#endif /*  MDL_START */


static void mdlOutputs(SimStruct *S, int_T tid)
{
    real_T * y = ssGetOutputPortRealSignal(S,1);
    y[0] = OUT_trigger;
    if(OUT_trigger) { OUT_trigger = 0; }
    
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

