#ifndef _H_SSVEPSTIMULUSVECTOR_H_
#define _H_SSVEPSTIMULUSVECTOR_H_

#include <bci-interface/SSVEPStimulus.h>

#include <boost/thread.hpp>

namespace bciinterface
{

class SSVEPStimulusVector : public DisplayObject
{
public:
    SSVEPStimulusVector();

    ~SSVEPStimulusVector();

    void Display(sf::RenderWindow * app, unsigned int frameCount = 0);

private:
    std::vector<SSVEPStimulus *> m_stimuli;
};

}

#endif
