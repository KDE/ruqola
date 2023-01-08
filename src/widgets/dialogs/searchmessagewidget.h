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
#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
namespace TextEditTextToSpeech
{
class TextToSpeechWidget;
}
#endif
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
#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
    void slotTextToSpeech(const QString &messageText);
#endif
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
#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
    TextEditTextToSpeech::TextToSpeechWidget *const mTextToSpeechWidget;
#endif
};
