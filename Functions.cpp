#include "Functions.hpp"

std::string doubleToString(double Val)
{
    std::string result;
    std::ostringstream ost;
    ost << Val;
    result = ost.str();
    return result;
}
