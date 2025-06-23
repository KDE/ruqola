/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ldapsettingswidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>

LDapSettingsWidget::LDapSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnabled(new QCheckBox(i18nc("@option:check", "LDAP Enabled"), this))
    , mPort(new QSpinBox(this))
    , mHost(new QLineEdit(this))
    , mReconnect(new QCheckBox(i18nc("@option:check", "Reconnect"), this))
    , mAuthenticationEnabled(new QCheckBox(i18nc("@option:check", "Enabled"), this))
    , mAuthenticationUserDN(new QLineEdit(this))
    , mAuthenticationPassword(new KPasswordLineEdit(this))
    , mTimeouts(new QSpinBox(this))
    , mConnectionTimeouts(new QSpinBox(this))
    , mIdleTimeouts(new QSpinBox(this))
{
    mEnabled->setObjectName(u"mEnabled"_s);
    mEnabled->setToolTip(i18nc("@info:tooltip", "Attempt to utilize LDAP for authentication."));
    addCheckBox(mEnabled, u"LDAP_Enable"_s);

    mPort->setObjectName(u"mPort"_s);
    mPort->setMaximum(9999);
    mPort->setToolTip(i18nc("@info:tooltip", "Port to access LDAP. eg: 389 or 636 for LDAPS"));
    addSpinbox(i18n("Port"), mPort, u"LDAP_Port"_s);

    mHost->setObjectName(u"mHost"_s);
    addLineEdit(i18n("Host"), mHost, u"LDAP_Host"_s);

    mReconnect->setObjectName(u"mReconnect"_s);
    mReconnect->setToolTip(i18nc("@info:tooltip", "Try to reconnect automatically when connection is interrupted by some reason while executing operations."));
    addCheckBox(mReconnect, u"LDAP_Reconnect"_s);

    auto authenticationLabel = createBoldLabel(i18n("Authentication"));
    authenticationLabel->setObjectName(u"authenticationLabel"_s);
    mMainLayout->addWidget(authenticationLabel);

    mAuthenticationEnabled->setObjectName(u"mAuthenticationEnabled"_s);
    addCheckBox(mAuthenticationEnabled, u"LDAP_Authentication"_s);

    mAuthenticationUserDN->setObjectName(u"mAuthenticationUserDN"_s);
    addLineEdit(i18n("User DN"), mAuthenticationUserDN, u"LDAP_Authentication_UserDN"_s);
    mAuthenticationUserDN->setToolTip(
        i18n("The LDAP user that performs user lookups to authenticate other users when they sign in. This is typically a service account created specifically "
             "for third-party integrations. Use a fully qualified name, such as cn=Administrator,cn=Users,dc=Example,dc=com."));

    mAuthenticationPassword->setObjectName(u"mAuthenticationPassword"_s);
    addPasswordEdit(i18n("Password"), mAuthenticationPassword, u"LDAP_Authentication_Password"_s);

    // Add password

    auto timeoutLabel = createBoldLabel(i18n("Timeouts"));
    timeoutLabel->setObjectName(u"timeoutLabel"_s);
    mMainLayout->addWidget(timeoutLabel);

    // Add timeouts
    mTimeouts->setObjectName(u"mTimeouts"_s);
    mTimeouts->setMaximum(99999);
    mTimeouts->setToolTip(i18nc("@info:tooltip", "How many milliseconds wait for a search result before return an error"));
    addSpinbox(i18n("Timeout (ms)"), mTimeouts, u"LDAP_Timeout"_s);

    mConnectionTimeouts->setObjectName(u"mConnectionTimeouts"_s);
    mConnectionTimeouts->setMaximum(99999);
    mConnectionTimeouts->setToolTip(i18nc("@info:tooltip", "How many milliseconds wait for a search result before return an error"));
    addSpinbox(i18n("Connection Timeout (ms)"), mConnectionTimeouts, u"LDAP_Connect_Timeout"_s);

    mIdleTimeouts->setObjectName(u"mIdleTimeouts"_s);
    mIdleTimeouts->setMaximum(99999);
    mIdleTimeouts->setToolTip(
        i18n("How many milliseconds wait after the latest LDAP operation until close the connection. (Each operation will open a new connection)"));
    addSpinbox(i18n("Idle Timeout (ms)"), mIdleTimeouts, u"LDAP_Idle_Timeout"_s);
}

LDapSettingsWidget::~LDapSettingsWidget() = default;

void LDapSettingsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mEnabled, mapSettings, true);
    initializeWidget(mHost, mapSettings);
    initializeWidget(mPort, mapSettings, 389);
    initializeWidget(mReconnect, mapSettings, false);
    initializeWidget(mAuthenticationEnabled, mapSettings);
    initializeWidget(mAuthenticationUserDN, mapSettings);
    initializeWidget(mTimeouts, mapSettings, 60000);
    initializeWidget(mConnectionTimeouts, mapSettings, 1000);
    initializeWidget(mIdleTimeouts, mapSettings, 1000);
    initializeWidget(mAuthenticationPassword, mapSettings);
}

#include "moc_ldapsettingswidget.cpp"
