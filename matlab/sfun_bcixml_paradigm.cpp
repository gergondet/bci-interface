/*  File    : sfun_bcixml_paradigm.cpp
 *  Abstract:
 *
 *     Interface between a remote P300 interface and matlab processing laptop 
 *      
 */

#include <fstream>
#include <sstream>
#include <windows.h>
#include <winsock.h>
#include <boost/thread.hpp>
#include <stdlib.h>

//#include "sfun_p300_rand_buffers.h"

#pragma comment (lib, "libboost_thread-vc90-mt-1_47.lib")
#pragma comment (lib, "ws2_32.lib")

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  sfun_bcixml_paradigm

struct m_io 
{
public:
    m_io() : IN_STOP(0), IN_ID(0), OUT_Init(0), OUT_FlashID(0), OUT_Target(0), OUT_SelectedID(0)
    {}

    unsigned int IN_STOP;
    unsigned int IN_ID;
    unsigned int OUT_Init;
    unsigned int OUT_FlashID;
    unsigned int OUT_Target;
    unsigned int OUT_SelectedID;
};


struct m_params
{
    m_params() : PARAM_mode(1), PARAM_FlashTime(60000), PARAM_DarkTime(10000), 
                 PARAM_ModelFreq(64), PARAM_TicksBeforeNext(5),
                 NB_PROCESSING_TARGETS(36), NB_TARGETS(16)
    {
        PARAM_TrainingWord.push_back(1);
        PARAM_TrainingWord.push_back(9);
        PARAM_TrainingWord.push_back(12);
        PARAM_TrainingWord.push_back(7);
        PARAM_TrainingWord.push_back(14);
    }

    unsigned int PARAM_mode; /* 1 : training mode ,  2 : free mode */
    double PARAM_FlashTime;  /* us */
    double PARAM_DarkTime;  /* us */
    double PARAM_ModelFreq;
    unsigned int PARAM_TicksBeforeNext;
    std::vector<unsigned int> PARAM_TrainingWord;
    
    unsigned int NB_PROCESSING_TARGETS;
    unsigned int NB_TARGETS;
};

enum P300State
{
    WAIT_FOR_RESUME = 0,
    INITIALIZE,
    STIMULATE,
    SHOW_SELECTION
};

class P300Server
{
public:
    m_io io;
    m_params params;
    P300Server(unsigned short port) 
    : has_client(false), m_close(false), 
      m_state(WAIT_FOR_RESUME), m_idx(0), 
      TRAIN_INIT(15,0), FREE_INIT(15, 0), 
      m_idx_limit(0), m_train_idx(0), m_stimulation_idx(0), m_reset_flash(0),
      debug("debug.log")
    {
        TRAIN_STIMULATION.resize(1);
        FREE_STIMULATION.resize(1);
        for(int i = 0; i < 15; ++i)
        {
            for(int j = 1; j < 17; ++j)
            {
                TRAIN_STIMULATION[0].push_back(j);
            }
        }
        for(int i = 0; i < 5; ++i)
        {
            for(int j = 1; j < 17; ++j)
            {
                FREE_STIMULATION[0].push_back(j);
            }
        }
        TRAIN_INIT[5] = 1;
        TRAIN_INIT[6] = 2;
        TRAIN_INIT[8] = 2;
        TRAIN_INIT[9] = 4040016;
        TRAIN_INIT[11] = 4;
        FREE_INIT[5] = 1;
        FREE_INIT[6] = 2;
        FREE_INIT[8] = 2;
        FREE_INIT[9] = 4040016;
        FREE_INIT[11] = 4;

		srand(time(0));
        WSAStartup(0x0202, &w);
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = htonl (INADDR_ANY);
        sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        bind(sSocket, (LPSOCKADDR)&addr, sizeof(addr));
        th = new boost::thread(boost::bind(&P300Server::GetClient, this));
    }

    ~P300Server()
    {
        m_close = true;
        closesocket(cSocket);
        closesocket(sSocket);
        if(th) { th->interrupt(); delete th; }
        WSACleanup();
    }

    void SendFlashID(unsigned int flash)
    {
		if(has_client)
		{
			std::stringstream ss;
			ss << flash;
			int err = send(cSocket, ss.str().c_str(), ss.str().size() + 1, 0);
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
            delete th;
            th = new boost::thread(&P300Server::Resume, this);
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
            debug << "Switching state to INITIALIZE" << std::endl;
            if(params.PARAM_mode == 1)
            {
                m_idx_limit = 301;
            }
            else
            {
                m_idx_limit = 101;
            }
            m_state = INITIALIZE;
        }
    }

    void GetClient()
    {
        listen(sSocket, 42);
        cSocket =  accept(sSocket, 0, 0);
        debug << "Got a client" << std::endl;
		has_client = true;
        Resume();
    }

    void ComputeStates()
    {
        switch(m_state)
        {
            case WAIT_FOR_RESUME:
                break;
            case INITIALIZE:
                io.OUT_Init = FREE_INIT[m_idx];
                m_idx++;
                if(m_idx == m_idx_limit)
                {
				    debug << "Switching to state STIMULATE" << std::endl;
                    m_state = STIMULATE;
                    m_stimulation_idx = 0; //rand() % 256;
                    m_idx = 0;
                    if(params.PARAM_mode == 1)
                    {
                        m_idx_limit = 240;
                    }
                    else
                    {
                        m_idx_limit = 80;
                    }
                }
                break;
            case STIMULATE:
                if(io.IN_STOP != 0)
                {
					debug << "Switching state to SHOW_SELECTION" << std::endl;
                    m_state = SHOW_SELECTION;
                    m_idx = 0;
                    m_reset_flash = 0;
					io.OUT_FlashID = 0;
                    io.OUT_Target = 0;
                }
                else
                {
                    if((m_reset_flash % params.PARAM_TicksBeforeNext) == 0)
                    {
                        if(params.PARAM_mode == 1)
                        {
                            io.OUT_FlashID = TRAIN_STIMULATION[m_stimulation_idx][m_idx];
                            SendFlashID(io.OUT_FlashID);
                            if(io.OUT_FlashID == params.PARAM_TrainingWord[m_train_idx]) 
                            {
                                io.OUT_Target = 1;
                            }
                        }
                        else
                        {
                            io.OUT_FlashID = FREE_STIMULATION[m_stimulation_idx][m_idx];
                            SendFlashID(io.OUT_FlashID);
                        }
                        m_idx++;
				    	if(m_idx == m_idx_limit)
				    	{
				    		debug << "Stimulation over, waiting for STOP signal" << std::endl;
				    	}
                        m_reset_flash = 1;
                    }
                    else
                    {
                        io.OUT_FlashID = 0;
                        io.OUT_Target = 0;
                        m_reset_flash++;
                    }
                }
                break;
            case SHOW_SELECTION:
                if(io.IN_ID != 0)
                {
                    SendResult(io.IN_ID);
                    if(params.PARAM_mode == 1)
                    {
                        m_train_idx++;
                    }
                    if(m_train_idx < 5)
                    {
						debug << "Switching state to WAIT_FOR_RESUME" << std::endl;
                        m_state = WAIT_FOR_RESUME;
                    }
                }
                break;
            default:
                break;
        }
    }

private:
    SOCKET sSocket;
    WSADATA w;
    SOCKET cSocket;
    sockaddr_in addr;
	bool has_client;
    bool m_close;
    P300State m_state;
    unsigned int m_idx;
    unsigned int m_idx_limit;
    unsigned int m_train_idx;
    unsigned int m_stimulation_idx;
    unsigned int m_reset_flash;
    std::ofstream debug;
    boost::thread * th;
    std::vector<unsigned int> TRAIN_INIT;
    std::vector<unsigned int> FREE_INIT;
    std::vector< std::vector<unsigned int> > TRAIN_STIMULATION;
    std::vector< std::vector<unsigned int> > FREE_STIMULATION;
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

    ssSetNumSFcnParams(S, 3);  /* Number of expected parameters */

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    if (!ssSetNumInputPorts(S, 2)) return;
    for(int i = 0; i < 2; ++i)
    {
        ssSetInputPortWidth(S, i, 1);
        ssSetInputPortDirectFeedThrough(S, i, 1);
        ssSetInputPortRequiredContiguous(S,i,1);
    }
    
    if (!ssSetNumOutputPorts(S, 4)) return;
    for(int i = 0; i < 4; ++i)
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
    P300Server * p300server = new P300Server(4242);
    ssGetPWork(S)[0] = (void *) p300server;

    p300server->params.PARAM_mode = mxGetPr(ssGetSFcnParam(S,0))[0];
    p300server->params.PARAM_FlashTime = mxGetPr(ssGetSFcnParam(S,1))[0]*1000;
    p300server->params.PARAM_DarkTime = mxGetPr(ssGetSFcnParam(S,2))[0]*1000;
    p300server->params.PARAM_TicksBeforeNext = ceil((p300server->params.PARAM_FlashTime+p300server->params.PARAM_DarkTime)*p300server->params.PARAM_ModelFreq/1e6);
}                                            
#endif /*  MDL_START */


static void mdlOutputs(SimStruct *S, int_T tid)
{
    P300Server * p300server = (P300Server *) ssGetPWork(S)[0];

    const real_T * u1  = ssGetInputPortRealSignal(S,0);
    unsigned int stop = (unsigned int)u1[0];
    if( stop != p300server->io.IN_STOP )
    {
        p300server->io.IN_STOP = stop;
    }
    const real_T * u2  = ssGetInputPortRealSignal(S,1);
    unsigned int ID = (unsigned int)u2[0];
    if( ID != p300server->io.IN_ID)
    {
        p300server->io.IN_ID = ID;
    }

    p300server->ComputeStates();

	real_T * y = ssGetOutputPortRealSignal(S,0);
    y[0] = p300server->io.OUT_Init;
	y = ssGetOutputPortRealSignal(S,1);
    y[0] = p300server->io.OUT_FlashID;
	y = ssGetOutputPortRealSignal(S,2);
    y[0] = p300server->io.OUT_Target;
	y = ssGetOutputPortRealSignal(S,3);
    y[0] = ID;
    
    UNUSED_ARG(tid);                             
}                                                

#ifdef MATLAB_MEX_FILE

static void mdlTerminate(SimStruct *S)
{
    P300Server * p300server = (P300Server *) ssGetPWork(S)[0];
    delete p300server;
}                                              

#endif

#ifdef  MATLAB_MEX_FILE    
#include "simulink.c"      
#else
#include "cg_sfun.h"       
#endif

