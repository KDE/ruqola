/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

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
class CommonMessagesModel;

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

    void setRoom(Room *room);
    [[nodiscard]] RocketChatAccount *currentRocketChatAccount() const;

Q_SIGNALS:
    void goToMessageRequested(const QByteArray &messageId, const QString &messageDateTimeUtc);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchLineMessagesEnterPressed();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotClearedMessages();
    LIBRUQOLAWIDGETS_NO_EXPORT void updateLabel();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchMessages(const QString &str);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLoadHistory();
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString displayShowSearch() const;
    LIBRUQOLAWIDGETS_NO_EXPORT void clearSearchModel();
    LIBRUQOLAWIDGETS_NO_EXPORT void messageSearch(const QString &pattern, const QByteArray &rid, bool userRegularExpression, int offset = -1);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchMessagesDone(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void searchMessages(const QByteArray &roomId, const QString &pattern, bool useRegularExpression, int offset = -1);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchMessagesFailed();

    CommonMessagesModel *const mSearchMessageModel;
    CommonMessageFilterProxyModel *const mSearchMessageFilterProxyModel;

    QString mSearchText;
    QByteArray mRoomId;
    int mOffset = 0;
    bool mMessageIsEmpty = false;
    QLabel *const mSearchLabel;
    SearchMessageWithDelayLineEdit *const mSearchLineEdit;
    MessageListView *const mResultListWidget;
    RocketChatAccount *const mCurrentRocketChatAccount;
    TextEditTextToSpeech::TextToSpeechContainerWidget *const mTextToSpeechWidget = nullptr;
};
