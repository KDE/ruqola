/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QLineEdit;
class MessageListView;
class QLabel;
class ListMessagesModelFilterProxyModel;
class RocketChatAccount;
class Room;
#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
namespace KPIMTextEditTextToSpeech
{
class TextToSpeechWidget;
}
#endif
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowListMessageBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowListMessageBaseWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowListMessageBaseWidget() override;

    void setModel(ListMessagesModelFilterProxyModel *model);
    Q_REQUIRED_RESULT MessageListView *messageListView() const;

    void setRoom(Room *room);

Q_SIGNALS:
    void loadMoreElements();

private:
    void updateLabel();
    void slotSearchMessageTextChanged(const QString &str);
#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
    void slotTextToSpeech(const QString &messageText);
#endif
    Q_REQUIRED_RESULT QString displayShowMessageInRoom() const;
    QLineEdit *const mSearchMessageLineEdit;
    QLabel *const mMessageListInfo;
    MessageListView *const mMessageListView;
    ListMessagesModelFilterProxyModel *mModel = nullptr;
#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
    KPIMTextEditTextToSpeech::TextToSpeechWidget *const mTextToSpeechWidget;
#endif
};
