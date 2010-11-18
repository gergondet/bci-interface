#include <ssvp-interface/square-function.h>

#include <cmath>
#include <cstdlib>
#include <ctime>

#include <iostream>

namespace ssvpinterface
{

#ifdef WIN32
    double nearbyint(double x)
    {
        return floor(x+0.5);
    }
#endif

bool squarefunction(int frequency, int screenFrequency, std::vector< std::pair<int, int> > & seq)
{
    // Cannot display this frequencies on a monitor
    if(frequency > screenFrequency/2)
    {
        return false;
    }
    if(frequency == 0)
    {
        seq.push_back(std::pair<int, int>(0,screenFrequency));
        return true;
    }
    // Frames per period
    double frameRate = (double)screenFrequency/(double)frequency;
    int lowBoundFrameRate = (int)floor(frameRate);
    int highBoundFrameRate = (int)ceil(frameRate);
    double highFrameRateProbability = frameRate - (double)lowBoundFrameRate;

    int nbHighBoundFrame = (int)nearbyint((double)frequency*highFrameRateProbability);
    int nbLowBoundFrame  = frequency - nbHighBoundFrame;

    // Generates each frame vector
    std::vector< std::pair<int,int> > lowBoundFrameVector;
    std::vector< std::pair<int,int> > highBoundFrameVector;
    if(lowBoundFrameRate % 2 == 0)
    {
        int demiFrame = lowBoundFrameRate/2;
        for(unsigned int i = 0; i < nbLowBoundFrame; ++i)
        {
            lowBoundFrameVector.push_back(std::pair<int,int>(demiFrame,demiFrame));
        }
        for(unsigned int i = 0; i < nbHighBoundFrame/2; ++i)
        {
            highBoundFrameVector.push_back(std::pair<int,int>(demiFrame,demiFrame+1));
        }
        for(unsigned int i = nbHighBoundFrame/2; i < nbHighBoundFrame; ++i)
        {
            highBoundFrameVector.push_back(std::pair<int,int>(demiFrame+1,demiFrame));
        }
    }
    else
    {
        int demiFrame = (lowBoundFrameRate+1)/2;
        for(unsigned int i = 0; i < nbLowBoundFrame/2; ++i)
        {
            lowBoundFrameVector.push_back(std::pair<int,int>(demiFrame-1,demiFrame));
        }
        for(unsigned int i = nbLowBoundFrame/2; i < nbLowBoundFrame; ++i)
        {
            lowBoundFrameVector.push_back(std::pair<int,int>(demiFrame,demiFrame-1));
        }
        for(unsigned int i = 0; i < nbHighBoundFrame; ++i)
        {
            highBoundFrameVector.push_back(std::pair<int,int>(demiFrame,demiFrame));
        }
    }

    // Randomly fuse the two frame vectors into seq
    seq.resize(0);
    time_t t;
    time(&t);
    #ifndef WIN32
    srandom(t);
    #endif
 
    for(int i = 0; i < frequency; ++i)
    {
        if(highBoundFrameVector.size() != 0)
        {
            #ifndef WIN32
            double tirage = (double)random()/RAND_MAX;
            #else
            double tirage = (double)rand()/RAND_MAX;
            #endif
            if(lowBoundFrameVector.size() == 0 || tirage < highFrameRateProbability)
            {
                std::pair<int, int> tmp = highBoundFrameVector.back();
                highBoundFrameVector.pop_back();
                seq.push_back(tmp);
            }
            else
            {
                std::pair<int, int> tmp = lowBoundFrameVector.back();
                lowBoundFrameVector.pop_back();
                seq.push_back(tmp);
            }
        }
        else if(lowBoundFrameVector.size() != 0)
        {
            std::pair<int, int> tmp = lowBoundFrameVector.back();
            lowBoundFrameVector.pop_back();
            seq.push_back(tmp);
        }
    }    

    return true;
}

} // namespace ssvpinterface

