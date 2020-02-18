#include "TsBuilder.hpp"

#include <QtXml>

void TsBuilder::build(const std::string &output_filename,
                      Translations trs) const
{
    QFile output(output_filename.c_str());
    if (!output.open(QFile::WriteOnly | QFile::Truncate)) {
        qDebug() << "can't open file" << output.fileName();
        return;
    }

    QDomDocument doc("TS");

    auto root = doc.createElement("TS");
    root.setAttribute("version", "2.1");    // TODO: extract as parameter
    root.setAttribute("language", "en_US"); // TODO: extract as parameter

    QString lastContextName = "";
    QDomElement currentContext;

    for (const auto &tr : trs) {
        if (lastContextName != tr.name || currentContext.isNull()) {
            QDomElement name = doc.createElement("name");
            name.appendChild(doc.createTextNode(tr.name));
            currentContext = doc.createElement("context");
            currentContext.appendChild(name);
            root.appendChild(currentContext);
        }

        auto message = doc.createElement("message");

        auto source = doc.createElement("source");
        source.appendChild(doc.createTextNode(tr.source));
        message.appendChild(source);

        if (!tr.comment.isEmpty()) {
            auto comment = doc.createElement("comment");
            comment.appendChild(doc.createTextNode(tr.comment));
            message.appendChild(comment);
        }

        auto translation = doc.createElement("translation");
        translation.appendChild(doc.createTextNode(tr.translation));
        if (tr.translation.isEmpty()) {
            translation.setAttribute("type", "unfinished");
        }
        message.appendChild(translation);

        currentContext.appendChild(message);

        lastContextName = tr.name;
    }

    doc.appendChild(root);

    QByteArray trText;
    QTextStream stream(&trText);
    stream << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    stream << doc.toString(4);

    QList<QByteArray> lines = trText.split('\n');
    trText.clear();
    for (QByteArray line : lines) {
        if (line.startsWith("    ")) {
            line.remove(0, 4);
        }
        trText.append(line);
        if (line != "\n") {
            trText.append('\n');
        }
    }

    output.write(trText);
    output.close();
}
