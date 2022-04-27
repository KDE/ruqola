/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "delegateutils/textselection.h"
#include "libruqolawidgets_private_export.h"
#include "lrucache.h"
#include <QItemDelegate>
#include <QTextDocument>
class TextSelectionImpl;

class LIBRUQOLAWIDGETS_TESTS_EXPORT NotificationHistoryDelegate : public QItemDelegate, public DocumentFactoryInterface
{
    Q_OBJECT
public:
    explicit NotificationHistoryDelegate(QObject *parent = nullptr);
    ~NotificationHistoryDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void clearCache();

    Q_REQUIRED_RESULT bool helpEvent(QHelpEvent *helpEvent, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index) override;

    Q_REQUIRED_RESULT bool mouseEvent(QEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);

    Q_REQUIRED_RESULT bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);

Q_SIGNALS:
    void updateView(const QModelIndex &index);

protected:
    Q_REQUIRED_RESULT QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:
    Q_REQUIRED_RESULT QTextDocument *documentForIndex(const QModelIndex &index) const override;
    Q_REQUIRED_RESULT QTextDocument *documentForIndex(const MessageAttachment &msgAttach) const override;

    Q_REQUIRED_RESULT QTextDocument *documentForIndex(const QModelIndex &index, int width) const;
    Q_REQUIRED_RESULT bool handleMouseEvent(QMouseEvent *mouseEvent, QRect messageRect, const QStyleOptionViewItem &option, const QModelIndex &index);

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
    void drawAccountRoomInfo(QPainter *painter, const QModelIndex &index, const QStyleOptionViewItem &option) const;
    Q_REQUIRED_RESULT NotificationHistoryDelegate::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    Q_REQUIRED_RESULT QSize textSizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option, qreal *pBaseLine) const;
    Q_REQUIRED_RESULT QPoint adaptMousePosition(const QPoint &pos, QRect textRect, const QStyleOptionViewItem &option);
    Q_REQUIRED_RESULT bool maybeStartDrag(QMouseEvent *mouseEvent, QRect messageRect, const QStyleOptionViewItem &option, const QModelIndex &index);

    mutable LRUCache<QString, std::unique_ptr<QTextDocument>, 32> mDocumentCache;
    TextSelectionImpl *const mTextSelectionImpl;
};
