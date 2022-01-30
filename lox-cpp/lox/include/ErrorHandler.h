#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

class ErrorHandler
{
  public:
    static void error(int line, const std::string &&message);
    static void report(int line, const std::string &where, const std::string &message);
};

#endif
