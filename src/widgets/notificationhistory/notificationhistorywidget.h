/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class NotificationHistoryListView;
class QLineEdit;
class ServersComboBox;
class NotificationHistoryModelFilterProxyModel;

namespace TextEditTextToSpeech
{
class TextToSpeechContainerWidget;
}

class LIBRUQOLAWIDGETS_TESTS_EXPORT NotificationHistoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NotificationHistoryWidget(QWidget *parent = nullptr);
    ~NotificationHistoryWidget() override;

    void addServerList(const QStringList &serverNames);

Q_SIGNALS:
    void showNotifyMessage(const QString &accountName, const QString &messageId, const QString &roomId);

private:
    void slotTextToSpeech(const QString &messageText);
    void slotCustomContextMenuRequested(const QPoint &pos);
    void slotShowMessage(const QModelIndex &index);
    void slotTextChanged(const QString &str);
    void slotFilterAccount(const QString &accountName);
    NotificationHistoryListView *const mListNotificationsListView;
    QLineEdit *const mSearchLineEdit;
    NotificationHistoryModelFilterProxyModel *const mNotificationFilterProxyModel;
    TextEditTextToSpeech::TextToSpeechContainerWidget *const mTextToSpeechWidget = nullptr;
    ServersComboBox *const mServersComboBox;
};
