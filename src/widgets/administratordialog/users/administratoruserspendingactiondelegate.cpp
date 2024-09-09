/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratoruserspendingactiondelegate.h"

AdministratorUsersPendingActionDelegate::AdministratorUsersPendingActionDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
}

AdministratorUsersPendingActionDelegate::~AdministratorUsersPendingActionDelegate() = default;

void AdministratorUsersPendingActionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return;
    }

    // TODO
}
#include "moc_administratoruserspendingactiondelegate.cpp"
