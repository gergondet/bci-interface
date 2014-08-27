/*! \brief SSVEPStimulus class 
 * \author Pierre Gergondet
 * Copyright: JRL, CNRS-AIST
 * Date: 16/11/2010
 */

#ifndef _H_SSVEPSTIMULUS_H_
#define _H_SSVEPSTIMULUS_H_

#include <bci-interface/DisplayObject.h>

#include <boost/shared_ptr.hpp>
#include <stdint.h>

namespace bciinterface
{

struct SSVEPStimulusImpl;

class SSVEPStimulus : public DisplayObject
{
public:
    /*! \brief Constructor: x and y are stimulus centers, can provide an image as graphic for the stim */
    BCIINTERFACE_API SSVEPStimulus(int frequency, int screenFrequency, float x, float y, float size_x, float size_y, const std::string & tx, const std::string & tx_hl);

    /*! \brief Constructor: x and y are stimulus centers, can provide an image as graphic for the stim */
    BCIINTERFACE_API SSVEPStimulus(int frequency, int screenFrequency, float x, float y, float size_x, float size_y, const std::string & tx, const std::string & tx_hl, const std::string & unactive_tx, const std::string & unactive_tx_hl);

    /*! \brief Constructor: x and y are stimulus centers, can provide an image as graphic for the stim, size is set to the image size */
    BCIINTERFACE_API SSVEPStimulus(int frequency, int screenFrequency, float x, float y, const std::string & tx, const std::string & tx_hl);

    /*! \brief Constructor: x and y are stimulus centers, can provide an image as graphic for the stim, 
                                size is set to the image size, second image is the unactive sprite */
    BCIINTERFACE_API SSVEPStimulus(int frequency, int screenFrequency, float x, float y, const std::string & tx, const std::string & tx_hl, const std::string & unactive_tx, const std::string & unactive_tx_hl);

    /*! \brief Constructor: x and y are stimulus centers, circle shape */
    BCIINTERFACE_API SSVEPStimulus(int frequency, int screenFrequency, float x, float y, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

    /*! \brief Constructor: x and y are stimulus centers */ 
    BCIINTERFACE_API SSVEPStimulus(int frequency, int screenFrequency, float x, float y, float size_x, float size_y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

    /*! \brief Constructor: Generic constructor for DisplayObject */
    BCIINTERFACE_API SSVEPStimulus(int frequency, int screenFrequency, DisplayObject * object, DisplayObject * inactive_object = 0);

    /*! \brief Constructor: for OpenGL SSVEPStimulus */
    BCIINTERFACE_API SSVEPStimulus(int frequency, int screenFrequency);

    /* \brief Get flickering frequency */
    BCIINTERFACE_API int GetFrequency();

    /* \brief Change flickering frequency */
    BCIINTERFACE_API bool ChangeFrequency(int frequency, int screenFrequency);

    /*! \brief Highlight the stimulus by changing outline color */
    BCIINTERFACE_API virtual void Highlight();

    /*! \brief Unhighlight the stimulus */
    BCIINTERFACE_API virtual void Unhighlight();

    /*Change position of the square*/
    BCIINTERFACE_API void Move(float diffX, float diffY);

    /* Set absolute position */
    BCIINTERFACE_API void SetPosition(float X, float Y);

    /* Set absolute rotation */
    BCIINTERFACE_API void SetRotation(float deg_angle);

    /* Set scale */
    BCIINTERFACE_API void SetScale(float sX, float sY);

    /* Resize the stimulus */
    BCIINTERFACE_API void Resize(float size_x, float size_y);

    /* Display function */
    BCIINTERFACE_API virtual void Display(sf::RenderTarget * app, unsigned int frameCount, sf::Clock & clock);

    BCIINTERFACE_API virtual bool DrawWithGL();

    /* Should draw active or inactive? Use if you implement Display */
    BCIINTERFACE_API bool DisplayActive(unsigned int frameCount);


private:
    boost::shared_ptr<SSVEPStimulusImpl> m_impl;
};

}

#endif
