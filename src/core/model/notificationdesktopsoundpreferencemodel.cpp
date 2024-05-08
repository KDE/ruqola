/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationdesktopsoundpreferencemodel.h"

NotificationDesktopSoundPreferenceModel::NotificationDesktopSoundPreferenceModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

NotificationDesktopSoundPreferenceModel::~NotificationDesktopSoundPreferenceModel() = default;

int NotificationDesktopSoundPreferenceModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mNotificationDestktopSoundInfo.count();
}

QVariant NotificationDesktopSoundPreferenceModel::data(const QModelIndex &index, int role) const
{
    const int rowIndex = index.row();
    if (rowIndex < 0 || rowIndex >= mNotificationDestktopSoundInfo.count()) {
        return {};
    }
    const CustomSoundInfo preferenceInfo = mNotificationDestktopSoundInfo.at(rowIndex);
    switch (role) {
    case Qt::DisplayRole:
    case NotificationPreferenceI18n:
        return preferenceInfo.name();
    case NotificationPreference:
        return preferenceInfo.identifier();
    }

    return {};
}

QList<CustomSoundInfo> NotificationDesktopSoundPreferenceModel::notificationDestktopSoundInfo() const
{
    return mNotificationDestktopSoundInfo;
}

void NotificationDesktopSoundPreferenceModel::setNotificationDestktopSoundInfo(const QList<CustomSoundInfo> &newNotificationDestktopSoundInfo)
{
    mNotificationDestktopSoundInfo = newNotificationDestktopSoundInfo;
}

int NotificationDesktopSoundPreferenceModel::setCurrentNotificationPreference(const QByteArray &preference)
{
    int newStatusIndex = 0;
    for (int i = 0; i < mNotificationDestktopSoundInfo.count(); ++i) {
        if (mNotificationDestktopSoundInfo.at(i).identifier() == preference) {
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

QByteArray NotificationDesktopSoundPreferenceModel::currentPreference(int index) const
{
    const QByteArray str = mNotificationDestktopSoundInfo.at(index).identifier();
    return str;
}

#include "moc_notificationdesktopsoundpreferencemodel.cpp"
