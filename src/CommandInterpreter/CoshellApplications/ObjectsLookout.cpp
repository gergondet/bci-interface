#include <bci-interface/CommandInterpreter/CoshellApplications/ObjectsLookout.h>

#include <bci-interface/CommandInterpreter/SimpleInterpreter.h>
#include <bci-interface/DisplayObject.h>

#include <coshell-bci/CoshellClient.h>
#include <visionsystem/vs_plugins/recognition/recognition.h>

namespace bciinterface
{

struct ObjectsLookoutImpl : public SimpleInterpreter
{
private:
    coshellbci::CoshellClient * m_coshell;
    Recognition * m_recognition;
    std::vector<std::string> m_objects;
    float m_head_lr;
    unsigned int m_width;
    unsigned int m_height;
public:
    ObjectsLookoutImpl(unsigned int width, unsigned int height, coshellbci::CoshellClient * coshell, Recognition * recognition, const std::vector<std::string> & objects)
        : m_coshell(coshell), m_recognition(recognition), m_objects(objects), m_head_lr(0), m_width(width), m_height(height)
    {}

    bool InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
    {
        SimpleInterpreter::InterpretCommand(command, objects);
        /* Update objects stim position according to recognition plugin */
        for(size_t i = 0; i < m_objects.size(); ++i)
        {
            vision::ImageRef object_pos = m_recognition->GetObjectPosition(m_objects[i]);
            if(object_pos.x != 0 && object_pos.y != 0)
            {
                objects[2+i]->SetPosition(object_pos.x*m_width/640, object_pos.y*m_height/480);
            }
            else
            {
                objects[2+i]->Unhighlight();
            }
        }
        if(command == 0) { return false; }
        /* 1: left, 2: right, 3-x: objects stims */
        std::stringstream cmd;
        if(command == 1)
        {
            if(m_head_lr < 0.6)
            {
                m_head_lr += 0.2;
                cmd << "set featureHeadDes.errorIN [2](" << m_head_lr << ",0.3)"; 
            }
        }
        if(command == 2)
        {
            if(m_head_lr > -0.6)
            {
                m_head_lr -= 0.2;
                cmd << "set featureHeadDes.errorIN [2](" << m_head_lr << ",0.3)"; 
            }
        }
        if(command > 2) { return true; }
        m_coshell->ExecuteACommand(cmd.str());
        return false;
    }
};

ObjectsLookout::ObjectsLookout(unsigned int width, unsigned int height, coshellbci::CoshellClient * coshell, Recognition * recognition_plugin, const std::vector<std::string> & objects)
: m_impl(new ObjectsLookoutImpl(width, height, coshell, recognition_plugin, objects))
{}

bool ObjectsLookout::InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
{
    return m_impl->InterpretCommand(command, objects);
}

} // namespace bciinterface

