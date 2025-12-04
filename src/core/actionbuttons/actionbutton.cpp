/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "actionbutton.h"
#include "ruqola_action_buttons_debug.h"

#include <QJsonArray>
#include <QJsonObject>
QT_IMPL_METATYPE_EXTERN_TAGGED(ActionButton, Ruqola_ActionButton)
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
    mButtonContext = convertContextFromString(json["context"_L1].toString());
    parseWhen(json["when"_L1].toObject());
    mCategory = convertCategoryFromString(json["category"_L1].toString());

    // TODO variant
}

ActionButton::Category ActionButton::convertCategoryFromString(const QString &str) const
{
    if (str.isEmpty()) {
        return ActionButton::Category::Unknown;
    } else if (str == "default"_L1) {
        return ActionButton::Category::Default;
    } else if (str == "ai"_L1) {
        return ActionButton::Category::AI;
    } else {
        qCWarning(RUQOLA_ACTION_BUTTONS_LOG) << "Unknown category action type " << str;
        return ActionButton::Category::Unknown;
    }
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

    const QJsonArray hasOnePermission = json["hasOnePermission"_L1].toArray();
    for (const auto &r : hasOnePermission) {
        mHasOnePermission.append(r.toString());
    }

    const QJsonArray hasAllPermissions = json["hasAllPermissions?"_L1].toArray();
    for (const auto &r : hasAllPermissions) {
        mHasAllPermissions.append(r.toString());
    }

    const QJsonArray messageActionContexts = json["messageActionContext"_L1].toArray();
    for (const auto &r : messageActionContexts) {
        mMessageActionContexts |= convertMessageActionContextsFromString(r.toString());
    }
}

ActionButton::MessageActionContext ActionButton::convertMessageActionContextsFromString(const QString &str) const
{
    if (str.isEmpty()) {
        return ActionButton::MessageActionContext::Unknown;
    } else if (str == "message"_L1) {
        return ActionButton::MessageActionContext::Message;
    } else if (str == "message-mobile"_L1) {
        return ActionButton::MessageActionContext::MessageMobile;
    } else if (str == "threads"_L1) {
        return ActionButton::MessageActionContext::Threads;
    } else if (str == "starred"_L1) {
        return ActionButton::MessageActionContext::Starred;
    } else {
        qCWarning(RUQOLA_ACTION_BUTTONS_LOG) << "Unknown MessageActionContext type " << str;
    }
    return ActionButton::MessageActionContext::Unknown;
}

ActionButton::Category ActionButton::category() const
{
    return mCategory;
}

void ActionButton::setCategory(Category newCategory)
{
    mCategory = newCategory;
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

ActionButton::MessageActionContexts ActionButton::messageActionContexts() const
{
    return mMessageActionContexts;
}

void ActionButton::setMessageActionContexts(MessageActionContexts newMessageActionContexts)
{
    mMessageActionContexts = newMessageActionContexts;
}

QStringList ActionButton::hasAllPermissions() const
{
    return mHasAllPermissions;
}

void ActionButton::setHasAllPermissions(const QStringList &newHasAllPermissions)
{
    mHasAllPermissions = newHasAllPermissions;
}

QStringList ActionButton::hasOnePermission() const
{
    return mHasOnePermission;
}

void ActionButton::setHasOnePermission(const QStringList &newHasOnePermission)
{
    mHasOnePermission = newHasOnePermission;
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
        && other.hasOneRole() == hasOneRole() && other.buttonContext() == buttonContext() && other.hasAllRoles() == hasAllRoles()
        && other.hasAllPermissions() == hasAllPermissions() && other.hasOnePermission() == hasOnePermission()
        && other.messageActionContexts() == messageActionContexts() && other.category() == category();
}

ActionButton::ButtonContext ActionButton::buttonContext() const
{
    return mButtonContext;
}

void ActionButton::setButtonContext(ButtonContext newContext)
{
    mButtonContext = newContext;
}

QDebug operator<<(QDebug d, const ActionButton &t)
{
    d.space() << "appId:" << t.appId();
    d.space() << "actionId:" << t.actionId();
    d.space() << "labelI18n:" << t.labelI18n();
    d.space() << "hasOneRole:" << t.hasOneRole();
    d.space() << "hasAllRoles:" << t.hasAllRoles();
    d.space() << "hasOnePermission:" << t.hasOnePermission();
    d.space() << "hasAllPermissions:" << t.hasAllPermissions();
    d.space() << "buttonContext:" << t.buttonContext();
    d.space() << "category:" << t.category();
    d.space() << "roomTypeFilters:" << static_cast<int>(t.roomTypeFilters());
    d.space() << "messageActionContexts:" << static_cast<int>(t.messageActionContexts());
    return d;
}

QDebug operator<<(QDebug d, const ActionButton::FilterActionInfo &t)
{
    d.space() << "roomTypeFilter:" << t.roomTypeFilter;
    d.space() << "buttonContext:" << t.buttonContext;
    d.space() << "messageActionContext:" << t.messageActionContext;
    d.space() << "category:" << t.category;
    return d;
}

#include "moc_actionbutton.cpp"
