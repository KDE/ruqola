/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

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

class LIBRUQOLAWIDGETS_TESTS_EXPORT ModerationUsersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModerationUsersWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ModerationUsersWidget() override;

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
