/* A BackgroundSprite implementation fed from a unsigned char * buffer */

#include <bci-interface/Background.h>

#include <boost/shared_ptr.hpp>
#include <string>

#ifndef _H_BUFFERBG_H_
#define _H_BUFFERBG_H_

namespace bciinterface
{

struct BufferBGImpl;

class BufferBG : public Background
{
public:
    BCIINTERFACE_API BufferBG(unsigned int width, unsigned int height, 
        unsigned int wwidth, unsigned int wheight, unsigned int iwidth = 0, unsigned int iheight = 0);

    BCIINTERFACE_API void UpdateLoop();

    BCIINTERFACE_API void Close();

    BCIINTERFACE_API void Draw(sf::RenderTarget * app);

    BCIINTERFACE_API void UpdateFromBuffer_MONO(unsigned char * img);

    BCIINTERFACE_API void UpdateFromBuffer_RGB(unsigned char * img);

    BCIINTERFACE_API void UpdateFromBuffer_BGR8(unsigned char * img);

    BCIINTERFACE_API void UpdateFromBuffer_RGB8(unsigned char * img);

    BCIINTERFACE_API void SwitchColorMode();

    BCIINTERFACE_API virtual void SetSubRect(int left, int top, int width, int height);

private:
    boost::shared_ptr<BufferBGImpl> m_impl;
};

}

#endif

