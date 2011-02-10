#ifndef _H_P300INTERFACE_H_
#define _H_P300INTERFACE_H_

#ifdef WIN32
#define P300INTERFACE_API __declspec(dllexport)
#else
#define P300INTERFACE_API 
#endif

#include <boost/shared_ptr.hpp>
#include <string>

#include <bci-interface/P300Object.h>
#include <bci-interface/BackgroundSprite.h>

namespace bciinterface
{

struct P300InterfaceImpl;

class P300Interface
{

public:
    /* Constructor, mode 1 for training, 2 free spell */
    P300INTERFACE_API P300Interface(unsigned int width, unsigned int height, unsigned int mode);

    /* P300 timing related functions */
    P300INTERFACE_API void SetNbTrials(const unsigned int nbTrials);
    
    P300INTERFACE_API void SetFlashTime(const float flashTime);
    
    P300INTERFACE_API void SetInterFlashTime(const float interFlashTime);
    
    P300INTERFACE_API void SetInterCycleTime(const float interCycleTime);

    /* Objects related functions */

    P300INTERFACE_API void AddObject(P300Object * object);

    P300INTERFACE_API void RemoveObject(const std::string & name);

    P300INTERFACE_API void ClearObjects();

    /* Interface with the BCI system related */

    P300INTERFACE_API void StartP300Client(const std::string & serverName, unsigned short serverPort);

    /* After each decision the interface is paused and should be relaunched like this */
    P300INTERFACE_API void ResumeP300Client();

    /* Interface related functions */

    P300INTERFACE_API void SetBackgroundSprite(BackgroundSprite * sprite);

    /* If true you will have to update the background by hand */
    P300INTERFACE_API void SetUpdateBackgroundManually(bool enable);

    /* Expect a w*h grayscale buffer */
    P300INTERFACE_API void UpdateBackground(unsigned char * img);

    P300INTERFACE_API void DisplayLoop(sf::RenderWindow * app, unsigned int * cmdOut = 0);

    P300INTERFACE_API void DisplayLoop(bool fullscreen = true);

    P300INTERFACE_API void Pause();

    P300INTERFACE_API void Resume();

    P300INTERFACE_API void Close();

private:
    boost::shared_ptr<P300InterfaceImpl> m_impl;

}; 

} // namespace p300interface

extern "C"
{
    void has_P300Interface();
}

#endif
