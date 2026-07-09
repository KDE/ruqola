/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QMetaType>
#include <QStringList>
class QJsonObject;
class LIBRUQOLACORE_EXPORT LicensesManager
{
    Q_GADGET
public:
    enum class ActiveModule : uint8_t {
        Unknown = 0,
        Auditing,
        DeviceManagement,
        MessageReadReceipt,
        CannedResponses,
        LdapEnterprise,
        LivechatEnterprise,
        VoipEnterprise,
        OmnichannelMobileEnterprise,
        EngagementDashboard,
        PushPrivacy,
        Scalability,
        TeamsMention,
        SamlEnterprise,
        OauthEnterprise,
        Federation,
        VideoconferenceEnterprise,
        OutlookCalendar,
        HideWatermark,
        CustomRoles,
        AccessibilityCertification,
        Abac
    };
    Q_ENUM(ActiveModule)

    LicensesManager();
    ~LicensesManager();

    [[nodiscard]] bool hasLicense(LicensesManager::ActiveModule activeModule) const;

    void parseLicenses(const QJsonObject &root);

    [[nodiscard]] static LicensesManager::ActiveModule convertStringToActiveModule(const QString &str);
    [[nodiscard]] static QString convertActiveModuleToString(LicensesManager::ActiveModule module);

    [[nodiscard]] QList<ActiveModule> activeModules() const;
    void setActiveModules(const QList<ActiveModule> &newActiveModules);

private:
    QList<ActiveModule> mActiveModules;
};
