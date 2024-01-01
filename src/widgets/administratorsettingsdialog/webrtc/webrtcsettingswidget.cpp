/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "webrtcsettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>

WebRtcSettingsWidget::WebRtcSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnabled(new QCheckBox(i18n("Enabled"), this))
    , mEnablePublicChannels(new QCheckBox(i18n("Enable for Public Channels"), this))
    , mEnablePrivateChannels(new QCheckBox(i18n("Enable for Private Channels"), this))
    , mEnableDirectMessages(new QCheckBox(i18n("Enable for Direct Messages"), this))
    , mServer(new QLineEdit(this))
{
    mEnabled->setObjectName(QStringLiteral("mEnabled"));
    mMainLayout->addWidget(mEnabled);
    connectCheckBox(mEnabled, QStringLiteral("WebRTC_Enabled"));

    mEnablePublicChannels->setObjectName(QStringLiteral("mEnablePublicChannels"));
    mMainLayout->addWidget(mEnablePublicChannels);
    connectCheckBox(mEnablePublicChannels, QStringLiteral("WebRTC_Enable_Channel"));

    mEnablePrivateChannels->setObjectName(QStringLiteral("mEnablePrivateChannels"));
    mMainLayout->addWidget(mEnablePrivateChannels);
    connectCheckBox(mEnablePrivateChannels, QStringLiteral("WebRTC_Enable_Private"));

    mEnableDirectMessages->setObjectName(QStringLiteral("mEnableDirectMessages"));
    mMainLayout->addWidget(mEnableDirectMessages);
    connectCheckBox(mEnableDirectMessages, QStringLiteral("WebRTC_Enable_Direct"));

    mServer->setObjectName(QStringLiteral("mServer"));
    mServer->setToolTip(
        i18n("A list of STUN and TURN servers separated by comma.\nUsername, password and port are allowed in the format username:password@stun:host:port or "
             "username:password@turn:host:port."));
    addLineEdit(i18n("STUN/TURN Servers"), mServer, QStringLiteral("WebRTC_Servers"));
}

WebRtcSettingsWidget::~WebRtcSettingsWidget() = default;

void WebRtcSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnabled, mapSettings, false);
    initializeWidget(mEnablePublicChannels, mapSettings, false);
    initializeWidget(mEnablePrivateChannels, mapSettings, false);
    initializeWidget(mEnableDirectMessages, mapSettings, false);
    initializeWidget(mServer, mapSettings, QString());
}

#include "moc_webrtcsettingswidget.cpp"
