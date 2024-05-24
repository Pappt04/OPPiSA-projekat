#include "IR.h"

#include "Types.h"
#include <iostream>

std::string Variable::getName() const
{
    return std::string();
}

int Variable::getValue() const
{
    return 0;
}

Variable::VariableType Variable::getType() const
{
    return VariableType();
}

int Variable::getPosition() const
{
    return 0;
}

Regs Variable::getAssignment() const
{
    return Regs();
}
