////////////////////////////////////
#ifndef TLOGGER_H
#define TLOGGER_H

////////////////////////////////////
#include <iostream>
#include <string>

////////////////////////////////////
namespace TRiOLD
{
    class Logger;
    extern Logger LOG;  // global loger
}

////////////////////////////////////
namespace TRiOLD
{
    class Logger
    {
    public:
        enum Level
        {
            NOMESSAGE,  // everything will be ignored / without messages
            FATAL,      // critical errors that terminate the program
            ERROR,      // only errors will be wrote / error message type
            WARNING,    // errors and warnings will be wrote / warning message type
            INFO        // all messages will be wrote /info message type
        };

    public:
        virtual ~Logger();

        Logger(Level level = INFO, bool isWithCout = true);
        Logger(const std::string& filepath, Level level = WARNING, bool isWithCout = false);

        void init(Level level = INFO,
                  bool isWithCout = true, bool isWithPrefix = true, bool isWithTimeMoment = true);
        void init(const std::string& filepath, Level level = WARNING,
                  bool isWithCout = false, bool isWithPrefix = true, bool isWithTimeMoment = true);

    private:
        std::string m_filepath;
        Level m_level;

        bool m_isWithFile;
        bool m_isWithCout;
        bool m_isWithPrefix;
        bool m_isWithTimeMoment;

        //buffer
        std::string m_lastErrorMessage;
        std::string m_lastWarningMessage;
        std::string m_lastInfoMessage;

    private:
        bool _checkFile(const std::string& filepath);
        void _cout(const std::string& line);
        void _writeToFile(const std::string& line);
        void _addToBuffer(const std::string& line, Level level);
        std::string _getPrefix(Level level);
        std::string _getThisTimeMoment();

    public:
        void setFilepath(const std::string& filepath);
        void setLevel(Level level);
        void setFlag_isWithCout(bool flag);
        void setFlag_isWithPrefix(bool flag);
        void setFlag_isWithTimeMoment(bool flag);

        void writeFatal(const std::string& message);
        void writeErr(const std::string& message);
        void writeWar(const std::string& message);
        void writeInfo(const std::string& message);
        void write(const std::string& message, Level level);

        std::string getlastErrorMessage();
        std::string getlastWarningMessage();
        std::string getlastInfoMessage();
    };
}

////////////////////////////////////
#endif // TLOGGER_H
