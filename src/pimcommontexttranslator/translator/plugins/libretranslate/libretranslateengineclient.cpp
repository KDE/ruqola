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
    auto enginePlugin = new LibreTranslateEnginePlugin();
    connect(this, &LibreTranslateEngineClient::configureChanged, enginePlugin, &LibreTranslateEnginePlugin::slotConfigureChanged);
    return enginePlugin;
}

QMap<PimCommonTextTranslator::TranslatorUtil::Language, QString> LibreTranslateEngineClient::supportedLanguages()
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

bool LibreTranslateEngineClient::isSupported(PimCommonTextTranslator::TranslatorUtil::Language lang) const
{
    switch (lang) {
    case PimCommonTextTranslator::TranslatorUtil::sq:
    case PimCommonTextTranslator::TranslatorUtil::af:
    case PimCommonTextTranslator::TranslatorUtil::hy:
    case PimCommonTextTranslator::TranslatorUtil::az:
    case PimCommonTextTranslator::TranslatorUtil::eu:
    case PimCommonTextTranslator::TranslatorUtil::be:
    case PimCommonTextTranslator::TranslatorUtil::bs:
    case PimCommonTextTranslator::TranslatorUtil::bg:
    case PimCommonTextTranslator::TranslatorUtil::ca:
    case PimCommonTextTranslator::TranslatorUtil::hr:
    case PimCommonTextTranslator::TranslatorUtil::cs:
    case PimCommonTextTranslator::TranslatorUtil::da:
    case PimCommonTextTranslator::TranslatorUtil::nl:
    case PimCommonTextTranslator::TranslatorUtil::et:
    case PimCommonTextTranslator::TranslatorUtil::tl:
    case PimCommonTextTranslator::TranslatorUtil::fi:
    case PimCommonTextTranslator::TranslatorUtil::gl:
    case PimCommonTextTranslator::TranslatorUtil::ka:
    case PimCommonTextTranslator::TranslatorUtil::el:
    case PimCommonTextTranslator::TranslatorUtil::iw:
    case PimCommonTextTranslator::TranslatorUtil::hmong:
    case PimCommonTextTranslator::TranslatorUtil::hu:
    case PimCommonTextTranslator::TranslatorUtil::is:
    case PimCommonTextTranslator::TranslatorUtil::lt:
    case PimCommonTextTranslator::TranslatorUtil::mk:
    case PimCommonTextTranslator::TranslatorUtil::ms:
    case PimCommonTextTranslator::TranslatorUtil::mt:
    case PimCommonTextTranslator::TranslatorUtil::no:
    case PimCommonTextTranslator::TranslatorUtil::fa:
    case PimCommonTextTranslator::TranslatorUtil::ro:
    case PimCommonTextTranslator::TranslatorUtil::sk:
    case PimCommonTextTranslator::TranslatorUtil::sl:
    case PimCommonTextTranslator::TranslatorUtil::sw:
    case PimCommonTextTranslator::TranslatorUtil::sv:
    case PimCommonTextTranslator::TranslatorUtil::th:
    case PimCommonTextTranslator::TranslatorUtil::uk:
    case PimCommonTextTranslator::TranslatorUtil::ur:
    case PimCommonTextTranslator::TranslatorUtil::cy:
    case PimCommonTextTranslator::TranslatorUtil::yi:
        return false;
    default:
        break;
    }
    return true;
}
