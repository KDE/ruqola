/*
 * SPDX-FileCopyrightText: 2020 Olivier de Gaalon <olivier.jg@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

#include <QWidget>

class QTabBar;

#include "libruqolawidgets_private_export.h"
class LIBRUQOLAWIDGETS_TESTS_EXPORT AccountsOverviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AccountsOverviewWidget(QWidget *parent = nullptr);
    ~AccountsOverviewWidget() override;
    void updateButtons();

private:
    void updateCurrentTab();

    QTabBar *const mTabBar;
};
