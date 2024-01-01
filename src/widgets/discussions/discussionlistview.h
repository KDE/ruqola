/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include "misc/messagelistviewbase.h"
class RocketChatAccount;
class ListDiscussionDelegate;
class LIBRUQOLAWIDGETS_TESTS_EXPORT DiscussionListView : public MessageListViewBase
{
    Q_OBJECT
public:
    explicit DiscussionListView(RocketChatAccount *account, QWidget *parent = nullptr);
    ~DiscussionListView() override;

    void setSearchText(const QString &str);

protected:
    [[nodiscard]] bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    [[nodiscard]] bool mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    void slotOpenDiscussion(const QString &roomDiscussionId);
    void slotCustomContextMenuRequested(const QPoint &pos);
    void slotTextToSpeech(const QModelIndex &index);
    void slotSelectAll(const QModelIndex &index);
    ListDiscussionDelegate *const mListDiscussionDelegate;
    RocketChatAccount *const mRocketChatAccount;
};
