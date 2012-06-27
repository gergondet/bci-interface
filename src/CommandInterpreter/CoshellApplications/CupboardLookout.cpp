#include <bci-interface/CommandInterpreter/CoshellApplications/CupboardLookout.h>

#include <bci-interface/CommandInterpreter/SimpleInterpreter.h>
#include <bci-interface/DisplayObject.h>

#include <coshell-bci/CoshellClient.h>
#include <visionsystem/vs_plugins/xmlrpc/xmlrpc-client.h>

using namespace visionsystem;

namespace bciinterface
{

struct CupboardLookoutImpl : public SimpleInterpreter
{
private:
    coshellbci::CoshellClient * m_coshell;
    XMLRPCClient * m_client;
    float m_head_lr;
    unsigned int m_width;
    unsigned int m_height;
public:
    CupboardLookoutImpl(unsigned int width, unsigned int height, coshellbci::CoshellClient * coshell, XMLRPCClient * client)
        : m_coshell(coshell), m_client(client), m_head_lr(0), m_width(width), m_height(height)
    {}

    bool InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
    {
        SimpleInterpreter::InterpretCommand(command, objects);
        /* Update cupboard stim position according to recognition plugin */
        XmlRpcValue params = "coca";
        XmlRpcValue result;
        m_client->execute("GetObjectPosition", params, result);
        vision::ImageRef cupboard_pos;
        cupboard_pos.x = (int)result["left"][0];
        cupboard_pos.y = (int)result["left"][1];
        if(cupboard_pos.x != 0 && cupboard_pos.y != 0)
        {
            objects[2]->SetPosition(cupboard_pos.x*m_width/640, cupboard_pos.y*m_height/480);
        }
        else
        {
            objects[2]->Unhighlight();
        }
        /* 1: left, 2: right, 3: cupboard stim */
        if(command == 0) { return false; }
        std::stringstream cmd;
        if(command == 1)
        {
            if(m_head_lr > 0.5)
            {
                cmd << "pg.parsecmd :stepseq 0 0 0 0 -0.19 10 0 0.19 10 0 -0.19 0";
            }
            else
            {
                m_head_lr += 0.2;
                cmd << "set featureHeadDes.errorIN [2](" << m_head_lr << ",0.45)"; 
            }
        }
        if(command == 2)
        {
            if(m_head_lr < -0.5)
            {
                cmd << "pg.parsecmd :stepseq 0 0 0 0 -0.19 -10 0 0.19 -10 0 -0.19 0";
            }
            else
            {
                m_head_lr -= 0.2;
                cmd << "set featureHeadDes.errorIN [2](" << m_head_lr << ",0.45)"; 
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

CupboardLookout::CupboardLookout(unsigned int width, unsigned int height, coshellbci::CoshellClient * coshell, XMLRPCClient * client)
: m_impl(new CupboardLookoutImpl(width, height, coshell, client))
{}

bool CupboardLookout::InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
{
    return m_impl->InterpretCommand(command, objects);
}

} // namespace bciinterface

