/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
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
    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    bool helpEvent(QHelpEvent *helpEvent, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index) override;

    [[nodiscard]] bool mouseEvent(QEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);

    [[nodiscard]] bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);

protected:
    [[nodiscard]] RocketChatAccount *rocketChatAccount(const QModelIndex &index) const override;

private:
    struct Layout {
        // Text message
        QRect textRect;
        qreal baseLine; // used to draw sender/timestamp
    };
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT BannerInfoListViewDelegate::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QTextDocument *documentForModelIndex(const QModelIndex &index, int width) const override;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QPoint adaptMousePosition(const QPoint &pos, QRect textRect, const QStyleOptionViewItem &option);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString cacheIdentifier(const QModelIndex &index) const;

    RocketChatAccount *const mRocketChatAccount;
};
