/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationdesktopsoundpreferencemodel.h"
#include <KLocalizedString>

NotificationDesktopSoundPreferenceModel::NotificationDesktopSoundPreferenceModel(QObject *parent)
    : QAbstractListModel(parent)
{
    fillModel();
}

NotificationDesktopSoundPreferenceModel::~NotificationDesktopSoundPreferenceModel() = default;

int NotificationDesktopSoundPreferenceModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
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

void NotificationDesktopSoundPreferenceModel::fillModel()
{
    mNotificationDestktopSoundPreferenceList.reserve(8);
    // This one must be first one.
    {
        NotificationDesktopSoundPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Default");
        preferenceInfo.preference = QStringLiteral("default");
        mNotificationDestktopSoundPreferenceList.append(std::move(preferenceInfo));
    }
    {
        NotificationDesktopSoundPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Nothing");
        preferenceInfo.preference = QStringLiteral("none");
        mNotificationDestktopSoundPreferenceList.append(std::move(preferenceInfo));
    }
    {
        NotificationDesktopSoundPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Beep");
        preferenceInfo.preference = QStringLiteral("beep");
        mNotificationDestktopSoundPreferenceList.append(std::move(preferenceInfo));
    }
    {
        NotificationDesktopSoundPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Chelle");
        preferenceInfo.preference = QStringLiteral("chelle");
        mNotificationDestktopSoundPreferenceList.append(std::move(preferenceInfo));
    }
    {
        NotificationDesktopSoundPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Ding");
        preferenceInfo.preference = QStringLiteral("ding");
        mNotificationDestktopSoundPreferenceList.append(std::move(preferenceInfo));
    }
    {
        NotificationDesktopSoundPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Droplet");
        preferenceInfo.preference = QStringLiteral("droplet");
        mNotificationDestktopSoundPreferenceList.append(std::move(preferenceInfo));
    }
    {
        NotificationDesktopSoundPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Highbell");
        preferenceInfo.preference = QStringLiteral("highbell");
        mNotificationDestktopSoundPreferenceList.append(std::move(preferenceInfo));
    }
    {
        NotificationDesktopSoundPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Seasons");
        preferenceInfo.preference = QStringLiteral("seasons");
        mNotificationDestktopSoundPreferenceList.append(std::move(preferenceInfo));
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

#include "moc_notificationdesktopsoundpreferencemodel.cpp"
