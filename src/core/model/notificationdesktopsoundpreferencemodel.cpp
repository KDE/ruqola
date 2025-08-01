/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationdesktopsoundpreferencemodel.h"
#include "customsound/customsoundsmanager.h"

NotificationDesktopSoundPreferenceModel::NotificationDesktopSoundPreferenceModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

NotificationDesktopSoundPreferenceModel::~NotificationDesktopSoundPreferenceModel() = default;

int NotificationDesktopSoundPreferenceModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) { // flat model
        return 0;
    }

    return mCustomSoundManager->customSoundsInfo().count();
}

QVariant NotificationDesktopSoundPreferenceModel::data(const QModelIndex &index, int role) const
{
    const int rowIndex = index.row();
    if (rowIndex < 0 || rowIndex >= mCustomSoundManager->customSoundsInfo().count()) {
        return {};
    }
    const CustomSoundInfo preferenceInfo = mCustomSoundManager->customSoundsInfo().at(rowIndex);
    switch (role) {
    case Qt::DisplayRole:
    case NotificationPreferenceI18n:
        return preferenceInfo.name();
    case NotificationPreference:
        return preferenceInfo.identifier();
    }

    return {};
}

CustomSoundsManager *NotificationDesktopSoundPreferenceModel::customSoundManager() const
{
    return mCustomSoundManager;
}

void NotificationDesktopSoundPreferenceModel::setCustomSoundManager(CustomSoundsManager *newCustomSoundManager)
{
    if (mCustomSoundManager != newCustomSoundManager) {
        mCustomSoundManager = newCustomSoundManager;
        connect(mCustomSoundManager, &CustomSoundsManager::customSoundChanged, this, [this]() {
            beginResetModel();
            endResetModel();
        });
    }
}

int NotificationDesktopSoundPreferenceModel::setCurrentNotificationPreference(const QByteArray &preference)
{
    int newStatusIndex = 0;
    const QList<CustomSoundInfo> info = mCustomSoundManager->customSoundsInfo();
    for (int i = 0; i < info.count(); ++i) {
        if (info.at(i).identifier() == preference) {
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
    if (index >= mCustomSoundManager->customSoundsInfo().count()) {
        return {};
    }
    const QByteArray str = mCustomSoundManager->customSoundsInfo().at(index).identifier();
    return str;
}

#include "moc_notificationdesktopsoundpreferencemodel.cpp"
