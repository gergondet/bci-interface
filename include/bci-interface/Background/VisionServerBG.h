/* A BackgroundSprite implementation for the vision_server */

#include <bci-interface/Background.h>

#include <boost/shared_ptr.hpp>
#include <string>

#ifndef _H_VISIONSERVERBG_H_
#define _H_VISIONSERVERBG_H_

namespace bciinterface
{

struct VisionServerBGImpl;

class VisionServerBG : public Background
{
public:
    VisionServerBG(const std::string & vs_name, unsigned short vs_port, unsigned int width, unsigned int height,
                    unsigned int wwidth, unsigned int wheight, unsigned int iwidth = 0, unsigned int iheight = 0);

    void UpdateLoop();

    void Close();

    void Draw(sf::RenderWindow * app);

private:
    boost::shared_ptr<VisionServerBGImpl> m_impl;
};

}

#endif

