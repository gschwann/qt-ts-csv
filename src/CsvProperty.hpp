#pragma once

#include <QByteArray>
#include <string>

class CsvProperty
{
  public:
    explicit CsvProperty(std::string field_sep, std::string string_sep) :
        field_separator{ field_sep }, string_separator{ string_sep }
    {
    }

    QByteArray stringSeparator() const
    {
        return QByteArray(string_separator.c_str());
    }
    QByteArray fieldSeparator() const
    {
        return QByteArray(field_separator.c_str());
    }

    std::string field_separator;
    std::string string_separator;
};
