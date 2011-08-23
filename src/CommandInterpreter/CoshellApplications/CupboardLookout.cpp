#include <bci-interface/CommandInterpreter/CoshellApplications/CupboardLookout.h>

#include <bci-interface/CommandInterpreter/SimpleInterpreter.h>
#include <bci-interface/DisplayObject.h>

#include <coshell-bci/CoshellClient.h>
#include <visionsystem/vs_plugins/recognition/recognition.h>

namespace bciinterface
{

struct CupboardLookoutImpl : public SimpleInterpreter
{
private:
    coshellbci::CoshellClient * m_coshell;
    Recognition * m_recognition;
    float m_head_lr;
public:
    CupboardLookoutImpl(coshellbci::CoshellClient * coshell, Recognition * recognition)
        : m_coshell(coshell), m_recognition(recognition), m_head_lr(0)
    {}

    bool InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
    {
        SimpleInterpreter::InterpretCommand(command, objects);
        /* Update cupboard stim position according to recognition plugin */
        vision::ImageRef cupboard_pos = m_recognition->GetObjectPosition("cupboard");
        if(cupboard_pos.x != 0 && cupboard_pos.y != 0)
        {
            objects[2]->SetPosition(cupboard_pos.x, cupboard_pos.y);
        }
        else
        {
            objects[2]->Unhighlight();
        }
        /* 1: left, 2: right, 3: cupboard stim */
        std::stringstream cmd;
        if(command == 1)
        {
            if(m_head_lr == 0.6)
            {
                cmd << "pg.parsecmd :stepseq 0 0 0 0 -0.19 10 0 0.19 10 0 -0.19 0";
            }
            else
            {
                m_head_lr += 0.2;
                cmd << "set featureHeadDes.errorIN [2](" << m_head_lr << ",0)"; 
            }
        }
        if(command == 2)
        {
            if(m_head_lr == -0.6)
            {
                cmd << "pg.parsecmd :stepseq 0 0 0 0 -0.19 -10 0 0.19 -10 0 -0.19 0";
            }
            else
            {
                m_head_lr -= 0.2;
                cmd << "set featureHeadDes.errorIN [2](" << m_head_lr << ",0)"; 
            }
        }
        if(command == 3)
        {
            return true;
        }
        m_coshell->ExecuteACommand(cmd.str());
        return false;
    }
};

CupboardLookout::CupboardLookout(coshellbci::CoshellClient * coshell, Recognition * recognition_plugin)
: m_impl(new CupboardLookoutImpl(coshell, recognition_plugin))
{}

bool CupboardLookout::InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
{
    return m_impl->InterpretCommand(command, objects);
}

} // namespace bciinterface

