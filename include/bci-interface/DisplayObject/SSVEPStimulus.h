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
    /*! \brief Constructor: x and y are stimulus centers, can provide an image as graphic for the stim */
    SSVEPStimulus(int frequency, int screenFrequency, float x, float y, float size_x, float size_y, const std::string & tx, const std::string & tx_hl);

    /*! \brief Constructor: x and y are stimulus centers, can provide an image as graphic for the stim */
    SSVEPStimulus(int frequency, int screenFrequency, float x, float y, float size_x, float size_y, const std::string & tx, const std::string & tx_hl, const std::string & unactive_tx, const std::string & unactive_tx_hl);

    /*! \brief Constructor: x and y are stimulus centers, can provide an image as graphic for the stim, size is set to the image size */
    SSVEPStimulus(int frequency, int screenFrequency, float x, float y, const std::string & tx, const std::string & tx_hl);

    /*! \brief Constructor: x and y are stimulus centers, can provide an image as graphic for the stim, 
                                size is set to the image size, second image is the unactive sprite */
    SSVEPStimulus(int frequency, int screenFrequency, float x, float y, const std::string & tx, const std::string & tx_hl, const std::string & unactive_tx, const std::string & unactive_tx_hl);

    /*! \brief Constructor: x and y are stimulus centers, circle shape */
    SSVEPStimulus(int frequency, int screenFrequency, float x, float y, float radius, int r, int g, int b, int a);

    /*! \brief Constructor: x and y are stimulus centers */ 
    SSVEPStimulus(int frequency, int screenFrequency, float x, float y, float size_x, float size_y, int r, int g, int b, int a);

    /*! \brief Constructor: for OpenGL SSVEPStimulus */
    SSVEPStimulus(int frequency, int screenFrequency);

    /* \brief Get flickering frequency */
    int GetFrequency();

    /* \brief Change flickering frequency */
    bool ChangeFrequency(int frequency, int screenFrequency);

    /*! \brief Highlight the stimulus by changing outline color */
    virtual void Highlight();

    /*! \brief Unhighlight the stimulus */
    virtual void Unhighlight();

    /*Change position of the square*/
    void Move(float diffX, float diffY);

    /* Set absolute position */
    void SetPosition(float X, float Y);

    /* Set absolute rotation */
    void SetRotation(float deg_angle);

    /* Set scale */
    void SetScale(float sX, float sY);

    /* Resize the stimulus */
    void Resize(float size_x, float size_y);

    /* Display function */
    virtual void Display(sf::RenderWindow * app, unsigned int frameCount, sf::Clock & clock);

    virtual bool DrawWithGL();

    /* Should draw active or inactive? Use if you implement Display */
    bool DisplayActive(unsigned int frameCount);


private:
    boost::shared_ptr<SSVEPStimulusImpl> m_impl;
};

}

#endif
