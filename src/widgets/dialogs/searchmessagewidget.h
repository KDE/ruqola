/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class SearchMessageWithDelayLineEdit;
class MessageListView;
class QLabel;
class SearchMessageFilterProxyModel;
class RocketChatAccount;
class Room;

namespace TextEditTextToSpeech
{
class TextToSpeechContainerWidget;
}

class LIBRUQOLAWIDGETS_TESTS_EXPORT SearchMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchMessageWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~SearchMessageWidget() override;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    void setModel(SearchMessageFilterProxyModel *model);
    void setRoom(Room *room);
Q_SIGNALS:
    void goToMessageRequested(const QString &messageId, const QString &messageDateTimeUtc);

private:
    void slotSearchLineMessagesEnterPressed();
    void slotClearedMessages();
    void updateLabel();
    void slotSearchMessages(const QString &str);
    Q_REQUIRED_RESULT QString displayShowSearch() const;
    QString mRoomId;
    QLabel *const mSearchLabel;
    SearchMessageWithDelayLineEdit *const mSearchLineEdit;
    MessageListView *const mResultListWidget;
    SearchMessageFilterProxyModel *mModel = nullptr;
    RocketChatAccount *const mCurrentRocketChatAccount;
    TextEditTextToSpeech::TextToSpeechContainerWidget *const mTextToSpeechWidget = nullptr;
};
