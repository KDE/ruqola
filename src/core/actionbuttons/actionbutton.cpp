/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "actionbutton.h"
#include "ruqola_action_buttons_debug.h"

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

ActionButton::RoomTypeFilter ActionButton::roomTypeFilter() const
{
    return mRoomTypeFilter;
}

void ActionButton::setRoomTypeFilter(RoomTypeFilter newRoomTypeFilter)
{
    mRoomTypeFilter = newRoomTypeFilter;
}

QStringList ActionButton::oneRole() const
{
    return mOneRole;
}

void ActionButton::setOneRole(const QStringList &newOneRole)
{
    mOneRole = newOneRole;
}

void ActionButton::parseActionButton(const QJsonObject &json)
{
    mActionId = json["actionId"_L1].toString().toLatin1();
    mLabelI18n = json["labelI18n"_L1].toString();
    mAppId = json["appId"_L1].toString().toLatin1();
    mContext = convertContextFromString(json["context"_L1].toString());
    // TODO add hasOneRole and when
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
        qCWarning(RUQOLA_ACTION_BUTTONS_LOG) << "Unknonw button action type " << str;
        return ActionButton::ButtonContext::Unknown;
    }
}

bool ActionButton::operator==(const ActionButton &other) const
{
    return other.actionId() == actionId() && other.appId() == appId() && other.labelI18n() == labelI18n() && other.roomTypeFilter() == roomTypeFilter()
        && other.oneRole() == oneRole() && other.context() == context();
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
    d.space() << "oneRole:" << t.oneRole();
    d.space() << "context:" << t.context();
    d.space() << "roomTypeFilter:" << static_cast<int>(t.roomTypeFilter());
    return d;
}
