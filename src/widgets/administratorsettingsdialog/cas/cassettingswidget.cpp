/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "cassettingswidget.h"
using namespace Qt::Literals::StringLiterals;

#include "rocketchataccount.h"

#include <KLocalizedString>

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

CasSettingsWidget::CasSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
    , mEnabled(new QCheckBox(i18nc("@option:check", "Enabled"), this))
    , mSSOBaseURL(new QLineEdit(this))
    , mSSOLoginURL(new QLineEdit(this))
    , mCASVersion(new QComboBox(this))
    , mTrustCASUsername(new QCheckBox(i18nc("@option:check", "Trust CAS Username"), this))
    , mAllowUserCreation(new QCheckBox(i18nc("@option:check", "Allow user creation"), this))
    , mAlwaysSyncUserData(new QCheckBox(i18nc("@option:check", "Always Sync User Data"), this))
    , mAttributeMap(new QLineEdit(this))
{
    mEnabled->setObjectName(u"mEnabled"_s);
    addCheckBox(mEnabled, u"CAS_enabled"_s);

    mSSOBaseURL->setObjectName(u"mSSOBaseURL"_s);
    addLineEdit(i18n("SSO Base URL"), mSSOBaseURL, u"CAS_base_url"_s);
    mSSOBaseURL->setToolTip(i18nc("@info:tooltip", "The base URL of your external SSO service e.g: https://sso.example.undef/sso/"));

    mSSOLoginURL->setObjectName(u"mSSOLoginURL"_s);
    addLineEdit(i18n("SSO Login URL"), mSSOLoginURL, u"CAS_login_url"_s);
    mSSOLoginURL->setToolTip(i18nc("@info:tooltip", "The login URL of your external SSO service e.g: https://sso.example.undef/sso/login"));

    mCASVersion->setObjectName(u"mCASVersion"_s);
    const QMap<QString, QString> maps = {
        {u"1.0"_s, u"1.0"_s},
        {u"2.0"_s, u"2.0"_s},
    };
    addComboBox(i18n("Protocol"), maps, mCASVersion, u"CAS_version"_s);

    mTrustCASUsername->setObjectName(u"mTrustCASUsername"_s);
    mTrustCASUsername->setToolTip(
        i18n("When enabled, Rocket.Chat will trust that any username from CAS belongs to the same user on Rocket.Chat.\nThis may be needed if a user is "
             "renamed on CAS, but may also allow people to take control of Rocket.Chat accounts by renaming their own CAS users."));
    addCheckBox(mTrustCASUsername, u"CAS_trust_username"_s);

    mAllowUserCreation->setObjectName(u"mAllowUserCreation"_s);
    mAllowUserCreation->setToolTip(i18nc("@info:tooltip", "Allow CAS User creation from data provided by the CAS ticket."));
    addCheckBox(mAllowUserCreation, u"CAS_Creation_User_Enabled"_s);

    auto attributeHandlingLabel = createBoldLabel(i18n("Attribute handling"));
    attributeHandlingLabel->setObjectName(u"attributeHandlingLabel"_s);
    mMainLayout->addWidget(attributeHandlingLabel);

    mAlwaysSyncUserData->setObjectName(u"mAlwaysSyncUserData"_s);
    mAlwaysSyncUserData->setToolTip(i18nc(
        "@info:tooltip",
        "Always synchronize external CAS User data into available attributes upon login.\nNote: Attributes are always synced upon account creation anyway."));
    addCheckBox(mAlwaysSyncUserData, u"CAS_Sync_User_Data_Enabled"_s);

    mAttributeMap->setObjectName(u"mAttributeMap"_s);
    addLineEdit(i18n("Attribute Map"), mAttributeMap, u"CAS_Sync_User_Data_FieldMap"_s);
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

void CasSettingsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mEnabled, mapSettings, false);
    initializeWidget(mTrustCASUsername, mapSettings, false);
    initializeWidget(mAllowUserCreation, mapSettings, true);
    initializeWidget(mSSOBaseURL, mapSettings, QString());
    initializeWidget(mSSOLoginURL, mapSettings, QString());
    initializeWidget(mCASVersion, mapSettings, u"1.0"_s);
    initializeWidget(mAlwaysSyncUserData, mapSettings, true);
    initializeWidget(mAttributeMap, mapSettings, u"{}"_s);
}

#include "moc_cassettingswidget.cpp"
