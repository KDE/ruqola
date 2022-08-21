/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ldapsettingswidget.h"

#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>

LDapSettingsWidget::LDapSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnabled(new QCheckBox(i18n("LDAP Enabled"), this))
    , mHost(new QLineEdit(this))
    , mPort(new QSpinBox(this))
    , mReconnect(new QCheckBox(i18n("Reconnect"), this))
    , mAuthenticationEnabled(new QCheckBox(i18n("Enabled"), this))
    , mAuthenticationUserDN(new QLineEdit(this))
    , mTimeouts(new QSpinBox(this))
    , mConnectionTimeouts(new QSpinBox(this))
    , mIdleTimeouts(new QSpinBox(this))
    , mAuthenticationPassword(new KPasswordLineEdit(this))
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

    auto authenticationLabel = createBoldLabel(i18n("Authentication"));
    authenticationLabel->setObjectName(QStringLiteral("authenticationLabel"));
    mMainLayout->addWidget(authenticationLabel);

    mAuthenticationEnabled->setObjectName(QStringLiteral("mAuthenticationEnabled"));
    mMainLayout->addWidget(mAuthenticationEnabled);
    connectCheckBox(mAuthenticationEnabled, QStringLiteral("LDAP_Authentication"));

    mAuthenticationUserDN->setObjectName(QStringLiteral("mAuthenticationUserDN"));
    addLineEdit(i18n("User DN"), mAuthenticationUserDN, QStringLiteral("LDAP_Authentication_UserDN"));
    mAuthenticationUserDN->setToolTip(
        i18n("The LDAP user that performs user lookups to authenticate other users when they sign in. This is typically a service account created specifically "
             "for third-party integrations. Use a fully qualified name, such as cn=Administrator,cn=Users,dc=Example,dc=com."));

    mAuthenticationPassword->setObjectName(QStringLiteral("mAuthenticationPassword"));
    addPasswordEdit(i18n("Password"), mAuthenticationPassword, QStringLiteral("LDAP_Authentication_Password"));

    // Add password

    auto timeoutLabel = createBoldLabel(i18n("Timeouts"));
    timeoutLabel->setObjectName(QStringLiteral("timeoutLabel"));
    mMainLayout->addWidget(timeoutLabel);

    // Add timeouts
    mTimeouts->setObjectName(QStringLiteral("mTimeouts"));
    mTimeouts->setMaximum(99999);
    mTimeouts->setToolTip(i18n("How many milliseconds wait for a search result before return an error"));
    addSpinbox(i18n("Timeout (ms)"), mTimeouts, QStringLiteral("LDAP_Timeout"));

    mConnectionTimeouts->setObjectName(QStringLiteral("mConnectionTimeouts"));
    mConnectionTimeouts->setMaximum(99999);
    mConnectionTimeouts->setToolTip(i18n("How many milliseconds wait for a search result before return an error"));
    addSpinbox(i18n("Connection Timeout (ms)"), mConnectionTimeouts, QStringLiteral("LDAP_Connect_Timeout"));

    mIdleTimeouts->setObjectName(QStringLiteral("mIdleTimeouts"));
    mIdleTimeouts->setMaximum(99999);
    mIdleTimeouts->setToolTip(
        i18n("How many milliseconds wait after the latest LDAP operation until close the connection. (Each operation will open a new connection)"));
    addSpinbox(i18n("Idle Timeout (ms)"), mIdleTimeouts, QStringLiteral("LDAP_Idle_Timeout"));
}

LDapSettingsWidget::~LDapSettingsWidget() = default;

void LDapSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnabled, mapSettings);
    initializeWidget(mHost, mapSettings);
    initializeWidget(mPort, mapSettings);
    initializeWidget(mReconnect, mapSettings);
    initializeWidget(mAuthenticationEnabled, mapSettings);
    initializeWidget(mAuthenticationUserDN, mapSettings);
    initializeWidget(mTimeouts, mapSettings);
    initializeWidget(mConnectionTimeouts, mapSettings);
    initializeWidget(mIdleTimeouts, mapSettings);
    initializeWidget(mAuthenticationPassword, mapSettings);
}

void LDapSettingsWidget::initializeDefaultValue()
{
    mPort->setValue(389);
    mReconnect->setChecked(false);
    mConnectionTimeouts->setValue(1000);
    mIdleTimeouts->setValue(1000);
    mTimeouts->setValue(60000);
}
