#ifndef _H_BCIINTERFACE_IMPL_H_
#define _H_BCIINTERFACE_IMPL_H_

#include "bci-interface/EventHandler.h"

#include "bci-interface/Background.h"
#include "bci-interface/DisplayObject.h"

#include "bci-interface/CommandReceiver.h"
#include "bci-interface/CommandOverrider.h"
#include "bci-interface/CommandInterpreter.h"

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "bci-interface/Utils/Win32.h"

namespace bciinterface
{

struct BCIInterfaceImpl
{
private:
    const BCIInterface & m_ref;
    unsigned int m_width;
    unsigned int m_height;
    bool m_in_paradigm;
    bool m_close;
    bool m_finished;
    sf::RenderWindow * m_app;
    std::ofstream m_fpslog;

    bool m_take_screenshot;
    unsigned int m_screenshot_index;

    std::vector<EventHandler *> m_handlers;

    Background * m_background;
    boost::thread * m_backgroundth;

    std::vector<DisplayObject *> m_active_objects;

    std::vector<DisplayObject *> m_objects;
    std::vector<DisplayObject *> m_objects_non_owned;

    std::vector<DisplayObject *> m_gl_objects;
    std::vector<DisplayObject *> m_gl_objects_non_owned;
    std::vector<DisplayObject *> m_gl_objects_to_delete;

    CommandReceiver * m_receiver;
    boost::thread * m_receiverth;

    CommandOverrider * m_overrider;

    CommandInterpreter * m_interpreter;

public:
    BCIInterfaceImpl(const BCIInterface & ref, unsigned int width, unsigned int height);

    ~BCIInterfaceImpl();

    void InitGL();

    void Resize();

    void AddEventHandler(EventHandler * handler);

    void SetBackground(Background * background);

    Background * GetBackground() const;

    void AddObject(DisplayObject * object);

    void AddNonOwnedObject(DisplayObject * object);

    void SetCommandReceiver(CommandReceiver * receiver);

    void SetCommandOverrider(CommandOverrider * overrider);

    void SetCommandInterpreter(CommandInterpreter * interpreter);

    CommandInterpreter * GetCommandInterpreter();

    void Clean();

    void DisplayLoop(bool fullscreen);

    sf::RenderWindow * DisplayLoop(sf::RenderWindow * app, bool fullscreen, int & cmd, float timeout = 0);

    /* Internal loop launched by BCIInterface public functions */
    void DisplayLoop(sf::Window & eventWindow, sf::RenderTarget & drawTarget, int & cmd, float timeout = 0);

    void Close();

    bool ParadigmStatus();

    void StopParadigm();

    void StartParadigm();

    sf::RenderWindow * GetRenderWindow();
};

} // namespace bciinterface

#endif
