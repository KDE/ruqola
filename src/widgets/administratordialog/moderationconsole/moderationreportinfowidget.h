/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class ModerationReportInfoListView;
class QLineEdit;
class NotificationHistoryModelFilterProxyModel;

namespace TextEditTextToSpeech
{
class TextToSpeechContainerWidget;
}

class LIBRUQOLAWIDGETS_TESTS_EXPORT ModerationReportInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModerationReportInfoWidget(QWidget *parent = nullptr);
    ~ModerationReportInfoWidget() override;

Q_SIGNALS:
    void showNotifyMessage(const QString &accountName, const QString &messageId, const QString &roomId);

private:
    void slotTextToSpeech(const QString &messageText);
    void slotCustomContextMenuRequested(const QPoint &pos);
    void slotShowMessage(const QModelIndex &index);
    void slotTextChanged(const QString &str);
    void slotFilterAccount(const QString &accountName);
    ModerationReportInfoListView *const mListNotificationsListView;
    QLineEdit *const mSearchLineEdit;
    NotificationHistoryModelFilterProxyModel *const mNotificationFilterProxyModel;
    TextEditTextToSpeech::TextToSpeechContainerWidget *const mTextToSpeechWidget = nullptr;
};
