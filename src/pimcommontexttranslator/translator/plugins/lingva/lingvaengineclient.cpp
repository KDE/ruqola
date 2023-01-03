/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "lingvaengineclient.h"
#include "lingvaenginedialog.h"
#include "lingvaengineplugin.h"
#include "lingvaengineutil.h"
#include "translator/misc/translatorutil.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QPointer>

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
    auto enginePlugin = new LingvaEnginePlugin();
    connect(this, &LingvaEngineClient::configureChanged, enginePlugin, &LingvaEnginePlugin::slotConfigureChanged);
    return enginePlugin;
}

QMap<PimCommonTextTranslator::TranslatorUtil::Language, QString> LingvaEngineClient::supportedLanguages()
{
    if (mLanguages.isEmpty()) {
        mLanguages = fillLanguages();
    }
    return mLanguages;
}

bool LingvaEngineClient::hasConfigurationDialog() const
{
    return true;
}

void LingvaEngineClient::showConfigureDialog()
{
    QPointer<LingvaEngineDialog> dlg = new LingvaEngineDialog();
    KConfigGroup myGroup(KSharedConfig::openConfig(), LingvaEngineUtil::groupName());
    dlg->setServerUrl(myGroup.readEntry(LingvaEngineUtil::serverUrlKey(), LingvaEngineUtil::defaultServerUrl()));
    if (dlg->exec()) {
        const QString serverUrl = dlg->serverUrl();
        myGroup.writeEntry(LingvaEngineUtil::serverUrlKey(), serverUrl);
        myGroup.sync();
        Q_EMIT configureChanged();
    }
    delete dlg;
}

bool LingvaEngineClient::isSupported(PimCommonTextTranslator::TranslatorUtil::Language lang) const
{
    switch (lang) {
    case PimCommonTextTranslator::TranslatorUtil::ka:
    case PimCommonTextTranslator::TranslatorUtil::tl:
        return false;
    default:
        break;
    }
    return true;
}
