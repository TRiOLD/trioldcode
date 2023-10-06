////////////////////////////////////
/// Created by TRiOLD -l-
/// Email: TRiOLD@email.ua
///
////////////////////////////////////
#include "TLogger.h"

#include <fstream>
#include <ctime>

////////////////////////////////////
namespace TRiOLD
{
    Logger LOG;     // global loger
}

////////////////////////////////////
namespace TRiOLD
{
    Logger::~Logger()
    {

    }

    Logger::Logger(Level level, bool isWithCout)
    {
        init(std::string(), level, isWithCout, true, true);
    }

    Logger::Logger(const std::string& filepath, Level level, bool isWithCout)
    {
        init(filepath, level, isWithCout, true, true);
    }

    void Logger::init(Level level,
                      bool isWithCout, bool isWithPrefix, bool isWithTimeMoment)
    {
        init(std::string(), level, isWithCout, isWithPrefix, isWithTimeMoment);
    }

    void Logger::init(const std::string& filepath, Level level,
                      bool isWithCout, bool isWithPrefix, bool isWithTimeMoment)
    {
        setFilepath(filepath);
        m_level = level;

        m_isWithCout = isWithCout;
        m_isWithPrefix = isWithPrefix;
        m_isWithTimeMoment = isWithTimeMoment;

        m_lastErrorMessage = std::string();
        m_lastWarningMessage = std::string();
        m_lastInfoMessage = std::string();
    }

    bool Logger::_checkFile(const std::string &filepath)
    {
        std::ofstream file(filepath, std::ios::app);
        bool res = file.is_open();
        file.close();
        return res;
    }

    void Logger::_cout(const std::string& line)
    {
        if(!m_isWithCout)
            return;
        std::cout << line << std::endl;
    }

    void Logger::_writeToFile(const std::string& line)
    {
        if(!m_isWithFile)
            return;

        std::ofstream file(m_filepath, std::ios::app);
        file << line << std::endl;
        file.close();
    }

    void Logger::_addToBuffer(const std::string& line, Level level)
    {
        switch(level)
        {
            case Level::ERROR:      m_lastErrorMessage = line;   return;
            case Level::WARNING:    m_lastWarningMessage = line; return;
            case Level::INFO:       m_lastInfoMessage = line;    return;
            default:                return;
        }
    }

    std::string Logger::_getPrefix(Level level)
    {
        if(!m_isWithPrefix)
            return std::string();

        switch(level)
        {
            case Level::FATAL:      return "FATAL! ";
            case Level::ERROR:      return "ERROR! ";
            case Level::WARNING:    return "WARNING! ";
            case Level::INFO:       return "INFO. ";
            default:                return std::string();
        }
    }

    std::string Logger::_getThisTimeMoment()
    {
        if(!m_isWithTimeMoment)
            return std::string();

        time_t rawtime;
        struct tm * timeinfo;
        char buffer[80];
        time (&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(buffer, 80, "[%Y-%m-%d %H:%M:%S] ", timeinfo);
        return std::string(buffer);
    }

    void Logger::setFilepath(const std::string& filepath)
    {
        if(_checkFile(filepath))
        {
            m_filepath = filepath;
            m_isWithFile = true;
        }
    }

    void Logger::setLevel(Level level)
    {
        m_level = level;
    }

    void Logger::setFlag_isWithCout(bool flag)
    {
        m_isWithCout = flag;
    }

    void Logger::setFlag_isWithPrefix(bool flag)
    {
        m_isWithPrefix = flag;
    }

    void Logger::setFlag_isWithTimeMoment(bool flag)
    {
        m_isWithTimeMoment = flag;
    }

    void Logger::writeFatal(const std::string& message)
    {
        write(message, Level::FATAL);
    }

    void Logger::writeErr(const std::string& message)
    {
        write(message, Level::ERROR);
    }

    void Logger::writeWar(const std::string& message)
    {
        write(message, Level::WARNING);
    }

    void Logger::writeInfo(const std::string& message)
    {
        write(message, Level::INFO);
    }

    void Logger::write(const std::string& message, Level level)
    {
        if(level > m_level)
            return;

        std::string line = _getThisTimeMoment() + _getPrefix(level) + message;
        _cout(line);
        _writeToFile(line);
        _addToBuffer(line, level);
    }

    std::string Logger::getlastErrorMessage()
    {
        return m_lastErrorMessage;
    }

    std::string Logger::getlastWarningMessage()
    {
        return m_lastWarningMessage;
    }

    std::string Logger::getlastInfoMessage()
    {
        return m_lastInfoMessage;
    }
}

////////////////////////////////////
