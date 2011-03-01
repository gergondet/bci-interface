#ifndef _H_RANDOMPOINT_H_
#define _H_RANDOMPOINT_H_

#include <boost/shared_ptr.hpp>
#include <SFML/Graphics.hpp>

#ifdef WIN32
#define SSVEPINTERFACE_API __declspec(dllexport)
#else
#define SSVEPINTERFACE_API
#endif

namespace bciinterface
{

struct RandomPointImpl;

class RandomPoint
{
public:
    //Constructor
    SSVEPINTERFACE_API RandomPoint(float x=0, float y=0, float size=5, float period=300, int r=0, int g=0, int b=0, int a=255);

    void SetPointDisplay(bool const displayPoint);

    bool PointDisplay();

    std::vector<float> GetPosition();

    sf::Shape * GetPoint();

    void SetPosition(std::vector<float> position);

    float GetPeriod();

    void SetPeriod(float period);

    float GetSize();

private:
    boost::shared_ptr<RandomPointImpl> m_rdmptsimpl;

};

}

#endif
