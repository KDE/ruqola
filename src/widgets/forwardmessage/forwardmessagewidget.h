/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class ForwardMessageAddChannelCompletionLineEdit;
class RocketChatAccount;
class FlowLayout;
class ClickableWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ForwardMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ForwardMessageWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ForwardMessageWidget() override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRemoveRoom(const QString &name);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAddNewName();
    ForwardMessageAddChannelCompletionLineEdit *const mForwardMessageAddChannelCompletionLineEdit;
    FlowLayout *const mFlowLayout;
    QMap<QString, ClickableWidget *> mMap;
};
