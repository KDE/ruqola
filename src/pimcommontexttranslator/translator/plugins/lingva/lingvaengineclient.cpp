/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "lingvaengineclient.h"
#include "lingvaengineplugin.h"
#include "translator/misc/translatorutil.h"
#include <KLocalizedString>

LingvaEngineClient::LingvaEngineClient(QObject *parent)
    : PimCommonTextTranslator::TranslatorEngineClient{parent}
{
}

LingvaEngineClient::~LingvaEngineClient() = default;

QString LingvaEngineClient::name() const
{
    return QStringLiteral("lingva");
}

QString LingvaEngineClient::translatedName() const
{
    return i18n("Lingva");
}

PimCommonTextTranslator::TranslatorEnginePlugin *LingvaEngineClient::createTranslator()
{
    return new LingvaEnginePlugin();
}

QVector<QPair<QString, QString>> LingvaEngineClient::supportedLanguages()
{
    if (mLanguages.isEmpty()) {
        mLanguages = PimCommonTextTranslator::TranslatorUtil::genericLanguages();
    }
    return mLanguages;
}

bool LingvaEngineClient::hasConfigurationDialog() const
{
    return true;
}

void LingvaEngineClient::showConfigureDialog()
{
    // TODO
}
