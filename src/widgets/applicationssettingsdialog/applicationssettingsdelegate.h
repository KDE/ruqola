/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "lrucache.h"
#include <QItemDelegate>
class ApplicationsSettingsDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsDelegate(QObject *parent = nullptr);
    ~ApplicationsSettingsDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    struct Layout {
        QPixmap appPixmap;
        QString appName;
        QString appDescription;
        bool premium = false;
        // TODO
    };

    [[nodiscard]] ApplicationsSettingsDelegate::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    [[nodiscard]] QByteArray cacheIdentifier(const QModelIndex &index) const;
    // Cache SizeHint value
    // We need to clear it when we resize widget.
    mutable LRUCache<QByteArray, QSize> mSizeHintCache;
};
