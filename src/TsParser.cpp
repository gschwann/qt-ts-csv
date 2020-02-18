#include "TsParser.hpp"

#include <QFile>
#include <QtXml>

Translations TsParser::parse(const std::string &filename) const
{
    QDomDocument doc;
    QFile file(filename.c_str());

    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file)) {
        qDebug() << "can't open file";
        return {};
    }

    std::vector<TranslationObj> tr_objs;

    auto contexts = doc.elementsByTagName("context");
    for (int i = 0; i < contexts.size(); ++i) {
        auto node = contexts.item(i);
        TranslationObj msg;
        msg.name = node.firstChildElement("name").text();

        auto msgs = node.childNodes();
        for (auto j = 0; j < msgs.size(); j++) {
            auto msgNode = msgs.item(j);

            if (msgNode.nodeName() == "message") {
                msg.source = msgNode.firstChildElement("source").text();
                msg.translation =
                    msgNode.firstChildElement("translation").text();

                msg.comment = msgNode.firstChildElement("comment").text();
                QDomElement extraCommentNode =
                    msgNode.firstChildElement("extracomment");
                if (!extraCommentNode.isNull()) {
                    msg.comment = extraCommentNode.text();
                }

                if (std::find_if(tr_objs.begin(), tr_objs.end(),
                                 [&msg](const TranslationObj &tr) {
                                     return tr.source == msg.source && tr.name == msg.name;
                                 }) == tr_objs.end()) {
                    tr_objs.emplace_back(msg);
                }
            }
        }
    }

    return tr_objs;
}
