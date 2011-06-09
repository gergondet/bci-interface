#ifndef _H_SQUARE_FUNCTION_H_
#define _H_SQUARE_FUNCTION_H_

#include <vector>

namespace bciinterface
{

/*! For a given desired stimuli frequency and screen frequency 
    returns frame sequence */
bool squarefunction(int frequency, int screenFrequency, std::vector< std::pair<int,int> > & seq);

} //namespace bciinterface

#endif

