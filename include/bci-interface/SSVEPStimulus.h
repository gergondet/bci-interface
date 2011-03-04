/*! \brief SSVEPStimulus class 
 * \author Pierre Gergondet
 * Copyright: JRL, CNRS-AIST
 * Date: 16/11/2010
 */

#ifndef _H_SSVEPSTIMULUS_H_
#define _H_SSVEPSTIMULUS_H_

#include <bci-interface/DisplayObject.h>

#include <boost/shared_ptr.hpp>

#ifdef WIN32
#define SSVEPINTERFACE_API __declspec(dllexport)
#else
#define SSVEPINTERFACE_API 
#endif

namespace bciinterface
{

struct SSVEPStimulusImpl;

class SSVEPStimulus : 
{
public:
    /*! \brief Constructor */ 
    SSVEPINTERFACE_API SSVEPStimulus(int frequency, int screenFrequency, float x = 0, float y = 0, float size_x = 100, float size_y = 100, int r = 255, int g = 0, int b = 0, int a = 255);

    /* \brief Change flickering frequency */
    bool ChangeFrequency(int frequency, int screenFrequency);

    /*! \brief Highlight the stimulus by changing outline color */
    void Highlight();

    /*! \brief Unhighlight the stimulus */
    void Unhighlight();

    /*Change position of the square*/
    void Move(float diffX, float diffY);

    /* Display function */
    void Display(sf::RenderWindow * app, unsigned int frameCount = 0);

private:
    boost::shared_ptr<SSVEPStimulusImpl> m_impl;
};

}

#endif
