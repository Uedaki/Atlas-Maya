#pragma once

#include <fstream>

#include <Maya/MIOStream.h>

#include "Exception.h"
#include "Stopwatch.h"

#define LOG_INFO(msg) Logger::get().write2(Logger::Type::INFO, std::ostringstream() << msg)
#define LOG_WARNING(msg) Logger::get().write2(Logger::Type::WARNING, std::ostringstream() << msg)
#define LOG_CRITICAL(msg) Logger::get().write2(Logger::Type::CRITICAL, std::ostringstream() << msg)
#define LOG_EXCEPTION(error) Logger::get().writeException(error)

#ifdef DEV
# define LOG_DEBUG(msg) Logger::get().write2(Logger::Type::DEBUG, std::ostringstream() << msg)
#else
# define LOG_WARNING(msg)
#endif

class Logger
{
public:
    enum class Type : uint8_t
    {
        INFO,
        DEBUG,
        WARNING,
        CRITICAL
    };

    static Logger &get()
    {
        static Logger log;
        return (log);
    }

    ~Logger()
    {
        if (file)
        {
            file << "\n  ===============================================\n"
                << "    End   Output log ( "
                << Stopwatch::getDate()
                << " at "
                << Stopwatch::getTime()
                << " ): "
                << "\n  ===============================================\n\n";
            file.flush();
            file.close();
        }
    }

    void defineOuputFile(const std::string &filename)
    {
        file.open(filename, std::ios::app);
        file.seekp(std::ios::beg);

        if (!file.good()) return;
        file << "  ===============================================\n"
            << "    Begin Output log ( "
            << Stopwatch::getDate()
            << " at "
            << Stopwatch::getTime()
            << " ): "
            << "\n  ===============================================\n\n";
        file.flush();
    }

    void write2(Type type, std::ostringstream msg)
    {
        std::ostringstream os;
        os << "[" << Stopwatch::getTime() << "][";
        switch (type)
        {
        default:
        case Logger::Type::INFO:
            os << "INFO";
            break;
        case Logger::Type::DEBUG:
            os << "DEBUG";
            break;
        case Logger::Type::WARNING:
            os << "WARNING";
            break;
        case Logger::Type::CRITICAL:
            os << "CRITICAL";
            break;
        }
        os << "]: " << msg.str() << "\n";

        std::cout << os.str();
        if (file)
        {
            file << os.str();
            file.flush();
        }
    }

    void write(Type type, const std::string &msg)
    {
        std::ostringstream os;
        os << "[" << Stopwatch::getTime() << "][";
        switch (type)
        {
        default:
        case Logger::Type::INFO:
            os << "INFO";
            break;
        case Logger::Type::DEBUG:
            os << "DEBUG";
            break;
        case Logger::Type::WARNING:
            os << "WARNING";
            break;
        case Logger::Type::CRITICAL:
            os << "CRITICAL";
            break;
        }
        os << "]: " << msg << "\n";

        std::cout << os.str();
        if (file)
        {
            file << os.str();
            file.flush();
        }
    }

    void writeException(const Exception &exception)
    {
        std::ostringstream os;
        os << "  ==========  EXCEPTION  ==========  " << "\n"
            << "Exception found in function " << exception.getFunction() << "\n"
            << "File: " << exception.getFile() << "\n"
            << "Line: " << exception.getLine() << "\n"
            << "  -------------  MSG  -------------  " << "\n"
            << exception.getMsg() << "\n";
        std::cout << os.str();
        if (file)
        {
            file << os.str();
            file.flush();
        }
    }

private:
    std::ofstream file;
};