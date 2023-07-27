////////////////////////////////////
/// Created by TRiOLD -l-
/// Email: TRiOLD@email.ua
///
////////////////////////////////////
#include "TException.h"

////////////////////////////////////
namespace TRiOLD
{
    Exception::Exception(const std::string& comment, int code)
    {
        m_comment = comment;
        m_code = code;
    }

    const char* Exception::what() const noexcept
    {
        return m_comment.c_str();
    }

    std::string Exception::getComment() const
    {
        return m_comment;
    }

    int Exception::getCode() const
    {
        return m_code;
    }
}
////////////////////////////////////
