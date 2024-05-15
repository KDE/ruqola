/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationdesktopsoundpreferenceproxymodel.h"

NotificationDesktopSoundPreferenceProxyModel::NotificationDesktopSoundPreferenceProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
}

NotificationDesktopSoundPreferenceProxyModel::~NotificationDesktopSoundPreferenceProxyModel() = default;

bool NotificationDesktopSoundPreferenceProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    // TODO
    return true;
}

#include "moc_notificationdesktopsoundpreferenceproxymodel.cpp"
