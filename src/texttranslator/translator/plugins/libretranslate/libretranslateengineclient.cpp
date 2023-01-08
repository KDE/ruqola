/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "libretranslateengineclient.h"
#include "libretranslateengineconfiguredialog.h"
#include "libretranslateengineplugin.h"
#include "libretranslateengineutil.h"
#include "translator/misc/translatorutil.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QPointer>

LibreTranslateEngineClient::LibreTranslateEngineClient(QObject *parent)
    : TextTranslator::TranslatorEngineClient{parent}
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

TextTranslator::TranslatorEnginePlugin *LibreTranslateEngineClient::createTranslator()
{
    auto enginePlugin = new LibreTranslateEnginePlugin();
    connect(this, &LibreTranslateEngineClient::configureChanged, enginePlugin, &LibreTranslateEnginePlugin::slotConfigureChanged);
    return enginePlugin;
}

QMap<TextTranslator::TranslatorUtil::Language, QString> LibreTranslateEngineClient::supportedLanguages()
{
    if (mLanguages.isEmpty()) {
        mLanguages = fillLanguages();
    }
    return mLanguages;
}

bool LibreTranslateEngineClient::hasConfigurationDialog() const
{
    return true;
}

void LibreTranslateEngineClient::showConfigureDialog(QWidget *parentWidget)
{
    QPointer<LibreTranslateEngineConfigureDialog> dlg = new LibreTranslateEngineConfigureDialog(parentWidget);
    KConfigGroup myGroup(KSharedConfig::openConfig(), LibreTranslateEngineUtil::groupName());
    QString serverUrl = myGroup.readEntry(LibreTranslateEngineUtil::serverUrlKey(), LibreTranslateEngineUtil::defaultServerUrl());
    if (serverUrl.isEmpty()) {
        serverUrl = LibreTranslateEngineUtil::defaultServerUrl();
    }
    serverUrl = LibreTranslateEngineUtil::adaptUrl(serverUrl);
    dlg->setServerUrl(serverUrl);
    dlg->setServerRequiredApiKey(myGroup.readEntry(LibreTranslateEngineUtil::serverRequiredApiKey(), false));
    dlg->setApiKey(myGroup.readEntry(LibreTranslateEngineUtil::apiGroupName(), QString()));
    if (dlg->exec()) {
        const QString serverUrl = dlg->serverUrl();
        const bool requiredApiKey = dlg->serverRequiredApiKey();
        myGroup.writeEntry(LibreTranslateEngineUtil::serverUrlKey(), serverUrl);
        myGroup.writeEntry(LibreTranslateEngineUtil::serverRequiredApiKey(), requiredApiKey);
        // TODO get api key // Store in api?
        myGroup.writeEntry(LibreTranslateEngineUtil::apiGroupName(), dlg->apiKey());
        myGroup.sync();
        Q_EMIT configureChanged();
    }
    delete dlg;
}

bool LibreTranslateEngineClient::isSupported(TextTranslator::TranslatorUtil::Language lang) const
{
    switch (lang) {
    case TextTranslator::TranslatorUtil::sq:
    case TextTranslator::TranslatorUtil::af:
    case TextTranslator::TranslatorUtil::hy:
    case TextTranslator::TranslatorUtil::az:
    case TextTranslator::TranslatorUtil::eu:
    case TextTranslator::TranslatorUtil::be:
    case TextTranslator::TranslatorUtil::bs:
    case TextTranslator::TranslatorUtil::bg:
    case TextTranslator::TranslatorUtil::ca:
    case TextTranslator::TranslatorUtil::hr:
    case TextTranslator::TranslatorUtil::cs:
    case TextTranslator::TranslatorUtil::da:
    case TextTranslator::TranslatorUtil::nl:
    case TextTranslator::TranslatorUtil::et:
    case TextTranslator::TranslatorUtil::tl:
    case TextTranslator::TranslatorUtil::fi:
    case TextTranslator::TranslatorUtil::gl:
    case TextTranslator::TranslatorUtil::ka:
    case TextTranslator::TranslatorUtil::el:
    case TextTranslator::TranslatorUtil::iw:
    case TextTranslator::TranslatorUtil::hmong:
    case TextTranslator::TranslatorUtil::hu:
    case TextTranslator::TranslatorUtil::is:
    case TextTranslator::TranslatorUtil::lt:
    case TextTranslator::TranslatorUtil::mk:
    case TextTranslator::TranslatorUtil::ms:
    case TextTranslator::TranslatorUtil::mt:
    case TextTranslator::TranslatorUtil::no:
    case TextTranslator::TranslatorUtil::fa:
    case TextTranslator::TranslatorUtil::ro:
    case TextTranslator::TranslatorUtil::sk:
    case TextTranslator::TranslatorUtil::sl:
    case TextTranslator::TranslatorUtil::sw:
    case TextTranslator::TranslatorUtil::sv:
    case TextTranslator::TranslatorUtil::th:
    case TextTranslator::TranslatorUtil::uk:
    case TextTranslator::TranslatorUtil::ur:
    case TextTranslator::TranslatorUtil::cy:
    case TextTranslator::TranslatorUtil::yi:
        return false;
    default:
        break;
    }
    return true;
}
