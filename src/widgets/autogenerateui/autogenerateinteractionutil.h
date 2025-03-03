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
    QString actionId;
    QString blockIdPayload;
    QString valuePayload;
    QString idContainer;
    QString triggerId;
};

[[nodiscard]] QJsonObject createViewBlockActionUser(const ViewBlockActionUserInfo &info);
[[nodiscard]] QJsonObject createViewClosedUser(const ViewBlockActionUserInfo &info);
[[nodiscard]] QJsonObject createViewSubmitUser(const ViewBlockActionUserInfo &info);
};
