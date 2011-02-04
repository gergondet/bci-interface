#include <bci-interface/P300Object.h>

namespace bciinterface
{

P300Object::P300Object(const std::string & name, unsigned int x, unsigned int y, 
                        unsigned int size_x, unsigned int size_y,
                        unsigned char r, unsigned char g, unsigned char b)
: name(name), x(x), y(y), size_x(size_x), size_y(size_y), r(r), g(g), b(b)
{
    shapeActive = new sf::Shape();
    shapeInactive = new sf::Shape();
    sf::Color colorActive(r, g, b, 255);
    sf::Color colorInactive(r, g, b, 60);

    shapeActive->AddPoint(x, y, colorActive);
    shapeActive->AddPoint(x+size_x, y, colorActive);
    shapeActive->AddPoint(x+size_x, y+size_y, colorActive);
    shapeActive->AddPoint(x, y+size_y, colorActive);

    shapeInactive->AddPoint(x, y, colorInactive);
    shapeInactive->AddPoint(x+size_x, y, colorInactive);
    shapeInactive->AddPoint(x+size_x, y+size_y, colorInactive);
    shapeInactive->AddPoint(x, y+size_y, colorInactive);
}

P300Object::~P300Object()
{
    delete shapeActive;
    delete shapeInactive;
}

bool P300Object::operator==(const P300Object & object)
{
    return this->name == object.name;
}

bool P300Object::operator==(const P300Object * object)
{
    return this->name == object->name;
}

inline void P300Object::DrawActive(sf::RenderWindow * app)
{
    app->Draw(*shapeActive);
}

inline void P300Object::DrawInactive(sf::RenderWindow * app)
{
    app->Draw(*shapeInactive);
}

} // namespace bciinterface

