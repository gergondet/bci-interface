#ifndef _H_SSVEPTRAININGMANAGER_H_
#define _H_SSVEPTRAININGMANAGER_H_

#include <bci-interface/CommandReceiver/UDPReceiver.h>
#include <bci-interface/CommandInterpreter/CoshellApplications/StaticSteering.h>
#include <bci-interface/DisplayObject/SpriteObject.h>

#include <boost/thread.hpp>

#include <SFML/Graphics.hpp>

using namespace bciinterface;

class SSVEPTrainingManager : public CommandReceiver, public StaticSteering
{
public:
    SSVEPTrainingManager(unsigned int width, unsigned int height, short port_command, short port_training, coshell::CoshellClient * coshell);

    virtual ~SSVEPTrainingManager();

    bciinterface::SpriteObject & GetShape() { return m_shape; }

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

    bciinterface::SpriteObject m_shape;
};


#endif
