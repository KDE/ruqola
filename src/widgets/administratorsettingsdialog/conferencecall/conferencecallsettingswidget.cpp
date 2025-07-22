/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecallsettingswidget.h"

#include "connection.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "video-conference/videoconferenceprovidersjob.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QJsonArray>

using namespace Qt::Literals::StringLiterals;
ConferenceCallSettingsWidget::ConferenceCallSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
    , mDefaultProvider(new QComboBox(this))
    , mEnableDirectMessage(new QCheckBox(i18nc("@option:check", "Enable in direct messages"), this))
    , mEnablePublicChannels(new QCheckBox(i18nc("@option:check", "Enable in public channels"), this))
    , mEnablePrivateChannels(new QCheckBox(i18nc("@option:check", "Enable in public channels"), this))
    , mEnableTeams(new QCheckBox(i18nc("@option:check", "Enable in teams"), this))
    , mEnableMobileRinging(new QCheckBox(i18nc("@option:check", "Enable mobile ringing"), this))
{
    mDefaultProvider->setObjectName(u"mDefaultProvider"_s);
    addComboBox(i18n("Default Provider"), {}, mDefaultProvider, u"VideoConf_Default_Provider"_s);

    mEnableDirectMessage->setObjectName(u"mEnableDirectMessage"_s);
    addCheckBox(mEnableDirectMessage, u"VideoConf_Enable_DMs"_s);

    mEnablePublicChannels->setObjectName(u"mEnablePublicChannels"_s);
    addCheckBox(mEnablePublicChannels, u"VideoConf_Enable_Channels"_s);

    mEnablePrivateChannels->setObjectName(u"mEnablePrivateChannels"_s);
    addCheckBox(mEnablePrivateChannels, u"VideoConf_Enable_Groups"_s);

    mEnableTeams->setObjectName(u"mEnableTeams"_s);
    addCheckBox(mEnableTeams, u"VideoConf_Enable_Teams"_s);

    mEnableMobileRinging->setObjectName(u"mEnableMobileRinging"_s);
    addCheckBox(mEnableMobileRinging, u"VideoConf_Mobile_Ringing"_s);
}

ConferenceCallSettingsWidget::~ConferenceCallSettingsWidget() = default;

void ConferenceCallSettingsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    auto job = new RocketChatRestApi::VideoConferenceProvidersJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::VideoConferenceProvidersJob::videoConferenceProvidersDone, this, [this, mapSettings](const QJsonObject &obj) {
        // {"data":[{"key":"jitsi","label":"Jitsi"}],"success":true}
        QMap<QString, QString> maps;
        const QJsonArray array = obj["data"_L1].toArray();
        for (const QJsonValue &current : array) {
            const QJsonObject roleObject = current.toObject();
            maps.insert(roleObject["key"_L1].toString(), roleObject["label"_L1].toString());
        }
        // qDebug() << " list " << obj << " maps " << maps;
        fillComboBox(mDefaultProvider, maps);
        initializeWidget(mDefaultProvider, mapSettings, QString());
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start VideoConferenceProvidersJob job";
    }
    initializeWidget(mEnableDirectMessage, mapSettings, true);
    initializeWidget(mEnablePublicChannels, mapSettings, true);
    initializeWidget(mEnablePrivateChannels, mapSettings, true);
    initializeWidget(mEnableTeams, mapSettings, true);
    initializeWidget(mEnableMobileRinging, mapSettings, false);
}

#include "moc_conferencecallsettingswidget.cpp"
