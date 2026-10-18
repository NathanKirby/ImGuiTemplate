#pragma once
#include "settings.hpp"
#ifdef WINDOW_CONSOLE
#include <iostream>
#define CLOG(msg) std::cout << msg << std::endl
#define ELOG(msg) std::cerr << msg << std::endl
#else
#define CLOG(msg)
#define ELOG(msg)
#endif // WINDOW_CONSOLE
