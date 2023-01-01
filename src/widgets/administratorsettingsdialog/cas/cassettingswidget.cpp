/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "cassettingswidget.h"
#include "rocketchataccount.h"

#include <KLocalizedString>

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

CasSettingsWidget::CasSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
    , mEnabled(new QCheckBox(i18n("Enabled"), this))
    , mTrustCASUsername(new QCheckBox(i18n("Trust CAS Username"), this))
    , mAllowUserCreation(new QCheckBox(i18n("Allow user creation"), this))
    , mSSOBaseURL(new QLineEdit(this))
    , mSSOLoginURL(new QLineEdit(this))
    , mCASVersion(new QComboBox(this))
    , mAlwaysSyncUserData(new QCheckBox(i18n("Always Sync User Data"), this))
    , mAttributeMap(new QLineEdit(this))
{
    mEnabled->setObjectName(QStringLiteral("mEnabled"));
    mMainLayout->addWidget(mEnabled);
    connectCheckBox(mEnabled, QStringLiteral("CAS_enabled"));

    mSSOBaseURL->setObjectName(QStringLiteral("mSSOBaseURL"));
    addLineEdit(i18n("SSO Base URL"), mSSOBaseURL, QStringLiteral("CAS_base_url"));
    mSSOBaseURL->setToolTip(i18n("The base URL of your external SSO service e.g: https://sso.example.undef/sso/"));

    mSSOLoginURL->setObjectName(QStringLiteral("mSSOLoginURL"));
    addLineEdit(i18n("SSO Login URL"), mSSOLoginURL, QStringLiteral("CAS_login_url"));
    mSSOLoginURL->setToolTip(i18n("The login URL of your external SSO service e.g: https://sso.example.undef/sso/login"));

    mCASVersion->setObjectName(QStringLiteral("mCASVersion"));
    const QMap<QString, QString> maps = {
        {QStringLiteral("1.0"), QStringLiteral("1.0")},
        {QStringLiteral("2.0"), QStringLiteral("2.0")},
    };
    addComboBox(i18n("Protocol"), maps, mCASVersion, QStringLiteral("CAS_version"));

    mTrustCASUsername->setObjectName(QStringLiteral("mTrustCASUsername"));
    mMainLayout->addWidget(mTrustCASUsername);
    mTrustCASUsername->setToolTip(
        i18n("When enabled, Rocket.Chat will trust that any username from CAS belongs to the same user on Rocket.Chat.\nThis may be needed if a user is "
             "renamed on CAS, but may also allow people to take control of Rocket.Chat accounts by renaming their own CAS users."));
    connectCheckBox(mTrustCASUsername, QStringLiteral("CAS_trust_username"));

    mAllowUserCreation->setObjectName(QStringLiteral("mAllowUserCreation"));
    mAllowUserCreation->setToolTip(i18n("Allow CAS User creation from data provided by the CAS ticket."));
    mMainLayout->addWidget(mAllowUserCreation);
    connectCheckBox(mAllowUserCreation, QStringLiteral("CAS_Creation_User_Enabled"));

    auto attributeHandlingLabel = createBoldLabel(i18n("Attribute handling"));
    attributeHandlingLabel->setObjectName(QStringLiteral("attributeHandlingLabel"));
    mMainLayout->addWidget(attributeHandlingLabel);

    mAlwaysSyncUserData->setObjectName(QStringLiteral("mAlwaysSyncUserData"));
    mAlwaysSyncUserData->setToolTip(i18n(
        "Always synchronize external CAS User data into available attributes upon login.\nNote: Attributes are always synced upon account creation anyway."));
    mMainLayout->addWidget(mAlwaysSyncUserData);
    connectCheckBox(mAlwaysSyncUserData, QStringLiteral("CAS_Sync_User_Data_Enabled"));

    mAttributeMap->setObjectName(QStringLiteral("mAttributeMap"));
    addLineEdit(i18n("Attribute Map"), mAttributeMap, QStringLiteral("CAS_Sync_User_Data_FieldMap"));
    mAttributeMap->setToolTip(
        i18n("Use this JSON input to build internal attributes (key) from external attributes (value).\n"
             "External attribute names enclosed with '%' will interpolated in value strings.<br/>Example, {\"email\":\"%email%\", \"name\":\"%firstname%, "
             "%lastname%\"}"
             "\nThe attribute map is always interpolated. In CAS 1.0 only the username attribute is available. Available internal attributes are: username, "
             "name, email, rooms;\n"
             "rooms is a comma separated list of rooms to join upon user creation e.g: {\"rooms\": \"%team%,%department%\"} "
             "would join CAS users on creation to their team and department channel."));
}

CasSettingsWidget::~CasSettingsWidget() = default;

void CasSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnabled, mapSettings, false);
    initializeWidget(mTrustCASUsername, mapSettings, false);
    initializeWidget(mAllowUserCreation, mapSettings, true);
    initializeWidget(mSSOBaseURL, mapSettings, QString());
    initializeWidget(mSSOLoginURL, mapSettings, QString());
    initializeWidget(mCASVersion, mapSettings, QStringLiteral("1.0"));
    initializeWidget(mAlwaysSyncUserData, mapSettings, true);
    initializeWidget(mAttributeMap, mapSettings, QStringLiteral("{}"));
}
