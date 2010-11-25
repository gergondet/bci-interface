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

enum ArrowPosition
{
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};
typedef enum ArrowPosition ArrowPosition;

class FlickeringSquare
{
public:
    /*! \brief Constructor */ 
    SSVPINTERFACE_API FlickeringSquare(int frequency, int screenFrequency, float x = 0, float y = 0, ArrowPosition arrowPos = UP, float size = 100, int r = 255, int g = 0, int b = 0, int a = 255, bool fill = true);

    /* \brief Change flickering frequency */
    bool ChangeFrequency(int frequency, int screenFrequency);

    /*! \brief Function to be called in the main program loop, make the flicker happens ! */
    void UpdateForNewFrame(unsigned int frameIndex);

    /*! \brief True: display square, flase: do not display square */
    void SetSquareDisplay(bool const display);

    bool SquareDisplay();

    /*! \brief Get the actual shape */
    sf::Shape * GetShape();

    /* True: display arrow, false: do not display arrow */
    void SetArrowDisplay(bool const display);
    
    bool ArrowDisplay();

    /*! \brief Get the arrow */
    sf::Sprite * GetArrow();

private:
    boost::shared_ptr<FlickeringSquareImpl> m_flsqimpl;
};

}

#endif
