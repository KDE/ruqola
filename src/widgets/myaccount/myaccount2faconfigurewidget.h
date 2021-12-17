/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QCheckBox;
class RocketChatAccount;
class MyAccount2FaTotpWidget;
class QStackedWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccount2FaConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAccount2FaConfigureWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MyAccount2FaConfigureWidget() override;
    void load();
    void save();

    void initialize();

private:
    void slot2FAViaTOTPActivated(bool checked);
    void slotTotpResult(const QString &secret, const QString &url);
    QCheckBox *const mActivate2FAViaEmailCheckbox;
    QCheckBox *const mActivate2FAViaTOTPCheckbox;
    RocketChatAccount *const mRocketChatAccount;
    MyAccount2FaTotpWidget *const mMyAccount2FaTotpWidget;
    QStackedWidget *const mStackedWidget;
};

