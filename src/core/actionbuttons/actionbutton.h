/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
class LIBRUQOLACORE_EXPORT ActionButton
{
    Q_GADGET
public:
    // see ./packages/apps-engine/src/definition/ui/UIActionButtonContext.ts
    // ./packages/apps-engine/src/definition/ui/IUIActionButtonDescriptor.ts
    enum class RoomTypeFilter : uint16_t {
        Unknown = 0,
        PublicChannel = 1,
        PrivateChannel = 2,
        PublicTeam = 4,
        PrivateTeam = 8,
        PublicDiscussion = 16,
        PrivateDiscussion = 32,
        Direct = 64,
        DirectMultiple = 128,
        LiveChat = 256,
    };
    Q_FLAGS(RoomTypeFilter RoomTypeFilters)
    Q_DECLARE_FLAGS(RoomTypeFilters, RoomTypeFilter)

    enum class ButtonContext : uint8_t {
        Unknown,
        MessageAction,
        RoomAction,
        MessageBoxAction,
        UserDropDownAction,
        RoomSideBarAction,
    };
    Q_ENUM(ButtonContext);

    ActionButton();
    ~ActionButton();

    [[nodiscard]] QByteArray appId() const;
    void setAppId(const QByteArray &newAppId);

    [[nodiscard]] QByteArray actionId() const;
    void setActionId(const QByteArray &newActionId);

    [[nodiscard]] QString labelI18n() const;
    void setLabelI18n(const QString &newLabelI18n);

    [[nodiscard]] RoomTypeFilters roomTypeFilters() const;
    void setRoomTypeFilters(RoomTypeFilters newRoomTypeFilter);

    [[nodiscard]] QStringList hasOneRole() const;
    void setHasOneRole(const QStringList &newOneRole);

    void parseActionButton(const QJsonObject &json);

    [[nodiscard]] bool operator==(const ActionButton &other) const;

    [[nodiscard]] ButtonContext context() const;
    void setContext(ButtonContext newContext);

    [[nodiscard]] QStringList hasAllRoles() const;
    void setHasAllRoles(const QStringList &newHasAllRoles);

    [[nodiscard]] QStringList hasOnePermission() const;
    void setHasOnePermission(const QStringList &newHasOnePermission);

    [[nodiscard]] QStringList hasAllPermissions() const;
    void setHasAllPermissions(const QStringList &newHasAllPermissions);

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT ActionButton::ButtonContext convertContextFromString(const QString &str) const;
    LIBRUQOLACORE_NO_EXPORT void parseWhen(const QJsonObject &json);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT ActionButton::RoomTypeFilter convertRoomTypeFiltersFromString(const QString &str) const;

    QStringList mHasOnePermission;
    QStringList mHasAllPermissions;

    QStringList mHasAllRoles;
    QStringList mHasOneRole;
    QByteArray mAppId;
    QByteArray mActionId;
    QString mLabelI18n;
    ButtonContext mContext = ButtonContext::Unknown;
    RoomTypeFilters mRoomTypeFilters = RoomTypeFilter::Unknown;
};

Q_DECLARE_METATYPE(ActionButton)
Q_DECLARE_TYPEINFO(ActionButton, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ActionButton &t);
