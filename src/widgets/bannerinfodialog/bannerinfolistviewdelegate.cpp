/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfolistviewdelegate.h"

BannerInfoListViewDelegate::BannerInfoListViewDelegate(QObject *parent)
    : QItemDelegate{parent}
{
}

BannerInfoListViewDelegate::~BannerInfoListViewDelegate() = default;

void BannerInfoListViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // TODO
}
