/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QStyledItemDelegate>

class AdministratorUsersPendingActionDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit AdministratorUsersPendingActionDelegate(QObject *parent = nullptr);
    ~AdministratorUsersPendingActionDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

Q_SIGNALS:
    void pendingActionActivated();
};
