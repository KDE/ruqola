/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QStringList>

class QJsonObject;
class LIBRUQOLACORE_EXPORT LicensesManager
{
public:
    enum class ActiveModule : uint8_t {
        Auditing,
        DeviceManagement,
        MessageReadReceipt,
        /*

                "auditing",
                "canned-responses",
                "ldap-enterprise",
                "livechat-enterprise",
                "voip-enterprise",
                "omnichannel-mobile-enterprise",
                "engagement-dashboard",
                "push-privacy",
                "scalability",
                "teams-mention",
                "saml-enterprise",
                "oauth-enterprise",
                "device-management",
                "federation",
                "videoconference-enterprise",
                "message-read-receipt",
                "outlook-calendar",
                "hide-watermark",
                "custom-roles",
                "accessibility-certification",
                "abac"
          */
    };

    LicensesManager();
    ~LicensesManager();

    [[nodiscard]] const QStringList &licenses() const;
    void setLicenses(const QStringList &newLicenses);

    [[nodiscard]] bool hasLicense(const QString &name);

    void parseLicenses(const QJsonObject &root);

private:
    QStringList mLicenses;
};
