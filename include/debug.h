#pragma once
#include "includes.h"
#define BLUE "\033[1;36m"
#define RED "\033[1;31m"
#define PRINT(stream, color, var) stream << "\033[1;37m" << __FILE__ << ":" << __LINE__ << ": " << color << var << "\033[0m " << std::endl
#define PRINT2(stream, color, label, var) stream << "\033[1;37m" << __FILE__ << ":" << __LINE__ << ": " << color << label << ":\033[0m " << var << std::endl

#if defined(VERBOSE_ENABLED) || defined(INFO_ENABLED)
#define DEBUGINFO DEBUG
#else
#define DEBUGINFO
#endif

#ifdef VALIDATION_ENABLED

#define DEBUG(var) PRINT(std::cerr, BLUE, var)
#define DEBUG2(label, var) PRINT2(std::cerr, BLUE, label, var)
#define DEBUG3(var) DEBUG2(#var, var)

#define ERROR(var)                          \
    {                                       \
        std::stringstream ss;               \
        PRINT(ss, RED, var);                \
        throw std::runtime_error(ss.str()); \
    }
#define ERROR2(label, var)                  \
    {                                       \
        std::stringstream ss;               \
        PRINT2(ss, RED, label, var);        \
        throw std::runtime_error(ss.str()); \
    }
#define CERROR(var) PRINT(std::cerr, RED, var)
#define CERROR2(label, var) PRINT2(std::cerr, RED, label, var)
#define CERROR3(var) CERROR2(#var, var)

#else

#define DEBUG(var)
#define DEBUG2(label, var)
#define DEBUG3(var)

#define ERROR(var)
#define ERROR2(label, var)
#define CERROR(var)
#define CERROR2(label, var)
#define CERROR3(var)

#endif