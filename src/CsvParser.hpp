#pragma once

#include "CsvProperty.hpp"
#include "TranslationObject.hpp"

class CsvParser
{
  public:
    explicit CsvParser(CsvProperty property);

    Translations parse(const std::string &filename) const;

  private:
    QList<QByteArray> joinFields(const QList<QByteArray> input) const;
    QByteArray unquote(const QByteArray input) const;

    CsvProperty property_;
};
