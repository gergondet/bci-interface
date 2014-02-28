#ifndef _H_SQUARE_FUNCTION_H_
#define _H_SQUARE_FUNCTION_H_

#include <vector>

#include <bci-interface/BCIAPI.h>

namespace bciinterface
{

/*! For a given desired stimuli frequency and screen frequency 
    returns frame sequence */
BCIINTERFACE_API bool squarefunction(int frequency, int screenFrequency, std::vector< std::pair<int,int> > & seq);

} //namespace bciinterface

#endif

