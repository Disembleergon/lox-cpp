#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

class ErrorHandler
{
  public:
    static void error(int line, const char *message);
    static void report(int line, const char *where, const char *message);
};

#endif
