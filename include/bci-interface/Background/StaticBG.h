/* A BackgroundSprite implementation with a static image */

#include <bci-interface/Background.h>

#include <boost/shared_ptr.hpp>
#include <string>

#ifndef _H_STATICBG_H_
#define _H_STATICBG_H_

namespace bciinterface
{

struct StaticBGImpl;

class StaticBG : public Background
{
public:
    BCIINTERFACE_API StaticBG(const std::string & src_file, 
        unsigned int wwidth, unsigned int wheight, unsigned int iwidth = 0, unsigned int iheight = 0);

    BCIINTERFACE_API void UpdateLoop();

    BCIINTERFACE_API void Close();

    BCIINTERFACE_API void Draw(sf::RenderTarget * app);

    BCIINTERFACE_API void ChangeImage(const std::string & new_src);

private:
    boost::shared_ptr<StaticBGImpl> m_impl;
};

}

#endif

