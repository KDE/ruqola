/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationdesktopsoundpreferenceproxymodel.h"
#include "notificationdesktopsoundpreferencemodel.h"

NotificationDesktopSoundPreferenceProxyModel::NotificationDesktopSoundPreferenceProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
}

NotificationDesktopSoundPreferenceProxyModel::~NotificationDesktopSoundPreferenceProxyModel() = default;

bool NotificationDesktopSoundPreferenceProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex modelIndex = sourceModel()->index(source_row, 0, source_parent);
    const QByteArray identifier = modelIndex.data(NotificationDesktopSoundPreferenceModel::NotificationPreference).toByteArray();
    if (identifier == "none" || identifier == "default") {
        return false;
    }
    return true;
}

#include "moc_notificationdesktopsoundpreferenceproxymodel.cpp"
