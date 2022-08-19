/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "ircfederationwidget.h"

#include <KLocalizedString>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>

IrcFederationWidget::IrcFederationWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnabled(new QCheckBox(i18n("Enabled"), this))
    , mProtocol(new QComboBox(this))
    , mHost(new QLineEdit(this))
    , mPort(new QSpinBox(this))
    , mName(new QLineEdit(this))
    , mDescription(new QLineEdit(this))
    , mLocalPassword(new QLineEdit(this))
    , mPeerPassword(new QLineEdit(this))

{
    mEnabled->setObjectName(QStringLiteral("mEnabled"));
    mEnabled->setToolTip(i18n("Attempt to integrate IRC support. Changing this value requires restarting Rocket.Chat."));
    mMainLayout->addWidget(mEnabled);
    connectCheckBox(mEnabled, QStringLiteral("IRC_Enabled"));

    mProtocol->setObjectName(QStringLiteral("mProtocol"));
    QMap<QString, QString> maps = {
        {QStringLiteral("RFC2813"), i18n("RFC2813")},
    };
    addComboBox(i18n("Protocol"), maps, mProtocol, QStringLiteral("IRC_Protocol"));

    mHost->setObjectName(QStringLiteral("mHost"));
    addLineEdit(i18n("Host"), mHost, QStringLiteral("IRC_Host"));

    mPort->setObjectName(QStringLiteral("mPort"));
    mPort->setToolTip(i18n("The port to bind to on the IRC host server."));
    mPort->setMaximum(99999);
    addSpinbox(i18n("Port"), mPort, QStringLiteral("IRC_Port"));

    mName->setObjectName(QStringLiteral("mName"));
    addLineEdit(i18n("Name"), mName, QStringLiteral("IRC_Name"));

    mDescription->setObjectName(QStringLiteral("mDescription"));
    addLineEdit(i18n("Description"), mDescription, QStringLiteral("IRC_Description"));

    mLocalPassword->setObjectName(QStringLiteral("mLocalPassword"));
    addLineEdit(i18n("Local Password"), mLocalPassword, QStringLiteral("IRC_Local_Password"));

    mPeerPassword->setObjectName(QStringLiteral("mPeerPassword"));
    addLineEdit(i18n("Peer Password"), mPeerPassword, QStringLiteral("IRC_Peer_Password"));
}

IrcFederationWidget::~IrcFederationWidget() = default;

void IrcFederationWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnabled, mapSettings);
    initializeWidget(mProtocol, mapSettings);
    initializeWidget(mHost, mapSettings);
    initializeWidget(mPort, mapSettings);
    initializeWidget(mName, mapSettings);
    initializeWidget(mDescription, mapSettings);
    initializeWidget(mLocalPassword, mapSettings);
    initializeWidget(mPeerPassword, mapSettings);
}

void IrcFederationWidget::initializeDefaultValue()
{
    mEnabled->setChecked(false);
    mHost->setText(QStringLiteral("localhost"));
    mPort->setValue(6667);
    mName->setText(QStringLiteral("irc.rocket.chat"));
    mDescription->setText(QStringLiteral("Rocket.Chat IRC Bridge"));
    mLocalPassword->setText(QStringLiteral("password"));
    mPeerPassword->setText(QStringLiteral("password"));
}
