/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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


#include "notificationpreferencemodel.h"
#include <KLocalizedString>

NotificationPreferenceModel::NotificationPreferenceModel(QObject *parent)
    : QAbstractListModel(parent)
{
    fillModel();
}

NotificationPreferenceModel::~NotificationPreferenceModel()
{

}

int NotificationPreferenceModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mNotificationPreferenceList.count();
}

QVariant NotificationPreferenceModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mNotificationPreferenceList.count()) {
        return {};
    }
    NotificationPreferenceInfo preferenceInfo = mNotificationPreferenceList.at(index.row());
    switch (role) {
    case NotificationPreferenceI18n:
        return preferenceInfo.displayText;
    case NotificationPreference:
        return preferenceInfo.preference;
    case Icon:
        return preferenceInfo.icon;
    }
    return {};
}

QHash<int, QByteArray> NotificationPreferenceModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NotificationPreferenceI18n] = QByteArrayLiteral("preferencei18n");
    roles[NotificationPreference] = QByteArrayLiteral("preference");
    roles[Icon] = QByteArrayLiteral("icon");
    return roles;
}

void NotificationPreferenceModel::fillModel()
{
    {
        NotificationPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Online");
        preferenceInfo.icon = QIcon::fromTheme(QStringLiteral("im-user-online"));
        //preferenceInfo.preference = User::PresenceStatus::PresenceOnline;
        mNotificationPreferenceList.append(preferenceInfo);
    }
    {
        NotificationPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Busy");
        preferenceInfo.icon = QIcon::fromTheme(QStringLiteral("im-user-busy"));
        //preferenceInfo.preference = User::PresenceStatus::PresenceBusy;
        mNotificationPreferenceList.append(preferenceInfo);
    }
    {
        NotificationPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Away");
        preferenceInfo.icon = QIcon::fromTheme(QStringLiteral("im-user-away"));
        //preferenceInfo.preference = User::PresenceStatus::PresenceAway;
        mNotificationPreferenceList.append(preferenceInfo);
    }
    {
        NotificationPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Offline");
        preferenceInfo.icon = QIcon::fromTheme(QStringLiteral("im-user-offline"));
        //preferenceInfo.preference = User::PresenceStatus::PresenceOffline;
        mNotificationPreferenceList.append(preferenceInfo);
    }
}

void NotificationPreferenceModel::setCurrentNotificationPreference(const QString &preference)
{
    int newStatusIndex = 0;
    for (int i = 0; i < mNotificationPreferenceList.count(); ++i) {
        if (mNotificationPreferenceList.at(i).preference == preference) {
            newStatusIndex = i;
            break;
        }
    }
    if (mCurrentPreference != newStatusIndex) {
        mCurrentPreference = newStatusIndex;
        Q_EMIT currentNotificationPreferenceChanged();
    }
}

int NotificationPreferenceModel::currentPreference() const
{
    return mCurrentPreference;
}
