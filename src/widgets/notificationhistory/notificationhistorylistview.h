/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] const QString &searchText() const;
    void setSearchText(const QString &newSearchText);

    [[nodiscard]] QString selectedText() const;

protected:
    [[nodiscard]] bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    [[nodiscard]] bool mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index) override;

Q_SIGNALS:
    void showMessage(const QModelIndex &index);
    void textToSpeech(const QString &str);

private:
    [[nodiscard]] QString selectedText(const QModelIndex &index);
    void slotTextToSpeech(const QModelIndex &index);
    void slotCustomContextMenuRequested(const QPoint &pos);
    void slotClearList();
    void copyMessageToClipboard(const QModelIndex &index);
    NotificationHistoryDelegate *const mListNotificationsDelegate;
};
