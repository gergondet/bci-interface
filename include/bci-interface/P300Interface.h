#ifndef _H_P300INTERFACE_H_
#define _H_P300INTERFACE_H_

#ifdef WIN32
#define P300INTERFACE_API __declspec(dllexport)
#else
#define P300INTERFACE_API 
#endif

#include <boost/shared_ptr.hpp>
#include <string>

namespace bciinterface
{

struct P300InterfaceImpl;

struct P300Object
{
public:
    P300Object(const std::string & name, unsigned int x, unsigned int y, 
                unsigned int size_x, unsigned int size_y, 
                unsigned char r, unsigned g, unsigned char b)
    :   name(name), x(x), y(y), size_x(size_x), size_y(size_y), r(r), g(g), b(b)
    {}
public:
    std::string name;
    unsigned int x, y;
    unsigned int size_x, size_y;
    unsigned char r,g,b;
    bool operator==(const std::string & foo) { return name == foo; }
};

class P300Interface
{

public:
    P300INTERFACE_API P300Interface(unsigned int width, unsigned int height);

    /* Objects related functions */

    P300INTERFACE_API void AddObject(P300Object * object);

    P300INTERFACE_API void RemoveObject(const std::string & name);

    P300INTERFACE_API void ClearObjects();

    /* Interface related functions */

    P300INTERFACE_API void DisplayLoop(bool fullscreen = true);

    P300INTERFACE_API void Pause();

    P300INTERFACE_API void Resume();

    P300INTERFACE_API void Close();

private:
    boost::shared_ptr<P300InterfaceImpl> m_impl;

}; 

} // namespace p300interface

#endif
