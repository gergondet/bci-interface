#include "SSVEPTrainingManager.h"

#include <iostream>

using namespace bciinterface;

SSVEPTrainingManager::SSVEPTrainingManager(unsigned int width, unsigned int height, short port_command, short port_training, coshellbci::CoshellClient * coshell)
: StaticSteering(coshell),
  m_train_with_moving_robot(false),
  m_width(width), m_height(height),
  m_command_receiver(new UDPReceiver(port_command)),
  m_training_receiver(new UDPReceiver(port_training)),
  m_training_th(0), m_training_command(0),
  m_shape()
{
    m_shape.SetPosition(-100,-100);
}

SSVEPTrainingManager::~SSVEPTrainingManager()
{
    delete m_command_receiver;
    delete m_training_receiver;
    if(m_training_th)
    {
        m_training_th->join();
        delete m_training_th;
    }
}

void SSVEPTrainingManager::CommandLoop()
{
    m_training_th = new boost::thread(boost::bind(&bciinterface::UDPReceiver::CommandLoop, m_training_receiver));
    m_command_receiver->CommandLoop();
}

void SSVEPTrainingManager::Close()
{
    m_training_receiver->Close();
    m_command_receiver->Close();
}

int SSVEPTrainingManager::GetCommand()
{
    m_training_command = m_training_receiver->GetCommand();
    if(m_train_with_moving_robot)
    {
        return m_training_command;
    }
    return m_command_receiver->GetCommand();
}

bool SSVEPTrainingManager::InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
{
    switch(m_training_command)
    {
        case 0:
            m_shape.SetPosition(-100,-100);
            break;
        case 1:
            m_shape.SetPosition(m_width/2,260);
            break;
        case 2:
            m_shape.SetPosition(m_width-260,m_height/2);
            break;
        case 3:
            m_shape.SetPosition(m_width/2,m_height - 260);
            break;
        case 4:
            m_shape.SetPosition(260,m_height/2);
            break;
        default:
            break;
    }
    return StaticSteering::InterpretCommand(command, objects);
}

