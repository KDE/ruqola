/*
  SPDX-FileCopyrightText: 2012-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "pimcommontexttranslator_export.h"
#include <PimCommonTextTranslator/TranslatorEngineBase>
#include <QMap>
#include <QPair>
#include <QString>

class QComboBox;

namespace PimCommonTextTranslator
{
class PIMCOMMONTEXTTRANSLATOR_EXPORT TranslatorUtil
{
public:
    TranslatorUtil();

    struct TranslatorSettings {
        QString engine;
        QString from;
        QString to;
    };

    void addItemToFromComboBox(QComboBox *combo, const QPair<QString, QString> &pair);

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
        yi,
        jv_yandex,
        zn_yandex,
    };
    Q_REQUIRED_RESULT static QVector<QPair<QString, QString>> genericLanguages();
    Q_REQUIRED_RESULT static QVector<QPair<QString, QString>> googleSpecificLanguages();
    Q_REQUIRED_RESULT static QVector<QPair<QString, QString>> yandexSpecificLanguages();
    Q_REQUIRED_RESULT static QVector<QPair<QString, QString>> bingSpecificLanguages();

    Q_REQUIRED_RESULT QPair<QString, QString> pair(TranslatorUtil::languages lang);
    Q_REQUIRED_RESULT static PimCommonTextTranslator::TranslatorEngineBase *
    switchEngine(PimCommonTextTranslator::TranslatorEngineBase::TranslatorEngine engineType, QObject *parent);
    static void fillComboboxSettings(QComboBox *combo);
    Q_REQUIRED_RESULT static PimCommonTextTranslator::TranslatorEngineBase::TranslatorEngine loadEngineSettings();
    Q_REQUIRED_RESULT static QString loadEngine();
    static void saveEngineSettings(const QString &engineName);
    Q_REQUIRED_RESULT static QString groupTranslateName();
    Q_REQUIRED_RESULT static QString engineTranslateName();
    Q_REQUIRED_RESULT static QString defaultEngineName();
    Q_REQUIRED_RESULT static bool hasConfigureDialog(TranslatorEngineBase::TranslatorEngine engineType);
    Q_REQUIRED_RESULT static PimCommonTextTranslator::TranslatorEngineBase::TranslatorEngine convertStringToTranslatorEngine(const QString &engineTypeStr);
    Q_REQUIRED_RESULT static QVector<QPair<QString, QString>> supportedLanguages(const QString &engineTypeStr);
};
}
