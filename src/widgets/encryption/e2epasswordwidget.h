/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT E2ePasswordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit E2ePasswordWidget(QWidget *parent = nullptr);
    ~E2ePasswordWidget() override;
};
