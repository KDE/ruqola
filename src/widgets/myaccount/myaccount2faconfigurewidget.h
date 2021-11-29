/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QCheckBox;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccount2FaConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAccount2FaConfigureWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MyAccount2FaConfigureWidget() override;
    void load();
    void save();

private:
    QCheckBox *const mActivate2FAViaEmailCheckbox;
    RocketChatAccount *const mRocketChatAccount;
};

