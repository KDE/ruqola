/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QItemDelegate>

#include "libruqolawidgets_private_export.h"

class LIBRUQOLAWIDGETS_TESTS_EXPORT NotificationHistoryDelegate : public QItemDelegate
{
public:
    explicit NotificationHistoryDelegate(QObject *parent = nullptr);
    ~NotificationHistoryDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
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
    };
    Q_REQUIRED_RESULT NotificationHistoryDelegate::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
