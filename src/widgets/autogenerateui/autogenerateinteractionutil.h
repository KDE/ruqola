/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QJsonObject>
// packages/ui-kit/src/interactions/UserInteraction.ts
namespace AutoGenerateInteractionUtil
{
struct ViewBlockActionUserInfo {
    QByteArray actionId;
    QByteArray blockIdPayload;
    QString valuePayload;
    QByteArray idContainer;
    QByteArray triggerId;
};

[[nodiscard]] QJsonObject createViewBlockActionUser(const ViewBlockActionUserInfo &info);
[[nodiscard]] QJsonObject createViewClosedUser(const ViewBlockActionUserInfo &info);
[[nodiscard]] QJsonObject createViewSubmitUser(const ViewBlockActionUserInfo &info);
};
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUtil::ViewBlockActionUserInfo, Q_RELOCATABLE_TYPE);
