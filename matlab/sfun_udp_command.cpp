/*  File    : sfun_keytest_trigger.cpp
 *  Abstract:
 *
 *    Used to add a remote trigger channel to bci datas
 *
 */

#include <fstream>
#include <sstream>
#include <windows.h>
#include <winsock.h>

#pragma comment (lib, "ws2_32.lib")

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  sfun_udp_command

class UDPClient
{
public:
    UDPClient(const char * srv_name, unsigned short port)
    {
        WSAStartup(0x0202, &w);
        sendaddr.sin_family = AF_INET;
        sendaddr.sin_port = htons(port);
        sendaddr.sin_addr.s_addr = inet_addr (srv_name);
        sSocket = socket(AF_INET, SOCK_DGRAM, 0);
    }

    ~UDPClient()
    {
        closesocket(sSocket);
        WSACleanup();
    }

    void SendMessage()
	{
		/* Send IN_cmd to remote server */
		std::stringstream ss;
		ss << IN_cmd;
		sendto(sSocket, ss.str().c_str(), ss.str().length() + 1, 0, (struct sockaddr *)&sendaddr, sizeof(sendaddr));
	}

    void SetInCmd(int in)
    {
        IN_cmd = in;
    }

private:
    SOCKET sSocket;
    WSADATA w;
    sockaddr_in sendaddr;
    int IN_cmd;
};

/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"

#define IS_PARAM_DOUBLE(pVal) (mxIsNumeric(pVal) && !mxIsLogical(pVal) &&\
!mxIsEmpty(pVal) && !mxIsSparse(pVal) && !mxIsComplex(pVal) && mxIsDouble(pVal))

static void mdlInitializeSizes(SimStruct *S)
{

    ssSetNumSFcnParams(S, 2);  /* Number of expected parameters */

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    if (!ssSetNumInputPorts(S, 1)) return;
	for(int i = 0; i < 1; ++i)
    {
        ssSetInputPortWidth(S, i, 1);
		ssSetInputPortDirectFeedThrough(S, i, 1);
		ssSetInputPortRequiredContiguous(S,i,1);
    }

    if (!ssSetNumOutputPorts(S, 1)) return;
    for(int i = 0; i < 1; ++i)
    {
        ssSetOutputPortWidth(S, i, 1);
    }

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 1); /* space for 1 pointer */
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
    char server_ip[32];
    mxGetString(ssGetSFcnParam(S,0), server_ip, 30); 

    short server_port = (short)(mxGetPr(ssGetSFcnParam(S,1))[0]);

    UDPClient * udp_client = new UDPClient(server_ip, server_port);
    ssGetPWork(S)[0] = (void*)udp_client;    
}
#endif /*  MDL_START */


static void mdlOutputs(SimStruct *S, int_T tid)
{
    UDPClient * udp_client = static_cast<UDPClient *>(ssGetPWork(S)[0]);

	const real_T * x = ssGetInputPortRealSignal(S, 0);
	udp_client->SetInCmd(x[0]);
	udp_client->SendMessage();

    real_T * y = ssGetOutputPortRealSignal(S,0);
    y[0] = x[0];

    UNUSED_ARG(tid);
}

#ifdef MATLAB_MEX_FILE

static void mdlTerminate(SimStruct *S)
{
    UDPClient * udp_client = static_cast<UDPClient *>(ssGetPWork(S)[0]);
    delete udp_client;
}

#endif

#ifdef  MATLAB_MEX_FILE
#include "simulink.c"
#else
#include "cg_sfun.h"
#endif

