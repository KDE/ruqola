/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

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
    QPointer<LibreTranslateEngineConfigureDialog> dlg = new LibreTranslateEngineConfigureDialog();
    KConfigGroup myGroup(KSharedConfig::openConfig(), LibreTranslateEngineUtil::groupName());
    dlg->setServerUrl(myGroup.readEntry(LibreTranslateEngineUtil::serverUrlKey(), QString()));
    if (dlg->exec()) {
        // TODO save
        Q_EMIT configureChanged();
    }
    delete dlg;
}
