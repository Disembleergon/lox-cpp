#pragma once

class ErrorHandler {
public:
	static void error(int line, const char* message);
	static void report(int line, const char* where, const char* message);
};