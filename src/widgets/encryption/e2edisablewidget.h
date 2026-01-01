/*
  SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT E2eDisableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit E2eDisableWidget(QWidget *parent = nullptr);
    ~E2eDisableWidget() override;
};
