/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "licensesmanager.h"
#include "ruqola_license_debug.h"
#include <QJsonArray>

#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;

LicensesManager::LicensesManager() = default;

LicensesManager::~LicensesManager() = default;

bool LicensesManager::hasLicense(LicensesManager::ActiveModule activeModule) const
{
    return mActiveModules.contains(activeModule);
}

void LicensesManager::parseLicenses(const QJsonObject &root)
{
    const QJsonObject license = root["license"_L1].toObject();
    const QJsonArray replyArray = license["activeModules"_L1].toArray();
    mActiveModules.clear();
    for (int i = 0, total = replyArray.count(); i < total; ++i) {
        mActiveModules.append(convertStringToActiveModule(replyArray.at(i).toString()));
    }
}

LicensesManager::ActiveModule LicensesManager::convertStringToActiveModule(const QString &str)
{
    if (str == "auditing"_L1) {
        return LicensesManager::ActiveModule::Auditing;
    } else if (str == "canned-responses"_L1) {
        return LicensesManager::ActiveModule::CannedResponses;
    } else if (str == "ldap-enterprise"_L1) {
        return LicensesManager::ActiveModule::LdapEnterprise;
    } else if (str == "livechat-enterprise"_L1) {
        return LicensesManager::ActiveModule::LivechatEnterprise;
    } else if (str == "voip-enterprise"_L1) {
        return LicensesManager::ActiveModule::VoipEnterprise;
    } else if (str == "omnichannel-mobile-enterprise"_L1) {
        return LicensesManager::ActiveModule::OmnichannelMobileEnterprise;
    } else if (str == "engagement-dashboard"_L1) {
        return LicensesManager::ActiveModule::EngagementDashboard;
    } else if (str == "push-privacy"_L1) {
        return LicensesManager::ActiveModule::PushPrivacy;
    } else if (str == "scalability"_L1) {
        return LicensesManager::ActiveModule::Scalability;
    } else if (str == "teams-mention"_L1) {
        return LicensesManager::ActiveModule::TeamsMention;
    } else if (str == "saml-enterprise"_L1) {
        return LicensesManager::ActiveModule::SamlEnterprise;
    } else if (str == "oauth-enterprise"_L1) {
        return LicensesManager::ActiveModule::OauthEnterprise;
    } else if (str == "device-management"_L1) {
        return LicensesManager::ActiveModule::DeviceManagement;
    } else if (str == "federation"_L1) {
        return LicensesManager::ActiveModule::Federation;
    } else if (str == "videoconference-enterprise"_L1) {
        return LicensesManager::ActiveModule::VideoconferenceEnterprise;
    } else if (str == "message-read-receipt"_L1) {
        return LicensesManager::ActiveModule::MessageReadReceipt;
    } else if (str == "outlook-calendar"_L1) {
        return LicensesManager::ActiveModule::OutlookCalendar;
    } else if (str == "hide-watermark"_L1) {
        return LicensesManager::ActiveModule::HideWatermark;
    } else if (str == "custom-roles"_L1) {
        return LicensesManager::ActiveModule::CustomRoles;
    } else if (str == "accessibility-certification"_L1) {
        return LicensesManager::ActiveModule::AccessibilityCertification;
    } else if (str == "abac"_L1) {
        return LicensesManager::ActiveModule::Abac;
    } else if (str == "teams-voip"_L1) {
        return LicensesManager::ActiveModule::TeamsVoip;
    } else if (str == "contact-id-verification"_L1) {
        return LicensesManager::ActiveModule::ContactIdVerification;
    } else if (str == "chat.rocket.rc-ai"_L1) {
        return LicensesManager::ActiveModule::RCAi;
    } else if (str == "chat.rocket.360-dialog"_L1) {
        return LicensesManager::ActiveModule::RC360Dialog;
    } else if (str == "outbound-messaging"_L1) {
        return LicensesManager::ActiveModule::OutboundMessaging;
    } else {
        qCWarning(RUQOLA_LICENSE_LOG) << "Unknown license module type " << str;
        return LicensesManager::ActiveModule::Unknown;
    }
}

QString LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule module)
{
    switch (module) {
    case LicensesManager::ActiveModule::Auditing:
        return u"auditing"_s;
    case LicensesManager::ActiveModule::CannedResponses:
        return u"canned-responses"_s;
    case LicensesManager::ActiveModule::LdapEnterprise:
        return u"ldap-enterprise"_s;
    case LicensesManager::ActiveModule::LivechatEnterprise:
        return u"livechat-enterprise"_s;
    case LicensesManager::ActiveModule::VoipEnterprise:
        return u"voip-enterprise"_s;
    case LicensesManager::ActiveModule::OmnichannelMobileEnterprise:
        return u"omnichannel-mobile-enterprise"_s;
    case LicensesManager::ActiveModule::EngagementDashboard:
        return u"engagement-dashboard"_s;
    case LicensesManager::ActiveModule::PushPrivacy:
        return u"push-privacy"_s;
    case LicensesManager::ActiveModule::Scalability:
        return u"scalability"_s;
    case LicensesManager::ActiveModule::TeamsMention:
        return u"teams-mention"_s;
    case LicensesManager::ActiveModule::SamlEnterprise:
        return u"saml-enterprise"_s;
    case LicensesManager::ActiveModule::OauthEnterprise:
        return u"oauth-enterprise"_s;
    case LicensesManager::ActiveModule::DeviceManagement:
        return u"device-management"_s;
    case LicensesManager::ActiveModule::Federation:
        return u"federation"_s;
    case LicensesManager::ActiveModule::VideoconferenceEnterprise:
        return u"videoconference-enterprise"_s;
    case LicensesManager::ActiveModule::MessageReadReceipt:
        return u"message-read-receipt"_s;
    case LicensesManager::ActiveModule::OutlookCalendar:
        return u"outlook-calendar"_s;
    case LicensesManager::ActiveModule::HideWatermark:
        return u"hide-watermark"_s;
    case LicensesManager::ActiveModule::CustomRoles:
        return u"custom-roles"_s;
    case LicensesManager::ActiveModule::AccessibilityCertification:
        return u"accessibility-certification"_s;
    case LicensesManager::ActiveModule::Abac:
        return u"abac"_s;
    case LicensesManager::ActiveModule::TeamsVoip:
        return u"teams-voip"_s;
    case LicensesManager::ActiveModule::ContactIdVerification:
        return u"contact-id-verification"_s;
    case LicensesManager::ActiveModule::RCAi:
        return u"chat.rocket.rc-ai"_s;
    case LicensesManager::ActiveModule::RC360Dialog:
        return u"chat.rocket.360-dialog"_s;
    case LicensesManager::ActiveModule::OutboundMessaging:
        return u"outbound-messaging"_s;
    case LicensesManager::ActiveModule::Unknown:
    default:
        qCWarning(RUQOLA_LICENSE_LOG) << "Unknown license module type";
        return QString();
    }
}

QList<LicensesManager::ActiveModule> LicensesManager::activeModules() const
{
    return mActiveModules;
}

void LicensesManager::setActiveModules(const QList<ActiveModule> &newActiveModules)
{
    mActiveModules = newActiveModules;
}

#include "moc_licensesmanager.cpp"
