////////////////////////////////////
/// Created by TRiOLD -l-
/// Email: TRiOLD@email.ua
///
////////////////////////////////////
#ifndef TFILETABLE_H
#define TFILETABLE_H

////////////////////////////////////
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

////////////////////////////////////
namespace TRiOLD
{
    class Filetable
    {
    public:
        class Row;
        class Token;

        enum Extension
        {
            UNKNOWN,
            TSV,    // tab-separated values (.tsv)
            CSV     // comma-separated values (.csv)
        };
        enum OpenMode
        {
            CLOSE,
            IN,     // open for reading
            OUT,    // open for writing (clearing the file first)
            ATE     // seek to the end of stream immediately after open
        };

    public:
        virtual ~Filetable();

        Filetable();
        Filetable(const std::string &filepath, OpenMode openMode = IN, Extension extension = UNKNOWN);
        bool open(const std::string &filepath, OpenMode openMode = IN, Extension extension = UNKNOWN);
        void close();

    private:
        std::string m_filepath;
        Extension m_extension;
        OpenMode m_openMode;
        std::string m_delimiter;

        std::fstream m_file;
        std::size_t m_rowCounter;

    private:
        Extension _parseExtension(const std::string &filepath);
        void _initSeparator();

    public:
        bool isOpen() const;
        bool isEOF() const;
        std::size_t getLastRowIndex() const;

        Row readNextRow();
        void writeBackRow(const Row& row);

        operator bool () const;
        friend Filetable& operator >> (Filetable &table, Row& row);
        friend Filetable& operator << (Filetable &table, const Row& row);
    };
}

////////////////////////////////////
namespace TRiOLD
{
    class Filetable::Row
    {
    public:
        virtual ~Row();

        Row();
        Row(const std::vector<Token>& tokens);
        Row(const std::string& line, const std::string& delimiter);

    private:
        std::vector<Token> m_tokens;

    private:
        std::vector<Token> _parseLine(const std::string& line, const std::string& delimiter);

    public:
        std::string toLine(const std::string& delimiter) const;

        std::size_t size();
        Token at(std::size_t index);
        Token& operator [] (std::size_t index);
        const Token& operator [] (std::size_t index) const;
    };
}

////////////////////////////////////
namespace TRiOLD
{
    class Filetable::Token
    {
    public:
        virtual ~Token();

        Token();
        Token(int value);
        Token(double value, int precision = -1);
        Token(const std::string& value);

    private:
        std::string m_value;

    public:
        void setValue(bool value);
        void setValue(int value);
        void setValue(double value, int precision = -1);
        void setValue(const std::string& value);

        bool toBool() const;
        int toInt() const;
        double toDouble() const;
        std::string toString() const;

        operator bool() const;
        friend std::ostream& operator << (std::ostream& str, const Token& token);
    };
}

////////////////////////////////////
#endif // TFILETABLE_H
