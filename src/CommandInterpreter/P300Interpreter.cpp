#include <bci-interface/CommandInterpreter/P300Interpreter.h>

#include <bci-interface/DisplayObject.h>

namespace bciinterface
{

struct P300InterpreterImpl
{
private:
    bool m_training;
    unsigned int m_nb_objects;
    unsigned int m_trials;
    unsigned int m_flashed_objects;
    bool m_next_is_cmd;
public:
    P300InterpreterImpl(bool training, unsigned int nb_objects, unsigned int trials)
    :   m_training(training), m_nb_objects(nb_objects), m_trials(trials),
        m_flashed_objects(0), m_next_is_cmd(false)
    {}

    bool InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
    {
        if(command <= 0) { return false; }
        int in_cmd = command - 1;
        if(m_next_is_cmd)
        {
            if(in_cmd < objects.size())
            {
                objects[in_cmd]->Highlight();
            }
            m_next_is_cmd = false;
            m_flashed_objects = 0;
            return true;
        }
        if(in_cmd < objects.size())
        {
            objects[in_cmd]->Highlight();
        }
        m_flashed_objects++;
        m_next_is_cmd = (m_flashed_objects == m_trials*m_nb_objects);
        return false;
    }
};

P300Interpreter::P300Interpreter(bool training, unsigned int nb_objects, unsigned int trials)
: m_impl(new P300InterpreterImpl(training, nb_objects, trials))
{}

bool P300Interpreter::InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
{
    return m_impl->InterpretCommand(command, objects);
}

} // namespace bciinterface

