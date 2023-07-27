////////////////////////////////////
/// Created by TRiOLD -l-
/// Email: TRiOLD@email.ua
///
////////////////////////////////////
#ifndef TECEXCEPTION_H
#define TECEXCEPTION_H

////////////////////////////////////
#include <exception>
#include <string>

////////////////////////////////////
namespace TRiOLD
{
    class Exception: public std::exception
    {
    public:
        Exception(const std::string& comment = "", int code = -1);

    private:
        int m_code;
        std::string m_comment;

    public:
        const char* what() const noexcept;
        std::string getComment() const;
        int getCode() const;
    };
}

////////////////////////////////////
#endif // TECEXCEPTION_H
