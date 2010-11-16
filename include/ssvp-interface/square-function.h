#ifndef _H_SQUARE_FUNCTION_H_
#define _H_SQUARE_FUNCTION_H_

#include <vector>

namespace ssvpinterface
{

/*! For a given desired stimuli frequency and screen frequency 
    returns frame sequence */
bool squarefunction(int frequency, int screenFrequency, std::vector< std::pair<int,int> > & seq);

} //namespace ssvpinterface

#endif

