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

#include "notificationdesktopdurationpreferencemodel.h"
#include <KLocalizedString>

NotificationDesktopDurationPreferenceModel::NotificationDesktopDurationPreferenceModel(QObject *parent)
    : QAbstractListModel(parent)
{
    fillModel();
}

NotificationDesktopDurationPreferenceModel::~NotificationDesktopDurationPreferenceModel()
{
}

int NotificationDesktopDurationPreferenceModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mNotificationDestktopDurationPreferenceList.count();
}

QVariant NotificationDesktopDurationPreferenceModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mNotificationDestktopDurationPreferenceList.count()) {
        return {};
    }
    NotificationDesktopDurationPreferenceInfo preferenceInfo = mNotificationDestktopDurationPreferenceList.at(index.row());
    switch (role) {
    case NotificationPreferenceI18n:
        return preferenceInfo.displayText;
    case NotificationPreference:
        return preferenceInfo.preference;
    }
    return {};
}

QHash<int, QByteArray> NotificationDesktopDurationPreferenceModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NotificationPreferenceI18n] = QByteArrayLiteral("preferencei18n");
    roles[NotificationPreference] = QByteArrayLiteral("preference");
    return roles;
}

void NotificationDesktopDurationPreferenceModel::fillModel()
{
    mNotificationDestktopDurationPreferenceList.reserve(6);
    {
        //Default ???? Verify it. Perhaps 0 ? Don't know
        NotificationDesktopDurationPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Default");
        preferenceInfo.preference = QStringLiteral("default");
        mNotificationDestktopDurationPreferenceList.append(preferenceInfo);
    }
    for (int i = 1; i <= 5; ++i) {
        NotificationDesktopDurationPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18np("1 second", "%1 seconds", i);
        preferenceInfo.preference = QString::number(i);
        mNotificationDestktopDurationPreferenceList.append(preferenceInfo);
    }
}

int NotificationDesktopDurationPreferenceModel::setCurrentNotificationPreference(const QString &preference)
{
    int newStatusIndex = 0;
    for (int i = 0; i < mNotificationDestktopDurationPreferenceList.count(); ++i) {
        if (mNotificationDestktopDurationPreferenceList.at(i).preference == preference) {
            newStatusIndex = i;
            break;
        }
    }
    if (mCurrentPreference != newStatusIndex) {
        mCurrentPreference = newStatusIndex;
        Q_EMIT currentNotificationPreferenceChanged();
    }
    return mCurrentPreference;
}

QString NotificationDesktopDurationPreferenceModel::currentPreference(int index) const
{
    const QString str = mNotificationDestktopDurationPreferenceList.at(index).preference;
    return str;
}
