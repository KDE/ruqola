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

    enum LanguageType {
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
        yi,
        jv_yandex,
        zn_yandex,
    };
    Q_REQUIRED_RESULT static QVector<QPair<QString, QString>> genericLanguages();
    Q_REQUIRED_RESULT static QVector<QPair<QString, QString>> googleSpecificLanguages();
    Q_REQUIRED_RESULT static QVector<QPair<QString, QString>> yandexSpecificLanguages();
    Q_REQUIRED_RESULT static QVector<QPair<QString, QString>> bingSpecificLanguages();

    Q_REQUIRED_RESULT QPair<QString, QString> pair(TranslatorUtil::LanguageType lang);
    Q_REQUIRED_RESULT static TranslatorEngineBase *switchEngine(TranslatorEngineBase::TranslatorEngine engineType, QObject *parent);
    Q_REQUIRED_RESULT static TranslatorEngineBase::TranslatorEngine loadEngineSettings();
    Q_REQUIRED_RESULT static QString loadEngine();
    Q_REQUIRED_RESULT static QVector<QPair<QString, QString>> supportedLanguages(const QString &engineTypeStr);
    Q_REQUIRED_RESULT static TranslatorEngineBase::TranslatorEngine convertStringToTranslatorEngine(const QString &engineTypeStr);
    static void saveEngineSettings(const QString &engineName);
    Q_REQUIRED_RESULT static QString groupTranslateName();
    Q_REQUIRED_RESULT static QString engineTranslateName();
    Q_REQUIRED_RESULT static QString defaultEngineName();
    Q_REQUIRED_RESULT static bool hasConfigureDialog(TranslatorEngineBase::TranslatorEngine engineType);
};
