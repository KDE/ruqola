/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "lrucache.h"
#include <QItemDelegate>
#include <QTextDocument>

class LIBRUQOLAWIDGETS_TESTS_EXPORT NotificationHistoryDelegate : public QItemDelegate
{
public:
    explicit NotificationHistoryDelegate(QObject *parent = nullptr);
    ~NotificationHistoryDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
protected:
    Q_REQUIRED_RESULT QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:
    Q_REQUIRED_RESULT QTextDocument *documentForIndex(const QModelIndex &index, int width) const;

    struct Layout {
        // Sender
        QString senderText;
        QFont senderFont;
        QRectF senderRect;

        // Avatar pixmap
        QPixmap avatarPixmap;
        QPointF avatarPos;

        // Text message
        QRect textRect;
        qreal baseLine; // used to draw sender/timestamp

        QString timeStampText;
        QPoint timeStampPos;
    };
    Q_REQUIRED_RESULT NotificationHistoryDelegate::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    mutable LRUCache<QString, std::unique_ptr<QTextDocument>, 32> mDocumentCache;
    Q_REQUIRED_RESULT QSize textSizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option, qreal *pBaseLine) const;
};
