/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QLabel;
class RocketChatAccount;

class LIBRUQOLAWIDGETS_TESTS_EXPORT ModerationUsersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModerationUsersWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ModerationUsersWidget() override;

private:
    RocketChatAccount *const mCurrentRocketChatAccount;
};
