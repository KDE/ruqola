/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QStyledItemDelegate>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseJsonDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ExploreDatabaseJsonDelegate(QObject *parent = nullptr);
    ~ExploreDatabaseJsonDelegate() override;

    [[nodiscard]] bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index) override;
};
