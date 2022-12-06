/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "libretranslateengineclient.h"
#include "libretranslateengineplugin.h"
#include "translator/misc/translatorutil.h"
#include <KLocalizedString>

LibreTranslateEngineClient::LibreTranslateEngineClient(QObject *parent)
    : PimCommonTextTranslator::TranslatorEngineClient{parent}
{
}

LibreTranslateEngineClient::~LibreTranslateEngineClient() = default;

QString LibreTranslateEngineClient::name() const
{
    return QStringLiteral("libretranslate");
}

QString LibreTranslateEngineClient::translatedName() const
{
    return i18n("Libre Translate");
}

PimCommonTextTranslator::TranslatorEnginePlugin *LibreTranslateEngineClient::createTranslator()
{
    return new LibreTranslateEnginePlugin();
}

QVector<QPair<QString, QString>> LibreTranslateEngineClient::supportedLanguages()
{
    if (mLanguages.isEmpty()) {
        mLanguages = PimCommonTextTranslator::TranslatorUtil::genericLanguages();
    }
    return mLanguages;
}

bool LibreTranslateEngineClient::hasConfigurationDialog() const
{
    return true;
}

void LibreTranslateEngineClient::showConfigureDialog()
{
    // TODO
}
