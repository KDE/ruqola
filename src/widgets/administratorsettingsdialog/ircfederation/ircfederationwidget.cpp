/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "ircfederationwidget.h"

#include <KLocalizedString>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>

IrcFederationWidget::IrcFederationWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnabled(new QCheckBox(i18n("Enabled"), this))
    , mProtocol(new QComboBox(this))
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
}

IrcFederationWidget::~IrcFederationWidget() = default;

void IrcFederationWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnabled, mapSettings);
}
