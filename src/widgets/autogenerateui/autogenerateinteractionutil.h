/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_export.h"
#include <QJsonObject>
#include <QList>
#include <algorithm>
class QDebug;
// packages/ui-kit/src/interactions/UserInteraction.ts
namespace AutoGenerateInteractionUtil
{
// Views, blocks and elements own their children as raw pointers. Comparing the
// pointers only tells whether both sides share the very same object, so compare
// what they point to instead.
template<typename T>
[[nodiscard]] inline bool isEqual(const T *lhs, const T *rhs)
{
    if (lhs == rhs) {
        return true;
    }
    return lhs && rhs && (*lhs == *rhs);
}

template<typename T>
[[nodiscard]] inline bool isEqual(const QList<T *> &lhs, const QList<T *> &rhs)
{
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend(), [](const T *a, const T *b) {
        return isEqual(a, b);
    });
}

// Same, for children stored as a base class pointer: their operator== would only
// compare the fields declared in that base class, the serialized form covers all
// of them.
template<typename T>
[[nodiscard]] inline bool isSerializedEqual(const T *lhs, const T *rhs)
{
    if (lhs == rhs) {
        return true;
    }
    return lhs && rhs && (lhs->serialize() == rhs->serialize());
}

template<typename T>
[[nodiscard]] inline bool isSerializedEqual(const QList<T *> &lhs, const QList<T *> &rhs)
{
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend(), [](const T *a, const T *b) {
        return isSerializedEqual(a, b);
    });
}

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
}
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUtil::ViewBlockActionUserInfo, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUtil::ViewSubmitUserInfo, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUtil::ActionButtonInfo, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUtil::ActionMessageInfo, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUtil::MessageBlockMessageActionUserInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUtil::ViewBlockActionUserInfo &t);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUtil::ActionButtonInfo &t);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUtil::ActionMessageInfo &t);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUtil::ViewSubmitUserInfo &t);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUtil::MessageBlockMessageActionUserInfo &t);
