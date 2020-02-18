#include "CsvParser.hpp"

#include <QDebug>
#include <QFile>

CsvParser::CsvParser(CsvProperty property) : property_{ property } {}

Translations CsvParser::parse(const std::string &filename) const
{
    const auto sep_line = '\n';
    QFile file(filename.c_str());

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "can't open file";
        return {};
    }

    std::vector<TranslationObj> tr_objs;

    auto content           = file.readAll();
    auto list              = content.split(sep_line);
    const auto sep         = property_.field_separator.front();
    const int column_count = list.first().split(sep).size();
    list.removeFirst();

    QByteArray last_line;
    for (const auto &l : list) {
        QByteArray line = l;
        if (!line.startsWith('"')) {
            line = last_line + '\n' + l;
        }
        auto fields = line.split(sep);
        fields      = joinFields(fields);

        if (fields.size() == column_count) {
            TranslationObj obj;
            obj.name        = unquote(fields.value(0));
            obj.source      = unquote(fields.value(1));
            obj.translation = unquote(fields.value(2));
            obj.comment     = unquote(fields.value(3));
            tr_objs.emplace_back(obj);
        }

        last_line = line;
    }

    tr_objs.erase(tr_objs.begin(), tr_objs.begin());
    return tr_objs;
}

QList<QByteArray> CsvParser::joinFields(const QList<QByteArray> input) const
{
    QList<QByteArray> output;
    const QByteArray stringSeparator = property_.stringSeparator();
    const QByteArray fieldSeparator  = property_.fieldSeparator();

    bool join = false;
    QByteArray field;
    for (auto &f : input) {
        bool append = false;

        if (join) {
            field += fieldSeparator;
            field += f;
        } else {
            field = f;
        }

        if (field.startsWith(stringSeparator) &&
            !field.endsWith(stringSeparator)) {
            join   = true;
            append = false;
        } else {
            append = true;
            join   = false;
        }

        if (append) {
            field = unquote(field);
            output.append(field);
        }
    }

    return output;
}

QByteArray CsvParser::unquote(const QByteArray input) const
{
    const QByteArray stringSeparator = property_.stringSeparator();
    QByteArray output                = input;
    if (output.startsWith(stringSeparator) &&
        output.endsWith(stringSeparator)) {
        output.remove(0, 1);
        output.remove(output.size() - 1, 1);
    }

    output.replace("\\" + stringSeparator, stringSeparator);

    return output;
}
