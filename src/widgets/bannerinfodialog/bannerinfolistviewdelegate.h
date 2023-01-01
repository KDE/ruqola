/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include "lrucache.h"
#include "misc/messagelistdelegatebase.h"
class QTextDocument;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT BannerInfoListViewDelegate : public MessageListDelegateBase
{
    Q_OBJECT
public:
    explicit BannerInfoListViewDelegate(QListView *view, RocketChatAccount *account, QObject *parent = nullptr);
    ~BannerInfoListViewDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    Q_REQUIRED_RESULT QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    bool helpEvent(QHelpEvent *helpEvent, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index) override;

    Q_REQUIRED_RESULT bool mouseEvent(QEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);

    Q_REQUIRED_RESULT bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);

protected:
    Q_REQUIRED_RESULT RocketChatAccount *rocketChatAccount(const QModelIndex &index) const override;

private:
    struct Layout {
        // Text message
        QRect textRect;
        qreal baseLine; // used to draw sender/timestamp
    };
    Q_REQUIRED_RESULT BannerInfoListViewDelegate::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    Q_REQUIRED_RESULT QTextDocument *documentForModelIndex(const QModelIndex &index, int width) const override;
    Q_REQUIRED_RESULT QPoint adaptMousePosition(const QPoint &pos, QRect textRect, const QStyleOptionViewItem &option);

    RocketChatAccount *const mRocketChatAccount;
};
