/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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

#include "notificationdesktopsoundpreferencemodel.h"
#include <KLocalizedString>

NotificationDesktopSoundPreferenceModel::NotificationDesktopSoundPreferenceModel(QObject *parent)
    : QAbstractListModel(parent)
{
    fillModel();
}

NotificationDesktopSoundPreferenceModel::~NotificationDesktopSoundPreferenceModel()
{
}

int NotificationDesktopSoundPreferenceModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mNotificationDestktopSoundPreferenceList.count();
}

QVariant NotificationDesktopSoundPreferenceModel::data(const QModelIndex &index, int role) const
{
    const int rowIndex = index.row();
    if (rowIndex < 0 || rowIndex >= mNotificationDestktopSoundPreferenceList.count()) {
        return {};
    }
    NotificationDesktopSoundPreferenceInfo preferenceInfo = mNotificationDestktopSoundPreferenceList.at(rowIndex);
    switch (role) {
    case Qt::DisplayRole:
    case NotificationPreferenceI18n:
        return preferenceInfo.displayText;
    case NotificationPreference:
        return preferenceInfo.preference;
    }

    return {};
}

QHash<int, QByteArray> NotificationDesktopSoundPreferenceModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NotificationPreferenceI18n] = QByteArrayLiteral("preferencei18n");
    roles[NotificationPreference] = QByteArrayLiteral("preference");
    return roles;
}

void NotificationDesktopSoundPreferenceModel::fillModel()
{
    mNotificationDestktopSoundPreferenceList.reserve(8);
    //This one must be first one.
    {
        NotificationDesktopSoundPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Default");
        preferenceInfo.preference = QStringLiteral("default");
        mNotificationDestktopSoundPreferenceList.append(preferenceInfo);
    }
    {
        NotificationDesktopSoundPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Nothing");
        preferenceInfo.preference = QStringLiteral("none");
        mNotificationDestktopSoundPreferenceList.append(preferenceInfo);
    }
    {
        NotificationDesktopSoundPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Beep");
        preferenceInfo.preference = QStringLiteral("beep");
        mNotificationDestktopSoundPreferenceList.append(preferenceInfo);
    }
    {
        NotificationDesktopSoundPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Chelle");
        preferenceInfo.preference = QStringLiteral("chelle");
        mNotificationDestktopSoundPreferenceList.append(preferenceInfo);
    }
    {
        NotificationDesktopSoundPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Ding");
        preferenceInfo.preference = QStringLiteral("ding");
        mNotificationDestktopSoundPreferenceList.append(preferenceInfo);
    }
    {
        NotificationDesktopSoundPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Droplet");
        preferenceInfo.preference = QStringLiteral("droplet");
        mNotificationDestktopSoundPreferenceList.append(preferenceInfo);
    }
    {
        NotificationDesktopSoundPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Highbell");
        preferenceInfo.preference = QStringLiteral("highbell");
        mNotificationDestktopSoundPreferenceList.append(preferenceInfo);
    }
    {
        NotificationDesktopSoundPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Seasons");
        preferenceInfo.preference = QStringLiteral("seasons");
        mNotificationDestktopSoundPreferenceList.append(preferenceInfo);
    }
}

int NotificationDesktopSoundPreferenceModel::setCurrentNotificationPreference(const QString &preference)
{
    int newStatusIndex = 0;
    for (int i = 0; i < mNotificationDestktopSoundPreferenceList.count(); ++i) {
        if (mNotificationDestktopSoundPreferenceList.at(i).preference == preference) {
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

QString NotificationDesktopSoundPreferenceModel::currentPreference(int index) const
{
    const QString str = mNotificationDestktopSoundPreferenceList.at(index).preference;
    return str;
}
