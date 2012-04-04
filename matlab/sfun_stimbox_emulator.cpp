/*  File    : sfun_keytest_trigger.cpp
 *  Abstract:
 *
 *    Used to add a remote trigger channel to bci datas
 *
 */

#include <fstream>
#include <sstream>
#include <windows.h>

#pragma comment (lib, "winmm.lib")

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  sfun_stimbox_emulator

unsigned int IN_class = 0;
unsigned int IN_trigger = 0;

std::ofstream debug("debug.log");

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

    if (!ssSetNumInputPorts(S, 2)) return;
	for(int i = 0; i < 2; ++i)
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
    IN_class = 0;
	IN_trigger = 0;
}
#endif /*  MDL_START */


static void mdlOutputs(SimStruct *S, int_T tid)
{
	const real_T * i_class = ssGetInputPortRealSignal(S, 0);
	/* During training, class is used to drive the training */
	if(IN_class != i_class[0])
	{
		IN_class = i_class[0];
		switch(IN_class)
		{
			case 0:
				PlaySound("data/relax.wav", NULL, SND_ASYNC);
				break;
			case 1:
				PlaySound("data/up.wav", NULL, SND_ASYNC);
				break;
			case 2:
				PlaySound("data/right.wav", NULL, SND_ASYNC);
				break;
			case 3:
				PlaySound("data/down.wav", NULL, SND_ASYNC);
				break;
			case 4:
				PlaySound("data/left.wav", NULL, SND_ASYNC);
				break;
			default:
				break;
		}
	}
	
	
	const real_T * i_trigger = ssGetInputPortRealSignal(S, 1);
	IN_trigger = i_trigger[0]; /* trigger not really important */

    real_T * y = ssGetOutputPortRealSignal(S,0);
    y[0] = IN_class;

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

