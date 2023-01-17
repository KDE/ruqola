/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class NotificationHistoryListView;
class QLineEdit;
class ServersComboBox;
class NotificationHistoryModelFilterProxyModel;
#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
namespace TextEditTextToSpeech
{
class TextToSpeechContainerWidget;
}
#endif
class LIBRUQOLAWIDGETS_TESTS_EXPORT NotificationHistoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NotificationHistoryWidget(QWidget *parent = nullptr);
    ~NotificationHistoryWidget() override;

Q_SIGNALS:
    void showNotifyMessage(const QString &accountName, const QString &messageId, const QString &roomId);

private:
#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
    void slotTextToSpeech(const QString &messageText);
#endif
    void slotCustomContextMenuRequested(const QPoint &pos);
    void slotShowMessage(const QModelIndex &index);
    void slotTextChanged(const QString &str);
    void slotFilterAccount(const QString &accountName);
    NotificationHistoryListView *const mListNotificationsListView;
    QLineEdit *const mSearchLineEdit;
    NotificationHistoryModelFilterProxyModel *const mNotificationFilterProxyModel;
#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
    TextEditTextToSpeech::TextToSpeechContainerWidget *const mTextToSpeechWidget;
#endif
    ServersComboBox *const mServersComboBox;
};
