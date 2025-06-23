/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "actionbuttonutil.h"
using namespace Qt::Literals::StringLiterals;

ActionButton::RoomTypeFilter ActionButtonUtil::convertRoomTypeToActionButtonRoomTypeFilter(Room *r)
{
    if (!r) {
        return ActionButton::RoomTypeFilter::Unknown;
    }
    switch (r->channelType()) {
    case Room::RoomType::Unknown:
        return ActionButton::RoomTypeFilter::Unknown;
    case Room::RoomType::Direct:
        // Disable for the moment
        /*
         if (r->uids().count() > 2) {
             return ActionButton::RoomTypeFilter::DirectMultiple;
         }
         */
        return ActionButton::RoomTypeFilter::Direct;
    case Room::RoomType::Channel:
        if (r->isDiscussionRoom()) {
            return ActionButton::RoomTypeFilter::PublicDiscussion;
        }
        if (r->teamInfo().isValid()) {
            return ActionButton::RoomTypeFilter::PublicTeam;
        }
        return ActionButton::RoomTypeFilter::PublicChannel;
    case Room::RoomType::Private:
        if (r->isDiscussionRoom()) {
            return ActionButton::RoomTypeFilter::PrivateDiscussion;
        }
        if (r->teamInfo().isValid()) {
            return ActionButton::RoomTypeFilter::PrivateTeam;
        }
        return ActionButton::RoomTypeFilter::PrivateChannel;
    }
    return ActionButton::RoomTypeFilter::Unknown;
}

QString ActionButtonUtil::generateTranslateIdentifier(const ActionButton &actionButton)
{
    const QString appId = QString::fromLatin1(actionButton.appId());
    const QString translateIdentifier = u"app-"_s + appId + u'.' + actionButton.labelI18n();
    return translateIdentifier;
}
