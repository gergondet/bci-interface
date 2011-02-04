#ifndef _H_P300OBJECT_H_
#define _H_P300OBJECT_H_

#ifdef WIN32
#define P300OBJECT_API __declspec(dllexport)
#else
#define P300_API 
#endif

#include <SFML/Graphics.hpp>

namespace bciinterface
{

struct P300Object
{
public:
    P300Object(const std::string & name, unsigned int x, unsigned int y, 
                unsigned int size_x, unsigned int size_y, 
                unsigned char r, unsigned char g, unsigned char b);

    ~P300Object();

    bool operator==(const P300Object & object);

    bool operator==(const P300Object * object);

    const std::string & GetName() { return name; }

    void DrawActive(sf::RenderWindow * app);

    void DrawInactive(sf::RenderWindow * app);

private:
    std::string name;
    unsigned int x, y;
    unsigned int size_x, size_y;
    unsigned char r,g,b;
    sf::Shape * shapeActive;
    sf::Shape * shapeInactive;
};

} // namespace p300interface

#endif
