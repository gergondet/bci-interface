#include <bci-interface/RandomPoint.h>

#include <SFML/Graphics.hpp>
#include <iostream>

namespace bciinterface
{

struct RandomPointImpl
{
private:
    sf::Shape point;
    bool pointDisplay;
    std::vector<float> position;
    float period;
    float size;

public:


    //(position x, position y, size, period nombre de passages de boucle, color r, color g, color b, color a);
    RandomPointImpl(float x, float y, float size, float period, int r, int g, int b, int a) :
        point(sf::Shape::Rectangle(x, y, x+size, y+size, sf::Color(r, g, b, a))) ,
        pointDisplay(true),
    	period(period),
    	size(size)

    {
    	position.push_back(x);
		position.push_back(y);
    	point.EnableFill(true);
    }

    ~RandomPointImpl()
    {
    }

   void SetPointDisplay(bool const displayPoint)
    {
        this->pointDisplay = displayPoint;
    }

    bool PointDisplay()
    {
        return pointDisplay;
    }

    std::vector<float> GetPosition()
    {
        return position;
    }

    sf::Shape * GetPoint()
    {
        return &point;
    }

    void SetPosition(std::vector<float> position)
     {
         this->point.SetPosition(position[0],position[1]);
     }

    float GetPeriod()
    {
        return period;
    }

    float GetSize()
    {
        return size;
    }

    void SetPeriod(float period)
     {
         this->period = period;
     }

}; //class RandomPointImpl


RandomPoint::RandomPoint(float x, float y, float size, float period, int r, int g, int b, int a) :
    m_rdmptsimpl(new RandomPointImpl(x, y, size, period, r, g, b, a))
{
}

void RandomPoint::SetPointDisplay(bool const displayPoint)
 {
	m_rdmptsimpl->SetPointDisplay(displayPoint);
 }

bool RandomPoint::PointDisplay()
{
    return m_rdmptsimpl->PointDisplay();
}

std::vector<float> RandomPoint::GetPosition()
{
    return m_rdmptsimpl->GetPosition();
}

sf::Shape * RandomPoint::GetPoint()
{
    return m_rdmptsimpl->GetPoint();
}

void RandomPoint::SetPosition(std::vector<float> position)
 {
	m_rdmptsimpl->SetPosition(position);
 }

float RandomPoint::GetPeriod()
{
    return m_rdmptsimpl->GetPeriod();
}

void RandomPoint::SetPeriod(float period)
 {
	m_rdmptsimpl->SetPeriod(period);
 }

float RandomPoint::GetSize()
{
	m_rdmptsimpl-> GetSize();
}


} // namespace bciinterface

