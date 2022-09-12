/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class NotificationHistoryListView;
class QLineEdit;
class NotificationHistoryModelFilterProxyModel;

class LIBRUQOLAWIDGETS_TESTS_EXPORT NotificationHistoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NotificationHistoryWidget(QWidget *parent = nullptr);
    ~NotificationHistoryWidget() override;

Q_SIGNALS:
    void showNotifyMessage(const QString &accountName, const QString &messageId, const QString &roomId);

private:
    void slotCustomContextMenuRequested(const QPoint &pos);
    void slotShowMessage(const QModelIndex &index);
    void slotClearList();
    void slotTextChanged(const QString &str);
    void copyMessageToClipboard(const QModelIndex &index);
    NotificationHistoryListView *const mListNotificationsListView;
    QLineEdit *const mSearchLineEdit;
    NotificationHistoryModelFilterProxyModel *const mNotificationFilterProxyModel;
};
