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
    : TextTranslator::TranslatorEngineClient{parent}
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

TextTranslator::TranslatorEnginePlugin *DeeplEngineClient::createTranslator()
{
    auto enginePlugin = new DeeplEnginePlugin();
    connect(this, &DeeplEngineClient::configureChanged, enginePlugin, &DeeplEnginePlugin::slotConfigureChanged);
    return enginePlugin;
}

QMap<TextTranslator::TranslatorUtil::Language, QString> DeeplEngineClient::supportedLanguages()
{
    if (mLanguages.isEmpty()) {
        mLanguages = fillLanguages();
    }
    return mLanguages;
}

bool DeeplEngineClient::isSupported(TextTranslator::TranslatorUtil::Language lang) const
{
    switch (lang) {
    case TextTranslator::TranslatorUtil::automatic:
    case TextTranslator::TranslatorUtil::bg:
    case TextTranslator::TranslatorUtil::cs:
    case TextTranslator::TranslatorUtil::da:
    case TextTranslator::TranslatorUtil::de:
    case TextTranslator::TranslatorUtil::en:
    case TextTranslator::TranslatorUtil::el:
    case TextTranslator::TranslatorUtil::es:
    case TextTranslator::TranslatorUtil::et:
    case TextTranslator::TranslatorUtil::fi:
    case TextTranslator::TranslatorUtil::fr:
    case TextTranslator::TranslatorUtil::hu:
    case TextTranslator::TranslatorUtil::id:
    case TextTranslator::TranslatorUtil::it:
    case TextTranslator::TranslatorUtil::ja:
    case TextTranslator::TranslatorUtil::lt:
    case TextTranslator::TranslatorUtil::lv:
    case TextTranslator::TranslatorUtil::nl:
    case TextTranslator::TranslatorUtil::pl:
    case TextTranslator::TranslatorUtil::pt:
    case TextTranslator::TranslatorUtil::ro:
    case TextTranslator::TranslatorUtil::ru:
    case TextTranslator::TranslatorUtil::sk:
    case TextTranslator::TranslatorUtil::sl:
    case TextTranslator::TranslatorUtil::sv:
    case TextTranslator::TranslatorUtil::tr:
    case TextTranslator::TranslatorUtil::uk:
    case TextTranslator::TranslatorUtil::zh:
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
