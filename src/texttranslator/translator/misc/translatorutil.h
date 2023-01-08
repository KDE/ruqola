/*
  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "texttranslator_export.h"
#include <QPair>
#include <QString>

class QComboBox;

namespace TextTranslator
{
class TEXTTRANSLATOR_EXPORT TranslatorUtil
{
public:
    TranslatorUtil();

    struct TranslatorSettings {
        QString engine;
        QString from;
        QString to;
    };

    enum Language : int {
        automatic = 0,
        en,
        bs,
        hmong,
        zh,
        zt,
        nl,
        fr,
        de,
        el,
        it,
        ja,
        ko,
        pt,
        ru,
        es,
        af,
        sq,
        ar,
        hy,
        az,
        eu,
        be,
        bg,
        ca,
        hr,
        cs,
        da,
        et,
        tl,
        fi,
        gl,
        ka,
        ht,
        iw,
        hi,
        hu,
        is,
        id,
        ga,
        lv,
        lt,
        mk,
        ms,
        mt,
        no,
        fa,
        pl,
        ro,
        sr,
        sk,
        sl,
        sw,
        sv,
        th,
        tr,
        uk,
        ur,
        vi,
        cy,
        yi,
        lastLanguage = yi
    };
    void addItemToFromComboBox(QComboBox *combo, const QString &languageCode, const QString &translatedStr);

    // Only for test
    Q_REQUIRED_RESULT static QMap<TranslatorUtil::Language, QString> translatedLanguages();

    Q_REQUIRED_RESULT static QString loadEngine();
    static void saveEngineSettings(const QString &engineName);
    Q_REQUIRED_RESULT static QString groupTranslateName();
    Q_REQUIRED_RESULT static QString engineTranslateName();
    Q_REQUIRED_RESULT static QString defaultEngineName();
    Q_REQUIRED_RESULT static QString translatedLanguage(TranslatorUtil::Language lang);
    Q_REQUIRED_RESULT static QString languageCode(TranslatorUtil::Language lang);
    Q_REQUIRED_RESULT static QString searchI18nFromLanguage(const QString &langCode);
};
}
