/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "ircfederationwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>

IrcFederationWidget::IrcFederationWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnabled(new QCheckBox(i18nc("@option:check", "Enabled"), this))
    , mProtocol(new QComboBox(this))
    , mHost(new QLineEdit(this))
    , mPort(new QSpinBox(this))
    , mName(new QLineEdit(this))
    , mDescription(new QLineEdit(this))
    , mLocalPassword(new QLineEdit(this))
    , mPeerPassword(new QLineEdit(this))

{
    mEnabled->setObjectName(u"mEnabled"_s);
    mEnabled->setToolTip(i18nc("@info:tooltip", "Attempt to integrate IRC support. Changing this value requires restarting Rocket.Chat."));
    addCheckBox(mEnabled, u"IRC_Enabled"_s);

    mProtocol->setObjectName(u"mProtocol"_s);
    const QMap<QString, QString> maps = {
        {u"RFC2813"_s, i18n("RFC2813")},
    };
    addComboBox(i18n("Protocol"), maps, mProtocol, u"IRC_Protocol"_s);

    mHost->setObjectName(u"mHost"_s);
    addLineEdit(i18n("Host"), mHost, u"IRC_Host"_s);

    mPort->setObjectName(u"mPort"_s);
    mPort->setToolTip(i18nc("@info:tooltip", "The port to bind to on the IRC host server."));
    mPort->setMaximum(99999);
    addSpinbox(i18n("Port"), mPort, u"IRC_Port"_s);

    mName->setObjectName(u"mName"_s);
    addLineEdit(i18n("Name"), mName, u"IRC_Name"_s);

    mDescription->setObjectName(u"mDescription"_s);
    addLineEdit(i18n("Description"), mDescription, u"IRC_Description"_s);

    mLocalPassword->setObjectName(u"mLocalPassword"_s);
    addLineEdit(i18n("Local Password"), mLocalPassword, u"IRC_Local_Password"_s);

    mPeerPassword->setObjectName(u"mPeerPassword"_s);
    addLineEdit(i18n("Peer Password"), mPeerPassword, u"IRC_Peer_Password"_s);
}

IrcFederationWidget::~IrcFederationWidget() = default;

void IrcFederationWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mEnabled, mapSettings, false);
    initializeWidget(mProtocol, mapSettings, u"RFC2813"_s);
    initializeWidget(mHost, mapSettings, u"localhost"_s);
    initializeWidget(mPort, mapSettings, 6667);
    initializeWidget(mName, mapSettings, u"irc.rocket.chat"_s);
    initializeWidget(mDescription, mapSettings, u"Rocket.Chat IRC Bridge"_s);
    initializeWidget(mLocalPassword, mapSettings, u"password"_s);
    initializeWidget(mPeerPassword, mapSettings, u"password"_s);
}

#include "moc_ircfederationwidget.cpp"
