/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include "misc/messagelistviewbase.h"

class LIBRUQOLAWIDGETS_TESTS_EXPORT DiscussionListView : public MessageListViewBase
{
    Q_OBJECT
public:
    explicit DiscussionListView(QWidget *parent = nullptr);
    ~DiscussionListView() override;

protected:
    Q_REQUIRED_RESULT bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    Q_REQUIRED_RESULT bool mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;
};
