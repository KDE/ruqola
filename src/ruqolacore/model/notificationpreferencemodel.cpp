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
#include <QDebug>
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
    }
    return {};
}

QHash<int, QByteArray> NotificationPreferenceModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NotificationPreferenceI18n] = QByteArrayLiteral("preferencei18n");
    roles[NotificationPreference] = QByteArrayLiteral("preference");
    return roles;
}

void NotificationPreferenceModel::fillModel()
{
    mNotificationPreferenceList.reserve(4);
    {
        NotificationPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Default");
        preferenceInfo.preference = QString();
        mNotificationPreferenceList.append(preferenceInfo);
    }
    {
        NotificationPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("All Messages");
        preferenceInfo.preference = QStringLiteral("all");
        mNotificationPreferenceList.append(preferenceInfo);
    }
    {
        NotificationPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Mentions");
        preferenceInfo.preference = QStringLiteral("mentions");
        mNotificationPreferenceList.append(preferenceInfo);
    }
    {
        NotificationPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Nothing");
        preferenceInfo.preference = QStringLiteral("nothing");
        mNotificationPreferenceList.append(preferenceInfo);
    }
}

int NotificationPreferenceModel::setCurrentNotificationPreference(const QString &preference)
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
    return mCurrentPreference;
}

QString NotificationPreferenceModel::currentPreference() const
{
    qDebug() << " mmCurrentPreferenceCurr" << mCurrentPreference;
    return mNotificationPreferenceList.at(mCurrentPreference).preference;
}
