#ifndef _H_P300OBJECT_H_
#define _H_P300OBJECT_H_

#include <bci-interface/DisplayObject.h>

#include <boost/shared_ptr.hpp>

namespace bciinterface
{

struct P300ObjectImpl;

class P300Object : public DisplayObject
{
public:
    /* active_time : time an object is displayed in milliseconds
       x,y : center coordinate of the object
       size_x, size_y : size of the object
       r,g,b : color */
    P300Object(unsigned int active_time, unsigned int x, unsigned int y, unsigned int size_x, unsigned int size_y, unsigned char r, unsigned char g, unsigned char b);

    void Display(sf::RenderWindow * app, unsigned int frameCount, sf::Clock & clock);

    void Highlight();

    void Unhighlight();
private:
    boost::shared_ptr<P300ObjectImpl> m_impl;
};

} // namespace bciinterface

#endif

