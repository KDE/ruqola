/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"

#include <QWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT E2eCopyPasswordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit E2eCopyPasswordWidget(QWidget *parent = nullptr);
    ~E2eCopyPasswordWidget() override;
};
