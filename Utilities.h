#ifndef __UTILITIES_H
#define __UTILITIES_H
#include <cstdio>
#include <cstdarg>
#include <assert.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef _MSC_VER 
#define __PRETTY_FUNCTION__ __FUNCSIG__ 
#endif


class Log {
public:
    enum class ELogColour {
        BLACK = 0,
        BLUE,
        GREEN,
        CYAN,
        RED,
        MAGENTA,
        BROWN,
        LIGHTGRAY,
        DARKGRAY,
        LIGHTBLUE,
        LIGHTGREEN,
        LIGHTCYAN,
        LIGHTRED,
        LIGHTMAGENTA,
        YELLOW,
        WHITE,
    };

    static void Print(const char* fmt, ...);
    static void Message(const char* fmt, ...);
    static void Success(const char* fmt, ...);
    static void Failure(const char* fmt, ...);
    static void Warning(const char* fmt, ...);
    static void Error(const char* file, int line, const char* function, const char* fmt, ...);
    static void Fatal(const char* file, int line, const char* function, const bool shouldBreak,  const char* fmt, ...);

private:
    static void _Output(ELogColour colour, FILE* stream, const char* fmt, va_list args);
    static void _Output(ELogColour colour, FILE* stream, const char* fmt, ...);
};

inline void Log::Print(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    _Output(ELogColour::WHITE, stdout, fmt, args);
    va_end(args);
}

inline void Log::Message(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Output(ELogColour::LIGHTCYAN, stdout, "Message: ");
    _Output(ELogColour::LIGHTCYAN, stdout, fmt, args);
    va_end(args);
}

inline void Log::Success(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Output(ELogColour::GREEN, stdout, "Success: ");
    _Output(ELogColour::GREEN, stdout, fmt, args);
    va_end(args);
}

inline void Log::Failure(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Output(ELogColour::BROWN, stdout, "Failure: ");
    _Output(ELogColour::BROWN, stdout, fmt, args);
    va_end(args);
}

inline void Log::Warning(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Output(ELogColour::YELLOW, stdout, "Warning: ");
    _Output(ELogColour::YELLOW, stdout, fmt, args);
    va_end(args);
}

inline void Log::Error(const char* file, int line, const char* function, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Output(ELogColour::LIGHTRED, stderr, "Error\nFILE: %s\n\tLINE: %d\n\tFUNCTION: %s\n", file, line, function);
    _Output(ELogColour::LIGHTRED, stderr, fmt, args);
    va_end(args);
}

inline void Log::Fatal(const char* file, int line, const char* function, const bool shouldBreak, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Output(ELogColour::RED, stderr, "FATAL ERROR\nFILE: %s\n\tLINE: %d\n\tFUNCTION: %s\n", file, line, function);
    _Output(ELogColour::RED, stderr, fmt, args);
    va_end(args);

    if (shouldBreak) {
        assert(false && "A Fatal Error has Occurred!");
    }
}



inline void Log::_Output(ELogColour colour, FILE * stream, const char* fmt, va_list args){
    
//Change the output colour 
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)colour);
#elif defined _LINUX
   // fprintf(stream, "\033["); //TODO: Evaluate different colour codes. 
#endif

    vfprintf(stream, fmt, args);
    //Reset the output colour
#ifdef _WIN32
    SetConsoleTextAttribute(hConsole, (WORD)ELogColour::WHITE);
#elif defined _LINUX
   // fprintf(stream, "\033["); //TODO: Evaluate different colour codes. 
#endif
}

inline void Log::_Output(ELogColour colour, FILE* stream, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _Output(colour, stream, fmt, args);
    va_end(args);
}


#endif
