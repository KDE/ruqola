/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "translatorenginebase.h"
#include <QMap>
#include <QPair>
#include <QString>

class LIBRUQOLACORE_EXPORT TranslatorUtil
{
public:
    TranslatorUtil();

    struct TranslatorSettings {
        QString engine;
        QString from;
        QString to;
    };

    enum languages {
        automatic,
        en,
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
        zh_cn_google, // For google only
        zh_tw_google, // For google only
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
        yi
    };
    Q_REQUIRED_RESULT QPair<QString, QString> pair(TranslatorUtil::languages lang);
    Q_REQUIRED_RESULT static TranslatorEngineBase *switchEngine(TranslatorEngineBase::TranslatorEngine engineType, QObject *parent);
    Q_REQUIRED_RESULT static TranslatorEngineBase::TranslatorEngine loadEngineSettings();
    Q_REQUIRED_RESULT static QString loadEngine();
    static void saveEngineSettings(const QString &engineName);
};
