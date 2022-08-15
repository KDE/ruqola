/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class MyAccount2FaConfigureWidget;
class MyAccountProfileConfigureWidget;
class MyAccountPreferenceConfigureWidget;
class MyAccount2e2ConfigureWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccountConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAccountConfigureWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MyAccountConfigureWidget() override;
    void save();
    void load();

    void initialize();

private:
    MyAccount2FaConfigureWidget *const mMyAccount2FaConfigureWidget;
    MyAccountProfileConfigureWidget *const mMyAccount2ProfileConfigureWidget;
    MyAccountPreferenceConfigureWidget *const mMyAccountPreferenceConfigureWidget;
    MyAccount2e2ConfigureWidget *const mMyAccount2e2ConfigureWidget;
    RocketChatAccount *const mRocketChatAccount;
};
