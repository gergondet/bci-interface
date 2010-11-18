/*! \brief FlickeringSquare class 
 * \author Pierre Gergondet
 * Copyright: JRL, CNRS-AIST
 * Date: 16/11/2010
 */

#ifndef _H_FLICKERINGSQUARE_H_
#define _H_FLICKERINGSQUARE_H_

#include <boost/shared_ptr.hpp>
#include <SFML/Graphics.hpp>

#ifdef WIN32
#define SSVPINTERFACE_API __declspec(dllexport)
#else
#define SSVPINTERFACE_API 
#endif

namespace ssvpinterface
{

struct FlickeringSquareImpl;

class FlickeringSquare
{
public:
    /*! \brief Constructor */ 
    SSVPINTERFACE_API FlickeringSquare(int frequency, int screenFrequency, float x = 0, float y = 0, float size = 100, int r = 255, int g = 0, int b = 0, int a = 255);

    /*! \brief Function to be called in the main program loop, make the flicker happens ! */
    void UpdateForNewFrame(unsigned int frameIndex);

    /*! \brief Get the actual shape when working with SFML */
    sf::Shape * GetShape();

private:
    boost::shared_ptr<FlickeringSquareImpl> m_flsqimpl;
};

}

#endif
