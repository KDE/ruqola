/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QListView;
class QLineEdit;
class NotificationHistoryDelegate;

class LIBRUQOLAWIDGETS_TESTS_EXPORT NotificationHistoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NotificationHistoryWidget(QWidget *parent = nullptr);
    ~NotificationHistoryWidget() override;

Q_SIGNALS:
    void showNotifyMessage(const QString &accountName, const QString &messageId, const QString &roomId);

protected:
    void resizeEvent(QResizeEvent *ev) override;

private:
    void slotCustomContextMenuRequested(const QPoint &pos);
    void slotShowMessage(const QModelIndex &index);
    void slotClearList();
    void checkIfAtBottom();
    void maybeScrollToBottom();
    void updateVerticalPageStep();
    QListView *const mListNotifications;
    QLineEdit *const mSearchLineEdit;
    NotificationHistoryDelegate *mListNotificationsDelegate = nullptr;
    bool mAtBottom = true;
};
