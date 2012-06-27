#include <bci-interface/CommandInterpreter/SimpleInterpreter.h>

class WhackInterpreter : public bciinterface::SimpleInterpreter
{
public:
    WhackInterpreter() : prev_cmd(0), hold_cmd_since(0) {}

    ~WhackInterpreter() {}

    void SetCurrentTarget(int tgt) { current_target = tgt; }

    bool InterpretCommand(int command, const std::vector<bciinterface::DisplayObject*> & objects);
private:
    int current_target;
    int prev_cmd;
    unsigned int hold_cmd_since;
};
