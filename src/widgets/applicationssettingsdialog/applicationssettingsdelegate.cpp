/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsdelegate.h"
#include "model/appsmarketplacemodel.h"

ApplicationsSettingsDelegate::ApplicationsSettingsDelegate(QObject *parent)
    : QItemDelegate{parent}
{
}

ApplicationsSettingsDelegate::~ApplicationsSettingsDelegate() = default;

void ApplicationsSettingsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // TODO reimplement it
    QItemDelegate::paint(painter, option, index);
}

QSize ApplicationsSettingsDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QSize size = QItemDelegate::sizeHint(option, index);
    return size + QSize(0, 4 * option.widget->devicePixelRatioF());
}

#include "moc_applicationssettingsdelegate.cpp"
