/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfolistview.h"
#include "bannerinfolistviewdelegate.h"

#include <QMouseEvent>
#include <QScrollBar>

BannerInfoListView::BannerInfoListView(RocketChatAccount *account, QWidget *parent)
    : MessageListViewBase(parent)
    , mBannerInfoListViewDelegate(new BannerInfoListViewDelegate(this, account, this))
    , mRocketChatAccount(account)
{
    setItemDelegate(mBannerInfoListViewDelegate);
    connect(mBannerInfoListViewDelegate, &BannerInfoListViewDelegate::updateView, this, [this](const QModelIndex &index) {
        update(index);
    });
    const auto lineHeight = fontMetrics().height() + 10;
    verticalScrollBar()->setSingleStep(lineHeight);
}

BannerInfoListView::~BannerInfoListView() = default;

bool BannerInfoListView::maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mBannerInfoListViewDelegate->maybeStartDrag(event, option, index);
}

bool BannerInfoListView::mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mBannerInfoListViewDelegate->mouseEvent(event, option, index);
}
