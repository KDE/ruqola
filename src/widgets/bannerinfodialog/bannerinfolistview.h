/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

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
    Q_REQUIRED_RESULT bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    Q_REQUIRED_RESULT bool mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    BannerInfoListViewDelegate *const mBannerInfoListViewDelegate;
    RocketChatAccount *const mRocketChatAccount;
};
