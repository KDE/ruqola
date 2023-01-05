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
    switch (lang) {
    case PimCommonTextTranslator::TranslatorUtil::automatic:
    case PimCommonTextTranslator::TranslatorUtil::bg:
    case PimCommonTextTranslator::TranslatorUtil::cs:
    case PimCommonTextTranslator::TranslatorUtil::da:
    case PimCommonTextTranslator::TranslatorUtil::de:
    case PimCommonTextTranslator::TranslatorUtil::en:
    case PimCommonTextTranslator::TranslatorUtil::el:
    case PimCommonTextTranslator::TranslatorUtil::es:
    case PimCommonTextTranslator::TranslatorUtil::et:
    case PimCommonTextTranslator::TranslatorUtil::fi:
    case PimCommonTextTranslator::TranslatorUtil::fr:
    case PimCommonTextTranslator::TranslatorUtil::hu:
    case PimCommonTextTranslator::TranslatorUtil::id:
    case PimCommonTextTranslator::TranslatorUtil::it:
    case PimCommonTextTranslator::TranslatorUtil::ja:
    case PimCommonTextTranslator::TranslatorUtil::lt:
    case PimCommonTextTranslator::TranslatorUtil::lv:
    case PimCommonTextTranslator::TranslatorUtil::nl:
    case PimCommonTextTranslator::TranslatorUtil::pl:
    case PimCommonTextTranslator::TranslatorUtil::pt:
    case PimCommonTextTranslator::TranslatorUtil::ro:
    case PimCommonTextTranslator::TranslatorUtil::ru:
    case PimCommonTextTranslator::TranslatorUtil::sk:
    case PimCommonTextTranslator::TranslatorUtil::sl:
    case PimCommonTextTranslator::TranslatorUtil::sv:
    case PimCommonTextTranslator::TranslatorUtil::tr:
    case PimCommonTextTranslator::TranslatorUtil::uk:
    case PimCommonTextTranslator::TranslatorUtil::zh:
        return true;
    default:
        break;
    }
    return false;
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
    dlg->setApiKey(myGroup.readEntry(DeeplEngineUtil::apiGroupName(), QString()));
    if (dlg->exec()) {
        myGroup.writeEntry(DeeplEngineUtil::freeLicenseKey(), dlg->useFreeLicenceKey());
        myGroup.writeEntry(DeeplEngineUtil::apiGroupName(), dlg->apiKey());
        myGroup.sync();
        Q_EMIT configureChanged();
    }
    delete dlg;
}
