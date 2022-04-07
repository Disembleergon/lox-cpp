#include "../include/types/Callables.h"
#include <chrono>

lox::literal_t lox::ClockFunction::call(const Interpreter &, const std::vector<literal_t> &) const
{
    using namespace std::chrono;

    const auto timepoint = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    return timepoint / 1000.0;
}
