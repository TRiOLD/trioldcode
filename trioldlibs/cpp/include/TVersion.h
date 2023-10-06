////////////////////////////////////
/// Created by TRiOLD -l-
/// Email: TRiOLD@email.ua
///
////////////////////////////////////
#ifndef TVERSION_H
#define TVERSION_H

////////////////////////////////////
#include <string>

////////////////////////////////////
namespace TRiOLD
{
    class Version
    {
    public:
        enum Label
        {
            INDEF,
            DEV,
            ALPHA,
            BETA,
            RELEASE,
            FINAL
        };

    public:
        virtual ~Version();

        Version(unsigned short int major = 0,
                unsigned short int minor = 0,
                unsigned short int path = 0,
                Label label = Label::INDEF);
        void init(unsigned short int major = 0,
                  unsigned short int minor = 0,
                  unsigned short int path = 0,
                  Label label = Label::INDEF);

    private:
        unsigned short int m_major;
        unsigned short int m_minor;
        unsigned short int m_path;
        Label m_label;

    public:
        void setMajorVersion(unsigned short int major);
        void setMinorVersion(unsigned short int minor);
        void setPathVersion(unsigned short int path);
        void setLabel(Label label);

        unsigned short int getMajorVersion() const;
        unsigned short int getMinorVersion() const;
        unsigned short int getPathVersion() const;
        Label getLabel() const;

        std::string toString(bool isWithLabel = false) const;

    public:
        friend bool operator == (const Version &v1, const Version &v2);
        friend bool operator != (const Version &v1, const Version &v2);
        friend bool operator > (const Version &v1, const Version &v2);
        friend bool operator <= (const Version &v1, const Version &v2);
        friend bool operator < (const Version &v1, const Version &v2);
        friend bool operator >= (const Version &v1, const Version &v2);
    };
}

////////////////////////////////////
#endif // TVERSION_H
