/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_export.h"
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
[[nodiscard]] QJsonObject createViewClosedUser(const QJsonObject &payload, const QString &triggerId);
struct ViewSubmitUserInfo {
    QByteArray actionId;
    QByteArray viewId;
    QString triggerId;
    QJsonObject payload;
};

[[nodiscard]] QJsonObject createViewSubmitUser(const ViewSubmitUserInfo &info);

struct ActionButtonInfo {
    QByteArray actionId;
    QByteArray roomId;
    QByteArray triggerId;
};
[[nodiscard]] QJsonObject createRoomActionButton(const ActionButtonInfo &info);

struct ActionMessageInfo {
    QByteArray actionId;
    QByteArray roomId;
    QByteArray messageId;
    QByteArray triggerId;
};
[[nodiscard]] QJsonObject createMessageActionButton(const ActionMessageInfo &info);

struct MessageBlockMessageActionUserInfo {
    QByteArray actionId;
    QByteArray roomId;
    QByteArray messageId;
    QByteArray triggerId;
    QByteArray threadId;
    QString value;
    QString blockId;
};
[[nodiscard]] QJsonObject createBlockMessageActionUser(const MessageBlockMessageActionUserInfo &info);
[[nodiscard]] QJsonObject createMessageBoxActionButton(const ActionButtonInfo &info);
};
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUtil::ViewBlockActionUserInfo, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUtil::ViewSubmitUserInfo, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUtil::ActionButtonInfo, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUtil::ActionMessageInfo, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUtil::MessageBlockMessageActionUserInfo, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUtil::ViewBlockActionUserInfo &t);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUtil::ActionButtonInfo &t);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUtil::ActionMessageInfo &t);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUtil::ViewSubmitUserInfo &t);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUtil::MessageBlockMessageActionUserInfo &t);
