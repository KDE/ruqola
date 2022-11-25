/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "deeplengineclient.h"
#include "deeplengineplugin.h"
#include "translator/misc/translatorutil.h"
#include <KLocalizedString>

DeeplEngineClient::DeeplEngineClient(QObject *parent)
    : PimCommonTextTranslator::TranslatorEngineClient{parent}
{
}

DeeplEngineClient::~DeeplEngineClient() = default;

QString DeeplEngineClient::name() const
{
    return QStringLiteral("deepl");
}

QString DeeplEngineClient::translatedName() const
{
    return i18n("DeepL");
}

PimCommonTextTranslator::TranslatorEnginePlugin *DeeplEngineClient::createTranslator()
{
    return new DeeplEnginePlugin();
}

QVector<QPair<QString, QString>> DeeplEngineClient::supportedLanguages()
{
    if (mLanguages.isEmpty()) {
        mLanguages = PimCommonTextTranslator::TranslatorUtil::genericLanguages();
    }
    return mLanguages;
}

bool DeeplEngineClient::hasConfigurationDialog() const
{
    return true;
}
