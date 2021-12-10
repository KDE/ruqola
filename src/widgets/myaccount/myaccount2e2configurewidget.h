/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"

class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccount2e2ConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAccount2e2ConfigureWidget(QWidget *parent = nullptr);
    ~MyAccount2e2ConfigureWidget() override;
};
