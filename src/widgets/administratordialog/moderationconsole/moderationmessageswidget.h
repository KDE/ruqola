/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class MessageListView;
class QLabel;
class CommonMessageFilterProxyModel;
class RocketChatAccount;

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
    void goToMessageRequested(const QByteArray &messageId, const QString &messageDateTimeUtc);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void updateLabel();
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString displayShowSearch() const;
    QLabel *const mSearchLabel;
    MessageListView *const mResultListWidget;
    CommonMessageFilterProxyModel *mModel = nullptr;
    RocketChatAccount *const mCurrentRocketChatAccount;
    TextEditTextToSpeech::TextToSpeechContainerWidget *const mTextToSpeechWidget = nullptr;
};
