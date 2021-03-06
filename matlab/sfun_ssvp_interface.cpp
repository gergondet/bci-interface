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
#include <windows.h>

#pragma comment (lib, "winmm.lib")

int freqLED1 = -1;
int freqLED2 = -1;
int freqLED3 = -1;
int freqLED4 = -1;
int trainArrowIdx = -1;
bool flashOn = true;
bool crossOn = false;

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  sfun_ssvp_interface

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

    if (!ssSetNumInputPorts(S, 7)) return;
	for(int i = 0; i < 7; ++i)
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
      freqLED1 = -1;
      freqLED2 = -1;
      freqLED3 = -1;
      freqLED4 = -1;
      trainArrowIdx = -1;
      flashOn = true;
      crossOn = false;
	  
  }                                            
#endif /*  MDL_START */


static void mdlOutputs(SimStruct *S, int_T tid)
{
	const real_T* u  = ssGetInputPortRealSignal(S,0);
	int in_freqLED1 = (int)u[0];
	if( freqLED1 != in_freqLED1)
	{
		freqLED1 = in_freqLED1;
	}
	
	u  = ssGetInputPortRealSignal(S,1);
	int in_freqLED2 = (int)u[0];
	if( freqLED2 != in_freqLED2)
	{
		freqLED2 = in_freqLED2;
	}
	
	u  = ssGetInputPortRealSignal(S,2);
	int in_freqLED3 = (int)u[0];
	if( freqLED3 != in_freqLED3)
	{
		freqLED3 = in_freqLED3;
	}
	
	u  = ssGetInputPortRealSignal(S,3);
	int in_freqLED4 = (int)u[0];
	if( freqLED4 != in_freqLED4)
	{
		freqLED4 = in_freqLED4;
	}
	
	u = ssGetInputPortRealSignal(S,4);
	int in_trainArrowIdx = (int)u[0];
	if( trainArrowIdx != in_trainArrowIdx )
	{
		trainArrowIdx = in_trainArrowIdx;
        switch(trainArrowIdx)
        {
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
	
	u = ssGetInputPortRealSignal(S,5);
	int in_flashOn = (int)u[0];
	if( flashOn != in_flashOn )
	{
		flashOn = in_flashOn;
	}
	
	u = ssGetInputPortRealSignal(S,6);
	int in_crossOn = (int)u[0];
	if( crossOn != in_crossOn )
	{
		crossOn = in_crossOn;
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

