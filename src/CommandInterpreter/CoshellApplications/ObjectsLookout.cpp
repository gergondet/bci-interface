#include <bci-interface/CommandInterpreter/CoshellApplications/ObjectsLookout.h>

#include <bci-interface/CommandInterpreter/SimpleInterpreter.h>
#include <bci-interface/DisplayObject.h>

#include <coshell-client/CoshellClient.h>
#include <visionsystem/vs_plugins/xmlrpc/xmlrpc-client.h>

using namespace visionsystem;

namespace bciinterface
{

struct ObjectsLookoutImpl : public SimpleInterpreter
{
private:
    coshell::CoshellClient * m_coshell;
    XMLRPCClient * m_client;
    std::vector<std::string> m_objects;
    float m_head_lr;
    unsigned int m_width;
    unsigned int m_height;
public:
    ObjectsLookoutImpl(unsigned int width, unsigned int height, coshell::CoshellClient * coshell, XMLRPCClient * client, const std::vector<std::string> & objects)
        : m_coshell(coshell), m_client(client), m_objects(objects), m_head_lr(0), m_width(width), m_height(height)
    {}

    bool InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
    {
        SimpleInterpreter::InterpretCommand(command, objects);
        /* Update objects stim position according to recognition plugin */
        for(size_t i = 0; i < m_objects.size(); ++i)
        {
            XmlRpcValue params = m_objects[i];
            XmlRpcValue result;
            m_client->execute("GetObjectPosition", params, result);
            vision::ImageRef object_pos;
            object_pos.x = (int)result["left"][0];
            object_pos.y = (int)result["left"][1];
            if(object_pos.x != 0 && object_pos.y != 0)
            {
                objects[i]->SetPosition(object_pos.x*m_width/640, object_pos.y*m_height/480);
            }
            else
            {
                objects[i]->Unhighlight();
            }
        }
        if(command == 0) { return false; }
        /* 1: left, 2: right, 3-x: objects stims */
        std::stringstream cmd;
//        if(command == 1)
//        {
//            if(m_head_lr < 0.6)
//            {
//                m_head_lr += 0.2;
//                cmd << "set featureHeadDes.errorIN [2](" << m_head_lr << ",0.45)"; 
//            }
//        }
//        if(command == 2)
//        {
//            if(m_head_lr > -0.6)
//            {
//                m_head_lr -= 0.2;
//                cmd << "set featureHeadDes.errorIN [2](" << m_head_lr << ",0.45)"; 
//            }
//        }
        if(command > 0) 
        { 
            m_objects.resize(0);
            return true; 
        }
        m_coshell->ExecuteACommand(cmd.str());
        return false;
    }
};

ObjectsLookout::ObjectsLookout(unsigned int width, unsigned int height, coshell::CoshellClient * coshell, XMLRPCClient * client_plugin, const std::vector<std::string> & objects)
: m_impl(new ObjectsLookoutImpl(width, height, coshell, client_plugin, objects))
{}

bool ObjectsLookout::InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
{
    return m_impl->InterpretCommand(command, objects);
}

} // namespace bciinterface

