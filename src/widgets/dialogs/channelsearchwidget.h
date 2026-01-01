/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] QString channelName() const;

    void setChannelInfo(const QString &name, const QByteArray &channelId);

    [[nodiscard]] QByteArray channelId() const;
Q_SIGNALS:
    void updateRoomName(bool valid);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotClearRoom();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSelectedRoom(const ChannelSearchNameLineEdit::ChannelCompletionInfo &userInfo);
    ChannelSearchNameLineEdit::ChannelCompletionInfo mUserInfo;
    QStackedWidget *const mStackedWidget;
    ChannelSearchNameLineEdit *const mChannelSearchNameLineEdit;
    ChannelSearchNameLineResultWidget *const mChannelSearchLineResult;
};
