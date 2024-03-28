/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "common/completionlineedit.h"
#include "libruqolawidgets_private_export.h"
class QJsonObject;
class InputCompleterModel;
class ChannelCompleterFilterProxyModel;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelSearchNameLineEdit : public CompletionLineEdit
{
    Q_OBJECT
public:
    struct ChannelCompletionInfo {
        QString channelName;
        QByteArray channelId;
    };
    explicit ChannelSearchNameLineEdit(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ChannelSearchNameLineEdit() override;
Q_SIGNALS:
    void newRoomName(const ChannelSearchNameLineEdit::ChannelCompletionInfo &userInfo);

private:
    void slotTextChanged(const QString &text);
    void slotComplete(const QModelIndex &index);
    void slotSearchDone(const QJsonObject &obj);
    ChannelCompleterFilterProxyModel *const mChannelCompleterFilterProxyModel;
    InputCompleterModel *const mChannelCompleterModel;
    RocketChatAccount *const mRocketChatAccount;
};
