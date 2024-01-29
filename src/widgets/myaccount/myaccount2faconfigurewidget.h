/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QCheckBox;
class RocketChatAccount;
class MyAccount2FaTotpWidget;
class MyAccount2FaDisableTotpWidget;
class QStackedWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccount2FaConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    enum PageType {
        EmptyPage = 0,
        Enable2FaPage,
        Disable2FaPage,
    };
    explicit MyAccount2FaConfigureWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MyAccount2FaConfigureWidget() override;
    void load();
    void save();

    void initialize();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slot2FAViaTOTPActivated(bool checked);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTotpResult(const QString &secret, const QString &url);
    QCheckBox *const mActivate2FAViaEmailCheckbox;
    QCheckBox *const mActivate2FAViaTOTPCheckbox;
    RocketChatAccount *const mRocketChatAccount;
    MyAccount2FaTotpWidget *const mMyAccount2FaTotpWidget;
    MyAccount2FaDisableTotpWidget *const mMyAccountDisable2FaTotpWidget;
    QWidget *const mMyAccount2FaEmpty;
    QStackedWidget *const mStackedWidget;
};
