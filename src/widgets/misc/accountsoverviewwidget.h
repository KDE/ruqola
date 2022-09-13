/*
 * SPDX-FileCopyrightText: 2020 Olivier de Gaalon <olivier.jg@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>

class QTabBar;
class AccountManager;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AccountsOverviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AccountsOverviewWidget(QWidget *parent = nullptr);
    ~AccountsOverviewWidget() override;
    void updateButtons();

    void showNextView();
    void showPreviousView();

private:
    void updateCurrentTab();
    void goToView(int index);

    QTabBar *const mTabBar;
    AccountManager *const mAccountManager;
};
