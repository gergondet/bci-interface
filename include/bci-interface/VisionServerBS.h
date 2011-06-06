/* A BackgroundSprite implementation for the vision_server */

#include <bci-interface/BackgroundSprite.h>

#include <boost/shared_ptr.hpp>
#include <string>

#ifndef _H_VISIONSERVERBS_H_
#define _H_VISIONSERVERBS_H_

namespace bciinterface
{

struct VisionServerBSImpl;

class VisionServerBS : public BackgroundSprite
{
public:
    VisionServerBS(const std::string & vs_name, unsigned short vs_port, unsigned int width, unsigned int height);

    void UpdateLoop();

    void Close();

    sf::Sprite * GetSprite();

private:
    boost::shared_ptr<VisionServerBSImpl> m_impl;
};

}

#endif

