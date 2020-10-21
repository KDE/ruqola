/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "statuscombobox.h"
#include <KLocalizedString>
#include <QIcon>

StatusCombobox::StatusCombobox(bool showModifyStatus, QWidget *parent)
    : QComboBox(parent)
{
    init(showModifyStatus);
}

StatusCombobox::~StatusCombobox()
{
}

void StatusCombobox::init(bool showModifyStatus)
{
    addItem(QIcon::fromTheme(QStringLiteral("im-user-online")), i18n("Online"), QVariant::fromValue(User::PresenceStatus::PresenceOnline));
    addItem(QIcon::fromTheme(QStringLiteral("im-user-busy")), i18n("Busy"), QVariant::fromValue(User::PresenceStatus::PresenceBusy));
    addItem(QIcon::fromTheme(QStringLiteral("im-user-away")), i18n("Away"), QVariant::fromValue(User::PresenceStatus::PresenceAway));
    addItem(QIcon::fromTheme(QStringLiteral("im-user-offline")), i18n("Offline"), QVariant::fromValue(User::PresenceStatus::PresenceOffline));
    if (showModifyStatus) {
        //TODO add icons ?
        addItem(i18n("Modify Status..."), QVariant::fromValue(User::PresenceStatus::Unknown));
    }
}

User::PresenceStatus StatusCombobox::status() const
{
    return currentData().value<User::PresenceStatus>();
}

void StatusCombobox::setStatus(User::PresenceStatus status)
{
    setCurrentIndex(findData(QVariant::fromValue(status)));
}
