/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class SearchMessageWithDelayLineEdit;
class MessageListView;
class QLabel;
class CommonMessageFilterProxyModel;
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

    [[nodiscard]] QByteArray roomId() const;
    void setRoomId(const QByteArray &roomId);

    void setModel(CommonMessageFilterProxyModel *model);
    void setRoom(Room *room);
    [[nodiscard]] RocketChatAccount *currentRocketChatAccount() const;

Q_SIGNALS:
    void goToMessageRequested(const QByteArray &messageId, const QString &messageDateTimeUtc);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchLineMessagesEnterPressed();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotClearedMessages();
    LIBRUQOLAWIDGETS_NO_EXPORT void updateLabel();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchMessages(const QString &str);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString displayShowSearch() const;
    QByteArray mRoomId;
    QLabel *const mSearchLabel;
    SearchMessageWithDelayLineEdit *const mSearchLineEdit;
    MessageListView *const mResultListWidget;
    CommonMessageFilterProxyModel *mModel = nullptr;
    RocketChatAccount *const mCurrentRocketChatAccount;
    TextEditTextToSpeech::TextToSpeechContainerWidget *const mTextToSpeechWidget = nullptr;
};
