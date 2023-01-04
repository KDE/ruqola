/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "deeplengineclient.h"
#include "deeplengineconfiguredialog.h"
#include "deeplengineplugin.h"
#include "deeplengineutil.h"
#include "translator/misc/translatorutil.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QPointer>

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
    auto enginePlugin = new DeeplEnginePlugin();
    connect(this, &DeeplEngineClient::configureChanged, enginePlugin, &DeeplEnginePlugin::slotConfigureChanged);
    return enginePlugin;
}

QMap<PimCommonTextTranslator::TranslatorUtil::Language, QString> DeeplEngineClient::supportedLanguages()
{
    if (mLanguages.isEmpty()) {
        mLanguages = fillLanguages();
    }
    return mLanguages;
}

bool DeeplEngineClient::isSupported(PimCommonTextTranslator::TranslatorUtil::Language lang) const
{
#if 0
    switch (lang) {
    case PimCommonTextTranslator::TranslatorUtil:::
        return false;
    default:
        break;
    }
#endif
    return true;
}

bool DeeplEngineClient::hasConfigurationDialog() const
{
    return true;
}

void DeeplEngineClient::showConfigureDialog(QWidget *parentWidget)
{
    QPointer<DeeplEngineConfigureDialog> dlg = new DeeplEngineConfigureDialog(parentWidget);
    KConfigGroup myGroup(KSharedConfig::openConfig(), DeeplEngineUtil::groupName());
    dlg->setUseFreeLicenceKey(myGroup.readEntry(DeeplEngineUtil::freeLicenseKey(), false));
    // TODO add support for apikey
    if (dlg->exec()) {
        myGroup.writeEntry(DeeplEngineUtil::freeLicenseKey(), dlg->useFreeLicenceKey());
        // TODO Save api key
        myGroup.sync();
        Q_EMIT configureChanged();
    }
    delete dlg;
}
