/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationdesktopdurationpreferencemodel.h"
#include <KLocalizedString>

NotificationDesktopDurationPreferenceModel::NotificationDesktopDurationPreferenceModel(QObject *parent)
    : QAbstractListModel(parent)
{
    fillModel();
}

NotificationDesktopDurationPreferenceModel::~NotificationDesktopDurationPreferenceModel() = default;

int NotificationDesktopDurationPreferenceModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mNotificationDestktopDurationPreferenceList.count();
}

QVariant NotificationDesktopDurationPreferenceModel::data(const QModelIndex &index, int role) const
{
    const int rowIndex = index.row();
    if (rowIndex < 0 || rowIndex >= mNotificationDestktopDurationPreferenceList.count()) {
        return {};
    }
    NotificationDesktopDurationPreferenceInfo preferenceInfo = mNotificationDestktopDurationPreferenceList.at(rowIndex);
    switch (role) {
    case Qt::DisplayRole:
    case NotificationPreferenceI18n:
        return preferenceInfo.displayText;
    case NotificationPreference:
        return preferenceInfo.preference;
    }

    return {};
}

void NotificationDesktopDurationPreferenceModel::fillModel()
{
    mNotificationDestktopDurationPreferenceList.reserve(6);
    {
        // Default ???? Verify it. Perhaps 0 ? Don't know
        NotificationDesktopDurationPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Default");
        preferenceInfo.preference = QStringLiteral("default");
        mNotificationDestktopDurationPreferenceList.append(std::move(preferenceInfo));
    }
    for (int i = 1; i <= 5; ++i) {
        NotificationDesktopDurationPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18np("1 second", "%1 seconds", i);
        preferenceInfo.preference = QString::number(i);
        mNotificationDestktopDurationPreferenceList.append(std::move(preferenceInfo));
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

#include "moc_notificationdesktopdurationpreferencemodel.cpp"
