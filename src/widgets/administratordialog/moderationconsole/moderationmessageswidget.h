/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QLineEdit;
class MessageListView;
class QLabel;
class CommonMessageFilterProxyModel;
class RocketChatAccount;
class Room;

namespace TextEditTextToSpeech
{
class TextToSpeechContainerWidget;
}

class LIBRUQOLAWIDGETS_TESTS_EXPORT ModerationMessagesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModerationMessagesWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ModerationMessagesWidget() override;

    void setModel(CommonMessageFilterProxyModel *model);

Q_SIGNALS:
    void goToMessageRequested(const QString &messageId, const QString &messageDateTimeUtc);

private:
    void slotSearchLineMessagesEnterPressed();
    void slotClearedMessages();
    void updateLabel();
    void slotSearchMessages(const QString &str);
    [[nodiscard]] QString displayShowSearch() const;
    QLabel *const mSearchLabel;
    QLineEdit *const mSearchLineEdit;
    MessageListView *const mResultListWidget;
    CommonMessageFilterProxyModel *mModel = nullptr;
    RocketChatAccount *const mCurrentRocketChatAccount;
    TextEditTextToSpeech::TextToSpeechContainerWidget *const mTextToSpeechWidget = nullptr;
};
