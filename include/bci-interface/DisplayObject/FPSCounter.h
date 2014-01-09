#ifndef _H_FSPCOUNTER_H_
#define _H_FSPCOUNTER_H_

#include <bci-interface/DisplayObject/TextObject.h>

#include <boost/shared_ptr.hpp>

namespace bciinterface
{

struct FPSCounterImpl;

class FPSCounter : public TextObject
{
public:
    FPSCounter(sf::Font & font);

    void Display(sf::RenderTarget * app, unsigned int frameCount, sf::Clock & clock);
private:
    boost::shared_ptr<FPSCounterImpl> impl;
};

}  // namespace bciinterface

#endif
