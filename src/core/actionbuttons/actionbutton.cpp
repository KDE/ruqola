/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "actionbutton.h"
#include "ruqola_action_buttons_debug.h"

#include <QJsonArray>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
ActionButton::ActionButton() = default;

ActionButton::~ActionButton() = default;

QByteArray ActionButton::appId() const
{
    return mAppId;
}

void ActionButton::setAppId(const QByteArray &newAppId)
{
    mAppId = newAppId;
}

QByteArray ActionButton::actionId() const
{
    return mActionId;
}

void ActionButton::setActionId(const QByteArray &newActionId)
{
    mActionId = newActionId;
}

QString ActionButton::labelI18n() const
{
    return mLabelI18n;
}

void ActionButton::setLabelI18n(const QString &newLabelI18n)
{
    mLabelI18n = newLabelI18n;
}

ActionButton::RoomTypeFilters ActionButton::roomTypeFilters() const
{
    return mRoomTypeFilters;
}

void ActionButton::setRoomTypeFilters(RoomTypeFilters newRoomTypeFilter)
{
    mRoomTypeFilters = newRoomTypeFilter;
}

QStringList ActionButton::hasOneRole() const
{
    return mHasOneRole;
}

void ActionButton::setHasOneRole(const QStringList &newOneRole)
{
    mHasOneRole = newOneRole;
}

void ActionButton::parseActionButton(const QJsonObject &json)
{
    mActionId = json["actionId"_L1].toString().toLatin1();
    mLabelI18n = json["labelI18n"_L1].toString();
    mAppId = json["appId"_L1].toString().toLatin1();
    mContext = convertContextFromString(json["context"_L1].toString());
    parseWhen(json["when"_L1].toObject());
}

void ActionButton::parseWhen(const QJsonObject &json)
{
    const QJsonArray roomTypes = json["roomTypes"_L1].toArray();
    for (const auto &r : roomTypes) {
        mRoomTypeFilters |= convertRoomTypeFiltersFromString(r.toString());
    }

    const QJsonArray hasOneRole = json["hasOneRole"_L1].toArray();
    for (const auto &r : hasOneRole) {
        mHasOneRole.append(r.toString());
    }

    const QJsonArray hasAllRoles = json["hasAllRoles?"_L1].toArray();
    for (const auto &r : hasAllRoles) {
        mHasAllRoles.append(r.toString());
    }
}

ActionButton::RoomTypeFilter ActionButton::convertRoomTypeFiltersFromString(const QString &str) const
{
    if (str.isEmpty()) {
        return ActionButton::RoomTypeFilter::Unknown;
    } else if (str == "public_channel"_L1) {
        return ActionButton::RoomTypeFilter::PublicChannel;
    } else if (str == "private_channel"_L1) {
        return ActionButton::RoomTypeFilter::PrivateChannel;
    } else if (str == "public_team"_L1) {
        return ActionButton::RoomTypeFilter::PublicTeam;
    } else if (str == "private_team"_L1) {
        return ActionButton::RoomTypeFilter::PrivateTeam;
    } else if (str == "public_discussion"_L1) {
        return ActionButton::RoomTypeFilter::PublicDiscussion;
    } else if (str == "private_discussion"_L1) {
        return ActionButton::RoomTypeFilter::PrivateDiscussion;
    } else if (str == "direct"_L1) {
        return ActionButton::RoomTypeFilter::Direct;
    } else if (str == "direct_multiple"_L1) {
        return ActionButton::RoomTypeFilter::DirectMultiple;
    } else if (str == "livechat"_L1) {
        return ActionButton::RoomTypeFilter::LiveChat;
    } else {
        qCWarning(RUQOLA_ACTION_BUTTONS_LOG) << "Unknown RoomTypeFilter type " << str;
    }

    return ActionButton::RoomTypeFilter::Unknown;
}

QStringList ActionButton::hasAllRoles() const
{
    return mHasAllRoles;
}

void ActionButton::setHasAllRoles(const QStringList &newHasAllRoles)
{
    mHasAllRoles = newHasAllRoles;
}

ActionButton::ButtonContext ActionButton::convertContextFromString(const QString &str) const
{
    if (str.isEmpty()) {
        return ActionButton::ButtonContext::Unknown;
    }
    if (str == "messageAction"_L1) {
        return ActionButton::ButtonContext::MessageAction;
    } else if (str == "roomAction"_L1) {
        return ActionButton::ButtonContext::RoomAction;
    } else if (str == "messageBoxAction"_L1) {
        return ActionButton::ButtonContext::MessageBoxAction;
    } else if (str == "userDropdownAction"_L1) {
        return ActionButton::ButtonContext::UserDropDownAction;
    } else if (str == "roomSideBarAction"_L1) {
        return ActionButton::ButtonContext::RoomSideBarAction;
    } else {
        qCWarning(RUQOLA_ACTION_BUTTONS_LOG) << "Unknown button action type " << str;
        return ActionButton::ButtonContext::Unknown;
    }
}

bool ActionButton::operator==(const ActionButton &other) const
{
    return other.actionId() == actionId() && other.appId() == appId() && other.labelI18n() == labelI18n() && other.roomTypeFilters() == roomTypeFilters()
        && other.hasOneRole() == hasOneRole() && other.context() == context() && other.hasAllRoles() == hasAllRoles();
}

ActionButton::ButtonContext ActionButton::context() const
{
    return mContext;
}

void ActionButton::setContext(ButtonContext newContext)
{
    mContext = newContext;
}

QDebug operator<<(QDebug d, const ActionButton &t)
{
    d.space() << "appId:" << t.appId();
    d.space() << "actionId:" << t.actionId();
    d.space() << "labelI18n:" << t.labelI18n();
    d.space() << "hasOneRole:" << t.hasOneRole();
    d.space() << "hasAllRoles:" << t.hasAllRoles();
    d.space() << "context:" << t.context();
    d.space() << "roomTypeFilter:" << static_cast<int>(t.roomTypeFilters());
    return d;
}
