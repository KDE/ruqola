/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>
class LIBRUQOLACORE_EXPORT AppsCountInfo
{
public:
    AppsCountInfo();
    ~AppsCountInfo();
    void parseCountInfo(const QJsonObject &replyObject);

    [[nodiscard]] int maxMarketplaceApps() const;
    void setMaxMarketplaceApps(int newMaxMarketplaceApps);

    [[nodiscard]] int maxPrivateApps() const;
    void setMaxPrivateApps(int newMaxPrivateApps);

    [[nodiscard]] int totalMarketplaceEnabled() const;
    void setTotalMarketplaceEnabled(int newTotalMarketplaceEnabled);

    [[nodiscard]] int totalPrivateEnabled() const;
    void setTotalPrivateEnabled(int newTotalPrivateEnabled);

    [[nodiscard]] bool operator==(const AppsCountInfo &other) const;

    [[nodiscard]] bool isValid() const;

private:
    int mMaxMarketplaceApps = -1;
    int mMaxPrivateApps = -1;
    int mTotalMarketplaceEnabled = -1;
    int mTotalPrivateEnabled = -1;
};

Q_DECLARE_METATYPE(AppsCountInfo)
Q_DECLARE_TYPEINFO(AppsCountInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AppsCountInfo &t);
