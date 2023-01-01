/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "channelsearchnamelineedit.h"
#include "libruqolawidgets_private_export.h"
class QStackedWidget;
class ChannelSearchNameLineEdit;
class ChannelSearchNameLineResultWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelSearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelSearchWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ChannelSearchWidget() override;

    Q_REQUIRED_RESULT QString channelName() const;

    void setChannelInfo(const QString &name, const QString &channelId);

    Q_REQUIRED_RESULT QString channelId() const;
Q_SIGNALS:
    void updateRoomName(bool valid);

private:
    void slotClearRoom();
    void slotSelectedRoom(const ChannelSearchNameLineEdit::ChannelCompletionInfo &userInfo);
    ChannelSearchNameLineEdit::ChannelCompletionInfo mUserInfo;
    QStackedWidget *const mStackedWidget;
    ChannelSearchNameLineEdit *const mChannelSearchNameLineEdit;
    ChannelSearchNameLineResultWidget *const mChannelSearchLineResult;
};
