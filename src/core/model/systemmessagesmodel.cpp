/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "systemmessagesmodel.h"
#include <KLocalizedString>

SystemMessagesModel::SystemMessagesModel(QObject *parent)
    : QAbstractListModel(parent)
{
    fillModel();
}

SystemMessagesModel::~SystemMessagesModel()
{
}

int SystemMessagesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mSystemMessagesList.count();
}

QVariant SystemMessagesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mSystemMessagesList.count()) {
        return {};
    }
    const SystemMessagesInfo systemMessagesInfo = mSystemMessagesList.at(index.row());
    switch (role) {
    case SystemMessagesI18n:
        return systemMessagesInfo.displayText;
    case SystemMessages:
        return systemMessagesInfo.messagesText;
    }

    return {};
}

void SystemMessagesModel::fillModel()
{
    {
        SystemMessagesInfo info;
        info.displayText = i18n("Hide \"User Added\" messages");
        info.messagesText = QStringLiteral("au");
        mSystemMessagesList.append(info);
    }
    {
        SystemMessagesInfo info;
        info.displayText = i18n("Hide \"Welcome\" messages");
        info.messagesText = QStringLiteral("wm");
        mSystemMessagesList.append(info);
    }
    {
        SystemMessagesInfo info;
        info.displayText = i18n("Hide \"User Joined Conversation\" messages");
        info.messagesText = QStringLiteral("ut");
        mSystemMessagesList.append(info);
    }
    {
        SystemMessagesInfo info;
        info.displayText = i18n("Hide \"User Leave\" messages");
        info.messagesText = QStringLiteral("ul");
        mSystemMessagesList.append(info);
    }
    {
        SystemMessagesInfo info;
        info.displayText = i18n("Hide \"User Join\" messages");
        info.messagesText = QStringLiteral("uj");
        mSystemMessagesList.append(info);
    }
    {
        SystemMessagesInfo info;
        info.displayText = i18n("Hide \"Role No Longer Defined\" messages");
        info.messagesText = QStringLiteral("role_removed");
        mSystemMessagesList.append(info);
    }
    {
        SystemMessagesInfo info;
        info.displayText = i18n("Hide \"Was Set Role\" messages");
        info.messagesText = QStringLiteral("role_added");
        mSystemMessagesList.append(info);
    }
    {
        SystemMessagesInfo info;
        info.displayText = i18n("Hide \"User Removed\" messages");
        info.messagesText = QStringLiteral("ru");
        mSystemMessagesList.append(info);
    }
    {
        SystemMessagesInfo info;
        info.displayText = i18n("Hide \"Room Unarchived\" messages");
        info.messagesText = QStringLiteral("room_unarchived");
        mSystemMessagesList.append(info);
    }
    {
        SystemMessagesInfo info;
        info.displayText = i18n("Hide \"Room encryption disabled\" messages");
        info.messagesText = QStringLiteral("room_disabled_encryption");
        mSystemMessagesList.append(info);
    }
    {
        SystemMessagesInfo info;
        info.displayText = i18n("Hide \"Room encryption enabled\" messages");
        info.messagesText = QStringLiteral("enabled_encryption");
        mSystemMessagesList.append(info);
    }
    {
        SystemMessagesInfo info;
        info.displayText = i18n("Hide \"Room type changed\" messages");
        info.messagesText = QStringLiteral("room_changed_privacy");
        mSystemMessagesList.append(info);
    }
    {
        SystemMessagesInfo info;
        info.displayText = i18n("Hide \"Room avatar changed\" messages");
        info.messagesText = QStringLiteral("room_changed_avatar");
        mSystemMessagesList.append(info);
    }
    {
        SystemMessagesInfo info;
        info.displayText = i18n("Hide \"User Muted / Unmuted\" messages");
        info.messagesText = QStringLiteral("mute_unmute");
        mSystemMessagesList.append(info);
    }
    {
        SystemMessagesInfo info;
        info.displayText = i18n("Hide \"Room Name Changed\" messages");
        info.messagesText = QStringLiteral("r");
        mSystemMessagesList.append(info);
    }
    {
        SystemMessagesInfo info;
        info.displayText = i18n("Hide \"Message Removed\" messages");
        info.messagesText = QStringLiteral("rm");
        mSystemMessagesList.append(info);
    }
    {
        SystemMessagesInfo info;
        info.displayText = i18n("Hide \"Room Archived\" messages");
        info.messagesText = QStringLiteral("room_archived");
        mSystemMessagesList.append(info);
    }
}
