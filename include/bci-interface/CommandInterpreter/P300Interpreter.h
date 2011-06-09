#ifndef _H_P300INTERPRETER_H_
#define _H_P300INTERPRETER_H_

#include <bci-interface/CommandInterpreter.h>

#include <boost/shared_ptr.hpp>

namespace bciinterface
{

struct P300InterpreterImpl;

class P300Interpreter : public CommandInterpreter
{
public:
    P300Interpreter(bool training, unsigned int nb_objects, unsigned int trials = 4);

    virtual bool InterpretCommand(int command, const std::vector<DisplayObject*> & objects);

    void SetTrainingSet(std::vector<unsigned int> & training_set);
private:
    boost::shared_ptr<P300InterpreterImpl> m_impl;
};

}

#endif

