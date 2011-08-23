/*! \brief SSVEPStimulus class 
 * \author Pierre Gergondet
 * Copyright: JRL, CNRS-AIST
 * Date: 16/11/2010
 */

#ifndef _H_SSVEPSTIMULUS_H_
#define _H_SSVEPSTIMULUS_H_

#include <bci-interface/DisplayObject.h>

#include <boost/shared_ptr.hpp>

namespace bciinterface
{

struct SSVEPStimulusImpl;

class SSVEPStimulus : public DisplayObject
{
public:
    /*! \brief Constructor: x and y are stimulus centers */ 
    SSVEPStimulus(int frequency, int screenFrequency, float x = 0, float y = 0, float size_x = 100, float size_y = 100, int r = 255, int g = 0, int b = 0, int a = 255);

    /* \brief Change flickering frequency */
    bool ChangeFrequency(int frequency, int screenFrequency);

    /*! \brief Highlight the stimulus by changing outline color */
    void Highlight();

    /*! \brief Unhighlight the stimulus */
    void Unhighlight();

    /*Change position of the square*/
    void Move(float diffX, float diffY);

    /* Set absolute position */
    void SetPosition(float X, float Y);

    /* Display function */
    void Display(sf::RenderWindow * app, unsigned int frameCount, sf::Clock & clock);

private:
    boost::shared_ptr<SSVEPStimulusImpl> m_impl;
};

}

#endif
