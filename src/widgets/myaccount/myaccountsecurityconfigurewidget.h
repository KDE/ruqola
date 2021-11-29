/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccountSecurityConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAccountSecurityConfigureWidget(QWidget *parent = nullptr);
    ~MyAccountSecurityConfigureWidget() override;
};
