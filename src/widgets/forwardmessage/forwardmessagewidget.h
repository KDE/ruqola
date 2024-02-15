/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class ForwardMessageAddChannelCompletionLineEdit;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ForwardMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ForwardMessageWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ForwardMessageWidget() override;

private:
    ForwardMessageAddChannelCompletionLineEdit *const mForwardMessageAddChannelCompletionLineEdit;
};
