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
#define SSVEPINTERFACE_API __declspec(dllexport)
#else
#define SSVEPINTERFACE_API 
#endif

namespace bciinterface
{

struct FlickeringSquareImpl;

class FlickeringSquare
{
public:
    /*! \brief Constructor */ 
    SSVEPINTERFACE_API FlickeringSquare(int frequency, int screenFrequency, float x = 0, float y = 0, float size_x = 100, float size_y = 100, int r = 255, int g = 0, int b = 0, int a = 255, bool fill = true);

    /*! \brief Circle constructor */
    SSVEPINTERFACE_API FlickeringSquare(int frequency, int screenFrequency, float x = 100, float y = 100, float radius = 50, int r = 255, int g = 0, int b = 0, int a = 255, bool fill = true);

    /* \brief Change flickering frequency */
    bool ChangeFrequency(int frequency, int screenFrequency);

    /*! \brief Function to be called in the main program loop, make the flicker happens ! */
    void UpdateForNewFrame(unsigned int frameIndex);

    /*! \brief True: display square, flase: do not display square */
    void SetSquareDisplay(bool const display);

    bool SquareDisplay();

    /*! \brief Get the actual shape */
    sf::Shape * GetShape();

    sf::Shape * GetBlackShape();

    /*! \brief Hight the square by changing outline color */
    void Highlight();

    /*! \brief Change outline color back to normal */
    void Unhighlight();

    /*Change position of the square*/
    void SetSquareX(float positionX);

    /*Get Radius for Circle, size_x for Square*/
    float GetRadius();


private:
    boost::shared_ptr<FlickeringSquareImpl> m_flsqimpl;
};

}

#endif
