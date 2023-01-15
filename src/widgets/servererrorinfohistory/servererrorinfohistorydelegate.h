/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "delegateutils/textselection.h"
#include "libruqolawidgets_private_export.h"
#include "misc/messagelistdelegatebase.h"
class QListView;

class LIBRUQOLAWIDGETS_TESTS_EXPORT ServerErrorInfoHistoryDelegate : public MessageListDelegateBase
{
    Q_OBJECT
public:
    explicit ServerErrorInfoHistoryDelegate(QListView *view, QObject *parent = nullptr);
    ~ServerErrorInfoHistoryDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    Q_REQUIRED_RESULT bool helpEvent(QHelpEvent *helpEvent, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index) override;

    Q_REQUIRED_RESULT bool mouseEvent(QEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);

    Q_REQUIRED_RESULT bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);

    Q_REQUIRED_RESULT QString selectedText() const;

    Q_REQUIRED_RESULT bool hasSelection() const;

protected:
    Q_REQUIRED_RESULT QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    Q_REQUIRED_RESULT RocketChatAccount *rocketChatAccount(const QModelIndex &index) const override;

private:
    Q_REQUIRED_RESULT QTextDocument *documentForModelIndex(const QModelIndex &index, int width) const override;

    struct Layout {
        // Text message
        QRect textRect;
        qreal baseLine; // used to draw sender/timestamp

        QString timeStampText;
        QPoint timeStampPos;
    };
    void drawAccountRoomInfo(QPainter *painter, const QModelIndex &index, const QStyleOptionViewItem &option) const;
    Q_REQUIRED_RESULT ServerErrorInfoHistoryDelegate::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    Q_REQUIRED_RESULT QPoint adaptMousePosition(const QPoint &pos, QRect textRect, const QStyleOptionViewItem &option);
};
