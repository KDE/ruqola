/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigureAccessibilityWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureAccessibilityWidget(QWidget *parent = nullptr);
    ~ConfigureAccessibilityWidget() override;
};
