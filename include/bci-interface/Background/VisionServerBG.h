/* A BackgroundSprite implementation for the vision_server */

#include <bci-interface/BackgroundSprite.h>

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
    VisionServerBG(const std::string & vs_name, unsigned short vs_port, unsigned int width, unsigned int height);

    void UpdateLoop();

    void Close();

    void Draw(sf::RenderWindow * app);

private:
    boost::shared_ptr<VisionServerBGImpl> m_impl;
};

}

#endif

