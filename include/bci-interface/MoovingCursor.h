#ifndef _H_MOOVINGCURSOR_H_
#define _H_MOOVINGCURSOR_H_

#include <boost/shared_ptr.hpp>
#include <SFML/Graphics.hpp>

#ifdef WIN32
#define SSVEPINTERFACE_API __declspec(dllexport)
#else
#define SSVEPINTERFACE_API
#endif

namespace bciinterface
{

struct MoovingCursorImpl;

class MoovingCursor
{
public:
    //Constructor
    SSVEPINTERFACE_API MoovingCursor(float x_init=0, float y_init=0, float size=100, float x_fin=0, int r=0, int g=0, int b=0, int a=255);

    void SetCursorDisplay(bool const display);

    bool CursorDisplay();

    float GetfinalPosX();

    sf::Shape * GetCursor();

    void SetCursorX(float posX);

private:
    boost::shared_ptr<MoovingCursorImpl> m_mvcrsimpl;

};

}

#endif
