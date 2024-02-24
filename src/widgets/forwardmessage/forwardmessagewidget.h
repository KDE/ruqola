/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "forwardmessage/forwardmessageaddchannelcompletionlineedit.h"
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class RocketChatAccount;
class FlowLayout;
class ClickableWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ForwardMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ForwardMessageWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ForwardMessageWidget() override;

    [[nodiscard]] QStringList channelIdentifiers() const;

Q_SIGNALS:
    void updateOkButton(bool enabled);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRemoveRoom(const QString &name);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotForwardToChannel(const JoinedChannelCompletionLineEditBase::JoinedChannelCompletionInfo &channelInfo);
    ForwardMessageAddChannelCompletionLineEdit *const mForwardMessageAddChannelCompletionLineEdit;
    FlowLayout *const mFlowLayout;
    QMap<QString, ClickableWidget *> mMap;
};
