/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QItemDelegate>

#include "libruqolawidgets_private_export.h"
class LIBRUQOLAWIDGETS_TESTS_EXPORT BannerInfoListViewDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit BannerInfoListViewDelegate(QObject *parent = nullptr);
    ~BannerInfoListViewDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};
