////////////////////////////////////
/// Created by TRiOLD -l-
/// Email: TRiOLD@email.ua
///
////////////////////////////////////
#include "TVersion.h"

////////////////////////////////////
namespace TRiOLD
{    
    Version::~Version()
    {

    }

    Version::Version(unsigned short major, unsigned short minor, unsigned short path, Label label)
    {
        init(major, minor, path, label);
    }

    void Version::init(unsigned short major, unsigned short minor, unsigned short path, Label label)
    {
        m_major = major;
        m_minor = minor;
        m_path = path;
        m_label = label;
    }

    void Version::setMajorVersion(unsigned short major)
    {
        m_major = major;
    }

    void Version::setMinorVersion(unsigned short minor)
    {
        m_minor = minor;
    }

    void Version::setPathVersion(unsigned short path)
    {
        m_path = path;
    }

    void Version::setLabel(Label label)
    {
        m_label = label;
    }

    unsigned short Version::getMajorVersion() const
    {
        return m_major;
    }

    unsigned short Version::getMinorVersion() const
    {
        return m_minor;
    }

    unsigned short Version::getPathVersion() const
    {
        return m_path;
    }

    Version::Label Version::getLabel() const
    {
        return m_label;
    }

    std::string Version::toString(bool isWithLabel) const
    {
        std::string res =
            std::to_string(m_major) + "." +
            std::to_string(m_minor) + "." +
            std::to_string(m_path);
        if(!isWithLabel || m_label == Label::INDEF)
            return res;

        switch (m_label)
        {
            case Label::DEV:        return res + "-dev";
            case Label::ALPHA:      return res + "-alpha";
            case Label::BETA:       return res + "-beta";
            case Label::RELEASE:    return res + "-release";
            case Label::FINAL:      return res + "-final";
            default: return res;
        }
    }

    bool operator == (const Version &v1, const Version &v2)
    {
        return (v1.m_major == v2.m_major && v1.m_minor == v2.m_minor &&
                v1.m_path == v2.m_path && v1.m_label == v2.m_label);
    }

    bool operator != (const Version &v1, const Version &v2)
    {
        return !(v1 == v2);
    }

    bool operator > (const Version &v1, const Version &v2)
    {
        if(v1.m_major > v2.m_major)
            return true;
        if(v1.m_major == v2.m_major && v1.m_minor > v2.m_minor)
            return true;
        if(v1.m_major == v2.m_major && v1.m_minor == v2.m_minor &&
            v1.m_path > v2.m_path)
            return true;
        if(v1.m_major == v2.m_major && v1.m_minor == v2.m_minor &&
           v1.m_path == v2.m_path && v1.m_label > v2.m_label)
            return true;
        return false;
    }

    bool operator >= (const Version &v1, const Version &v2)
    {
        return (v1 == v2 || v1 > v2);
    }

    bool operator < (const Version &v1, const Version &v2)
    {
        if(v1.m_major < v2.m_major)
            return true;
        if(v1.m_major == v2.m_major && v1.m_minor < v2.m_minor)
            return true;
        if(v1.m_major == v2.m_major && v1.m_minor == v2.m_minor &&
            v1.m_path < v2.m_path)
            return true;
        if(v1.m_major == v2.m_major && v1.m_minor == v2.m_minor &&
           v1.m_path == v2.m_path && v1.m_label < v2.m_label)
            return true;
        return false;
    }

    bool operator <= (const Version &v1, const Version &v2)
    {
        return (v1 == v2 || v1 < v2);
    }

}
////////////////////////////////////
