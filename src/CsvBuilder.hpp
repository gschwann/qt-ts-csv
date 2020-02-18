#pragma once

#include "CsvProperty.hpp"
#include "TranslationObject.hpp"

class CsvBuilder
{
  public:
    explicit CsvBuilder(CsvProperty property);

    void build(const std::string &output_filename, Translations trs) const;

  private:
    QString quotedText(const QString &text) const;

    CsvProperty property_;
};
