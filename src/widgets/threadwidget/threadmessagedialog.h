/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "threadmessagewidget.h"
#include <QDialog>
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ThreadMessageDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ThreadMessageDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ThreadMessageDialog() override;

    void setThreadMessageInfo(const ThreadMessageWidget::ThreadMessageInfo &info);

private:
    void writeConfig();
    void readConfig();
    ThreadMessageWidget *const mThreadMessageWidget;
};
