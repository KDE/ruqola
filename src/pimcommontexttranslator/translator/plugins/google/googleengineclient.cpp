/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "googleengineclient.h"
#include "googleengineplugin.h"
#include "translator/misc/translatorutil.h"
#include <KLocalizedString>

GoogleEngineClient::GoogleEngineClient(QObject *parent)
    : PimCommonTextTranslator::TranslatorEngineClient{parent}
{
}

GoogleEngineClient::~GoogleEngineClient() = default;

QString GoogleEngineClient::name() const
{
    return QStringLiteral("google");
}

QString GoogleEngineClient::translatedName() const
{
    return i18n("Google");
}

PimCommonTextTranslator::TranslatorEnginePlugin *GoogleEngineClient::createTranslator()
{
    return new GoogleEnginePlugin();
}

QVector<QPair<QString, QString>> GoogleEngineClient::supportedLanguages()
{
    if (mLanguages.isEmpty()) {
        mLanguages = PimCommonTextTranslator::TranslatorUtil::genericLanguages();
        PimCommonTextTranslator::TranslatorUtil translatorUtil;
        QVector<QPair<QString, QString>> langLanguage;
        mLanguages.append(translatorUtil.pair(PimCommonTextTranslator::TranslatorUtil::zh_cn_google)); // For google only
        mLanguages.append(translatorUtil.pair(PimCommonTextTranslator::TranslatorUtil::zh_tw_google)); // For google only
    }
    return mLanguages;
}
