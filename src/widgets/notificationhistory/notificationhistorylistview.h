/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include "misc/messagelistviewbase.h"
class NotificationHistoryDelegate;

class LIBRUQOLAWIDGETS_TESTS_EXPORT NotificationHistoryListView : public MessageListViewBase
{
    Q_OBJECT
public:
    explicit NotificationHistoryListView(QWidget *parent = nullptr);
    ~NotificationHistoryListView() override;

    void clearCache();

    void slotSelectAll(const QModelIndex &index);

    Q_REQUIRED_RESULT const QString &searchText() const;
    void setSearchText(const QString &newSearchText);

    Q_REQUIRED_RESULT QString selectedText() const;

protected:
    Q_REQUIRED_RESULT bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    Q_REQUIRED_RESULT bool mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    NotificationHistoryDelegate *const mListNotificationsDelegate;
};
