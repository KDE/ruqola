/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "webrtcsettingswidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>

WebRtcSettingsWidget::WebRtcSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnabled(new QCheckBox(i18nc("@option:check", "Enabled"), this))
    , mEnablePublicChannels(new QCheckBox(i18nc("@option:check", "Enable for Public Channels"), this))
    , mEnablePrivateChannels(new QCheckBox(i18nc("@option:check", "Enable for Private Channels"), this))
    , mEnableDirectMessages(new QCheckBox(i18nc("@option:check", "Enable for Direct Messages"), this))
    , mServer(new QLineEdit(this))
{
    mEnabled->setObjectName(u"mEnabled"_s);
    addCheckBox(mEnabled, u"WebRTC_Enabled"_s);

    mEnablePublicChannels->setObjectName(u"mEnablePublicChannels"_s);
    addCheckBox(mEnablePublicChannels, u"WebRTC_Enable_Channel"_s);

    mEnablePrivateChannels->setObjectName(u"mEnablePrivateChannels"_s);
    addCheckBox(mEnablePrivateChannels, u"WebRTC_Enable_Private"_s);

    mEnableDirectMessages->setObjectName(u"mEnableDirectMessages"_s);
    addCheckBox(mEnableDirectMessages, u"WebRTC_Enable_Direct"_s);

    mServer->setObjectName(u"mServer"_s);
    mServer->setToolTip(
        i18n("A list of STUN and TURN servers separated by comma.\nUsername, password and port are allowed in the format username:password@stun:host:port or "
             "username:password@turn:host:port."));
    addLineEdit(i18n("STUN/TURN Servers"), mServer, u"WebRTC_Servers"_s);
}

WebRtcSettingsWidget::~WebRtcSettingsWidget() = default;

void WebRtcSettingsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mEnabled, mapSettings, false);
    initializeWidget(mEnablePublicChannels, mapSettings, false);
    initializeWidget(mEnablePrivateChannels, mapSettings, false);
    initializeWidget(mEnableDirectMessages, mapSettings, false);
    initializeWidget(mServer, mapSettings, QString());
}

#include "moc_webrtcsettingswidget.cpp"
