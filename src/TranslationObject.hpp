#pragma once

#include <QString>
#include <vector>

struct TranslationObj {
    QString name;
    QString source;
    QString translation;
    QString comment;
};

using Translations = std::vector<TranslationObj>;
