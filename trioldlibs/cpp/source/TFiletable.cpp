////////////////////////////////////
/// Created by TRiOLD -l-
/// Email: TRiOLD@email.ua
///
////////////////////////////////////
#include <sstream>
#include <iomanip>

#include "TFiletable.h"

////////////////////////////////////
namespace TRiOLD
{
    Filetable::~Filetable()
    {
        close();
    }

    Filetable::Filetable()
    {
        close();
    }

    Filetable::Filetable(const std::string& filepath, OpenMode openMode, Extension extension)
    {
        open(filepath, openMode, extension);
    }

    bool Filetable::open(const std::string& filepath, OpenMode openMode, Extension extension)
    {
        switch (openMode)
        {
            case IN:    m_file.open(filepath, std::ios::in);  break;
            case OUT:   m_file.open(filepath, std::ios::out); break;
            case ATE:   m_file.open(filepath, std::ios::ate); break;
            default:
            {
                close();
                return false;
            }
        }

        if(!m_file.is_open())
        {
            close();
            return false;
        }

        if(extension == UNKNOWN)
        {
            extension = _parseExtension(filepath);
            if(extension == UNKNOWN)
            {
                close();
                return false;
            }
        }
        m_extension = extension;
        _initSeparator();

        m_filepath = filepath;
        m_openMode = openMode;
        m_rowCounter = 0;
        return true;
    }

    void Filetable::close()
    {
        m_filepath = std::string();
        m_extension = UNKNOWN;
        m_openMode = CLOSE;
        _initSeparator();

        m_file.close();
        m_file = std::fstream();
        m_rowCounter = 0;
    }

    Filetable::Extension Filetable::_parseExtension(const std::string& filepath)
    {
        std::string ext = filepath.substr(filepath.find_last_of(".") + 1);
        if(ext == "tsv" || ext == "TSV")
            return TSV;
        if(ext == "csv" || ext == "CSV")
            return CSV;
        return UNKNOWN;
    }

    void Filetable::_initSeparator()
    {
        switch (m_extension)
        {
            case TSV:   m_delimiter = "\t"; return;
            case CSV:   m_delimiter =  ","; return;
            default:    m_delimiter =   ""; return;
        }
    }

    bool Filetable::isOpen() const
    {
        return m_file.is_open();
    }

    bool Filetable::isEOF() const
    {
        return m_file.eof();
    }

    std::size_t Filetable::getLastRowIndex() const
    {
        return m_rowCounter;
    }

    Filetable::Row Filetable::readNextRow()
    {
        if(isOpen() && m_openMode == IN)
        {
            std::string line;
            if(std::getline(m_file, line))
                return Row(line, m_delimiter);
        }
        return Row();
    }

    void Filetable::writeBackRow(const Filetable::Row& row)
    {
        if(isOpen() && (m_openMode == OUT || m_openMode == ATE))
            m_file << row.toLine(m_delimiter) << '\n';
    }

    Filetable::operator bool() const
    {
        return (isOpen() && !isEOF());
    }

    Filetable& operator >> (Filetable &table, Filetable::Row& row)
    {
        row = table.readNextRow();
        return table;
    }

    Filetable& operator << (Filetable &table, const Filetable::Row& row)
    {
        table.writeBackRow(row);
        return table;
    }
}


////////////////////////////////////
namespace TRiOLD
{
    Filetable::Row::~Row()
    {

    }

    Filetable::Row::Row()
    {

    }

    Filetable::Row::Row(const std::vector<Filetable::Token>& tokens)
    {
        m_tokens = tokens;
    }

    Filetable::Row::Row(const std::string &line, const std::string& delimiter)
    {
        m_tokens = _parseLine(line, delimiter);
    }

    std::vector<Filetable::Token> Filetable::Row::_parseLine(const std::string& line, const std::string& delimiter)
    {
        std::string cell;
        std::vector<Token> res;
        size_t pos_start = 0, pos_end;
        while((pos_end = line.find(delimiter, pos_start)) != std::string::npos)
        {
            cell = line.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delimiter.size();
            res.push_back(cell);
        }
        res.push_back(line.substr(pos_start));
        return res;
    }

    std::string Filetable::Row::toLine(const std::string& delimiter) const
    {
        if(m_tokens.empty())
            return std::string();

        std::stringstream s;
        for(std::size_t i = 0; i < m_tokens.size()-1; ++i)
            s << m_tokens[i] << delimiter;
        s << m_tokens[m_tokens.size()-1];
        return s.str();
    }

    std::size_t Filetable::Row::size()
    {
        return m_tokens.size();
    }

    Filetable::Token Filetable::Row::at(std::size_t index)
    {
        return m_tokens.at(index);
    }

    Filetable::Token& Filetable::Row::operator [](std::size_t index)
    {
        return m_tokens[index];
    }

    const Filetable::Token& Filetable::Row::operator [] (std::size_t index) const
    {
        return m_tokens[index];
    }
}


////////////////////////////////////
namespace TRiOLD
{
    Filetable::Token::~Token()
    {

    }

    Filetable::Token::Token()
    {
        m_value = std::string();
    }

    Filetable::Token::Token(int value)
    {
        setValue(value);
    }

    Filetable::Token::Token(double value, int precision)
    {
         setValue(value, precision);
    }

    Filetable::Token::Token(const std::string &value)
    {
        setValue(value);
    }

    void Filetable::Token::setValue(bool value)
    {
        m_value = (value) ? "true" : "false";
    }

    void Filetable::Token::setValue(int value)
    {
        m_value = std::to_string(value);
    }

    void Filetable::Token::setValue(double value, int precision)
    {
        std::stringstream s;
        if( precision < 0 )
            s << value;
        else
        s << std::setiosflags(std::ios_base::fixed)
          << std::setprecision(precision) << value;
        m_value = s.str();
    }

    void Filetable::Token::setValue(const std::string &value)
    {
        m_value = value;
    }

    bool Filetable::Token::toBool() const
    {
        if(m_value == "true" ||
           m_value == "True" ||
           m_value == "TRUE" ||
           m_value == "+" || toInt())
            return true;
        return false;
    }

    int Filetable::Token::toInt() const
    {
        return std::stoi(m_value);
    }

    double Filetable::Token::toDouble() const
    {
        return std::stod(m_value);
    }

    std::string Filetable::Token::toString() const
    {
        return m_value;
    }

    Filetable::Token::operator bool() const
    {
        return !m_value.empty();
    }

    std::ostream& operator << (std::ostream& str, const Filetable::Token& token)
    {
        str << token.toString();
        return str;
    }
}

////////////////////////////////////
