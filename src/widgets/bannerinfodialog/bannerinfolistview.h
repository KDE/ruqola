/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include "misc/messagelistviewbase.h"
class RocketChatAccount;
class BannerInfoListViewDelegate;
class LIBRUQOLAWIDGETS_TESTS_EXPORT BannerInfoListView : public MessageListViewBase
{
    Q_OBJECT
public:
    explicit BannerInfoListView(RocketChatAccount *account, QWidget *parent = nullptr);
    ~BannerInfoListView() override;

protected:
    [[nodiscard]] bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    [[nodiscard]] bool mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    void slotCustomContextMenuRequested(const QPoint &pos);
    void slotSelectAll(const QModelIndex &index);
    [[nodiscard]] QString selectedText(const QModelIndex &index) override;
    [[nodiscard]] QString selectedText() const;
    BannerInfoListViewDelegate *const mBannerInfoListViewDelegate;
};
