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
    unsigned int m_training_words[5];
    unsigned int m_flashtime;
    unsigned int m_reset_flasht;

    unsigned int m_flashed_objects;
    unsigned int m_flash_before_cmd;
    unsigned int m_cmd;
public:
    P300InterpreterImpl(bool training, unsigned int nb_objects)
    :   m_training(training), m_nb_objects(nb_objects),
        m_reset_flasht(0), m_flashed_objects(0), m_cmd(0)
    {
        m_trials = m_training ? 15:5;
        m_training_words = {1, 9, 12, 7, 14};
        m_flashtime = 60000; /* 60 ms in us */
        m_flash_before_cmd = m_trials*m_nb_objects;
    }

    bool InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
    {

    }
};

P300Interpreter::P300Interpreter(bool training, unsigned int nb_objects)
: m_impl(new P300InterpreterImpl(training, nb_objects))
{}

bool P300Interpreter::InterpretCommand(int command, const std::vector<DisplayObject *> & objects)
{
    return m_impl->InterpretCommand(command, objects);
}

} // namespace bciinterface

