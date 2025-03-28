/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationpreferencemodel.h"
#include <KLocalizedString>
using namespace Qt::Literals::StringLiterals;
NotificationPreferenceModel::NotificationPreferenceModel(QObject *parent)
    : QAbstractListModel(parent)
{
    fillModel();
}

NotificationPreferenceModel::~NotificationPreferenceModel() = default;

int NotificationPreferenceModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) // flat model
        return 0;
    return mNotificationPreferenceList.count();
}

QVariant NotificationPreferenceModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mNotificationPreferenceList.count()) {
        return {};
    }
    NotificationPreferenceInfo preferenceInfo = mNotificationPreferenceList.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case NotificationPreferenceI18nRole:
        return preferenceInfo.displayText;
    case NotificationPreferenceRole:
        return preferenceInfo.preference;
    }

    return {};
}

void NotificationPreferenceModel::fillModel()
{
    mNotificationPreferenceList.reserve(4);
    {
        NotificationPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Default");
        preferenceInfo.preference = "default"_ba;
        mNotificationPreferenceList.append(std::move(preferenceInfo));
    }
    {
        NotificationPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("All Messages");
        preferenceInfo.preference = "all"_ba;
        mNotificationPreferenceList.append(std::move(preferenceInfo));
    }
    {
        NotificationPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Mentions");
        preferenceInfo.preference = "mentions"_ba;
        mNotificationPreferenceList.append(std::move(preferenceInfo));
    }
    {
        NotificationPreferenceInfo preferenceInfo;
        preferenceInfo.displayText = i18n("Nothing");
        preferenceInfo.preference = "nothing"_ba;
        mNotificationPreferenceList.append(std::move(preferenceInfo));
    }
}

int NotificationPreferenceModel::setCurrentNotificationPreference(const QByteArray &preference)
{
    int newStatusIndex = 0;
    for (int i = 0, total = mNotificationPreferenceList.count(); i < total; ++i) {
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

QByteArray NotificationPreferenceModel::currentPreference(int index) const
{
    const QByteArray str = mNotificationPreferenceList.at(index).preference;
    return str;
}

#include "moc_notificationpreferencemodel.cpp"
