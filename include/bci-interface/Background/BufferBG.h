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
    BufferBG(unsigned int width, unsigned int height, 
        unsigned int wwidth, unsigned int wheight, unsigned int iwidth = 0, unsigned int iheight = 0);

    void UpdateLoop();

    void Close();

    void Draw(sf::RenderWindow * app);

    void UpdateFromBuffer_MONO(unsigned char * img);

    void UpdateFromBuffer_RGB(unsigned char * img);

    void UpdateFromBuffer_BGR8(unsigned char * img);

    void SwitchColorMode();

    void SetSubRect(int left, int top, int width, int height);

private:
    boost::shared_ptr<BufferBGImpl> m_impl;
};

}

#endif

