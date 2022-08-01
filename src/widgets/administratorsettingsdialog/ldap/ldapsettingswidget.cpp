/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ldapsettingswidget.h"

#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>

LDapSettingsWidget::LDapSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnabled(new QCheckBox(i18n("LDAP Enabled"), this))
    , mHost(new QLineEdit(this))
    , mPort(new QSpinBox(this))
    , mReconnect(new QCheckBox(i18n("Reconnect"), this))
{
    mEnabled->setObjectName(QStringLiteral("mEnabled"));
    mEnabled->setToolTip(i18n("Attempt to utilize LDAP for authentication."));
    mMainLayout->addWidget(mEnabled);
    connectCheckBox(mEnabled, QStringLiteral("LDAP_Enable"));

    mPort->setObjectName(QStringLiteral("mPort"));
    mPort->setMaximum(9999);
    mPort->setToolTip(i18n("Port to access LDAP. eg: 389 or 636 for LDAPS"));
    addSpinbox(i18n("Port"), mPort, QStringLiteral("LDAP_Port"));

    mHost->setObjectName(QStringLiteral("mHost"));
    addLineEdit(i18n("Host"), mHost, QStringLiteral("LDAP_Host"));

    mReconnect->setObjectName(QStringLiteral("mReconnect"));
    mReconnect->setToolTip(i18n("Try to reconnect automatically when connection is interrupted by some reason while executing operations."));
    mMainLayout->addWidget(mReconnect);
    connectCheckBox(mReconnect, QStringLiteral("LDAP_Reconnect"));
}

LDapSettingsWidget::~LDapSettingsWidget() = default;

void LDapSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnabled, mapSettings);
    initializeWidget(mHost, mapSettings);
    initializeWidget(mPort, mapSettings);
    initializeWidget(mReconnect, mapSettings);
}
