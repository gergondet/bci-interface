#ifndef _H_SSVEPTRAININGMANAGER_H_
#define _H_SSVEPTRAININGMANAGER_H_

#include <bci-interface/CommandReceiver/UDPReceiver.h>
#include <bci-interface/CommandInterpreter/CoshellApplications/StaticSteering.h>
#include <bci-interface/DisplayObject.h>

#include <boost/thread.hpp>

#include <SFML/Graphics.hpp>

using namespace bciinterface;

class SimpleShape : public DisplayObject
{
public:
    SimpleShape() : m_shape(sf::Shape::Rectangle(-30, -30, 60, 60, sf::Color(0,255,0,255))) 
    {
        m_shape.SetPosition(0,0);
    }

    virtual void Display(sf::RenderWindow * app, unsigned int frameCount, sf::Clock & clock)
    {
        app->Draw(m_shape);
    }

    virtual void SetPosition(float X, float Y)
    {
        m_shape.SetPosition(X,Y);
    }

private:
    sf::Shape m_shape;
};

class SSVEPTrainingManager : public CommandReceiver, public StaticSteering
{
public:
    SSVEPTrainingManager(unsigned int width, unsigned int height, short port_command, short port_training, coshellbci::CoshellClient * coshell);

    virtual ~SSVEPTrainingManager();

    SimpleShape & GetShape() { return m_shape; }

    void SetTrainWithMovingRobot(bool in) { m_train_with_moving_robot = in; }

    /* CommandReceiver part */
    virtual void CommandLoop();

    virtual void Close();

    virtual int GetCommand();

    /* CommandInterpreter part */
    virtual bool InterpretCommand(int command, const std::vector<DisplayObject *> & objects);
private:
    bool m_train_with_moving_robot;

    unsigned int m_width;
    unsigned int m_height;

    UDPReceiver * m_command_receiver;
    UDPReceiver * m_training_receiver;

    boost::thread * m_training_th;
    int m_training_command;

    SimpleShape m_shape;
};


#endif
