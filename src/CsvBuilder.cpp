#include "CsvBuilder.hpp"

#include <QDebug>
#include <QFile>
#include <QTextStream>

CsvBuilder::CsvBuilder(CsvProperty property) : property_{ property } {}

void CsvBuilder::build(const std::string &output_filename,
                       Translations trs) const
{
    QFile file(output_filename.c_str());

    if (!file.open(QFile::WriteOnly | QFile::Truncate)) {
        qDebug() << "can't open file";
        return;
    }

    QTextStream output(&file);

    output << quotedText("Context") << property_.field_separator.c_str()
           << quotedText("Input") << property_.field_separator.c_str()
           << quotedText("Output") << property_.field_separator.c_str()
           << quotedText("Comment") << "\n";

    for (const auto &tr : trs) {
        output << quotedText(tr.name) << property_.field_separator.c_str()
               << quotedText(tr.source) << property_.field_separator.c_str()
               << quotedText(tr.translation)
               << property_.field_separator.c_str() << quotedText(tr.comment)
               << "\n";
    }

    file.close();
}

QString CsvBuilder::quotedText(const QString &text) const
{
    const QString stringSeparator = property_.stringSeparator();
    QString quoted                = text;
    quoted.replace(stringSeparator, QString("\\%1").arg(stringSeparator));
    return QString("%1%2%1").arg(stringSeparator, text);
}
