/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>

class LIBRUQOLACORE_EXPORT NotificationDesktopSoundPreferenceProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit NotificationDesktopSoundPreferenceProxyModel(QObject *parent = nullptr);
    ~NotificationDesktopSoundPreferenceProxyModel() override;

protected:
    [[nodiscard]] bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};
