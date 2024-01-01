/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

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

    [[nodiscard]] bool helpEvent(QHelpEvent *helpEvent, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index) override;

    [[nodiscard]] bool mouseEvent(QEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);

    [[nodiscard]] bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);

    [[nodiscard]] QString selectedText() const;

    [[nodiscard]] bool hasSelection() const;

protected:
    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    [[nodiscard]] RocketChatAccount *rocketChatAccount(const QModelIndex &index) const override;

private:
    [[nodiscard]] QTextDocument *documentForModelIndex(const QModelIndex &index, int width) const override;

    struct Layout {
        // Text message
        QRect textRect;
        qreal baseLine; // used to draw sender/timestamp

        QString timeStampText;
        QPoint timeStampPos;
        bool sameAccountAsPreviousMessage = false;
    };
    void drawAccountInfo(QPainter *painter, const QModelIndex &index, const QStyleOptionViewItem &option) const;
    [[nodiscard]] ServerErrorInfoHistoryDelegate::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    [[nodiscard]] QPoint adaptMousePosition(const QPoint &pos, QRect textRect, const QStyleOptionViewItem &option);
    [[nodiscard]] QString cacheIdentifier(const QModelIndex &index) const;
};
